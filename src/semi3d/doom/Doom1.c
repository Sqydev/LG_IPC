#include "raylib.h"
#include <math.h>

#define Window_Width (960 * WindowScale)
#define Window_Height (540 * WindowScale)
#define Minimum_X_In_Rendering playerFov / Window_Width
#define NumbOfMapSectors 100

float WindowScale = 1.0f;
int TargetFps = 60;
float MouseSensevity = 0.003f;

typedef struct Sector {
	float FloorHeight;
	float CeilingHeight;
	char FloorTexturePath[200];
	char CeilingTexturePath[200];
	float LightLevel;
	int SideDefsNumb;
	Vector2 SideDefsCords[100];
} Sector;

Vector2 Vector2Sum(Vector2 v1, Vector2 v2) {
	Vector2 Returner = {v1.x + v2.x, v1.y + v2.y};
	return Returner;
}

Vector2 Vector2SumAndScale(Vector2 v1, Vector2 v2, float Scale) {
	Vector2 Returner = {v1.x + v2.x * Scale, v1.y + v2.y * Scale};
	return Returner;
}

Vector2 ConvertSecPosRelativeToPlayer(Sector sector, Vector2 playerPos, int ArrayPlace, float playerAngle) {
	Vector2 Buff = {sector.SideDefsCords[ArrayPlace].x - playerPos.x, sector.SideDefsCords[ArrayPlace].y - playerPos.y};

	float cosA = cosf(-playerAngle);
	float sinA = sinf(-playerAngle);

	Vector2 RotatedBuff = {
		Buff.x * cosA - Buff.y * sinA,
		Buff.x * sinA + Buff.y * cosA
	};

	return RotatedBuff;
}


int main() {
	InitWindow(Window_Width, Window_Height, "Doom Like Engine");
	SetTargetFPS(TargetFps);
	DisableCursor();

	Vector2 playerPos = {0, 0};
	float playerAngle = 0.0f;
	float playerFov = 90 * DEG2RAD;
	float playerHeight = 32.0f;
	
	// Create array of MapSectors
	Sector MapSectors[NumbOfMapSectors] = {
		// Every {} like this is new sector so:
		// New
		{
			.FloorHeight = 0.0f,
			.CeilingHeight = 64.0f,
			.LightLevel = 1.0f,
			.SideDefsNumb = 4,
			.SideDefsCords = {
				{100, 100},
        		{164, 100},
        		{164, 164},
        		{100, 164}
			}
		},
		// New
		{
			.FloorHeight = 0.0f,
			.CeilingHeight = 32.0f,
			.LightLevel = 1.0f,
			.SideDefsNumb = 4,
			.SideDefsCords = {
				{200, 200},
        		{264, 200},
        		{264, 264},
        		{200, 264}
			}	
		}
	};

	while(!WindowShouldClose()) {
		float dt = GetFrameTime();
		float md = GetMouseDelta().x;
	
		Vector2 playerDir = {cosf(playerAngle), sinf(playerAngle)};

		playerAngle += MouseSensevity * md;

		int playerSpeed = 300;
	
		if(IsKeyDown(KEY_W)) {
			playerPos.x += playerDir.x * playerSpeed * dt;
			playerPos.y += playerDir.y * playerSpeed * dt;
		}
		if(IsKeyDown(KEY_S)) {
			playerPos.x -= playerDir.x * playerSpeed * dt;
			playerPos.y -= playerDir.y * playerSpeed * dt;
		}
		if(IsKeyDown(KEY_D)) {
			playerPos.x += cosf(playerAngle + PI / 2) * playerSpeed * dt;
			playerPos.y += sinf(playerAngle + PI / 2) * playerSpeed * dt;
		}
		if(IsKeyDown(KEY_A)) {
			playerPos.x -= cosf(playerAngle + PI / 2) * playerSpeed * dt;
			playerPos.y -= sinf(playerAngle + PI / 2) * playerSpeed * dt;
		}

		BeginDrawing();
		ClearBackground(WHITE);

		for (int a = 0; a < NumbOfMapSectors; a++) {
			for(int i = 0; i < MapSectors[a].SideDefsNumb; i++) {
			    Vector2 start = ConvertSecPosRelativeToPlayer(MapSectors[a], playerPos, i, playerAngle);
    			Vector2 end = ConvertSecPosRelativeToPlayer(MapSectors[a], playerPos, (i + 1) % MapSectors[a].SideDefsNumb, playerAngle);
    			
				// This is to fix bug that wall is all over the screen when player is loocking is spec. location
				if (start.x < Minimum_X_In_Rendering && start.x != 0) start.x = 0;
				if (end.x < Minimum_X_In_Rendering && end.x != 0) end.x = 0;

    			// Skip walls behind player
    			if (start.x <= 0 && end.x <= 0) continue;

    			int screenCenterX = Window_Width / 2;
				float fovRatio = (float)Window_Width / playerFov;

    			int x1 = screenCenterX + (start.y * fovRatio / start.x);
    			int x2 = screenCenterX + (end.y * fovRatio / end.x);

    			// Z perspective
    			float z1Floor = (MapSectors[a].FloorHeight - playerHeight) / start.x * fovRatio;
    			float z1Ceil  = (MapSectors[a].CeilingHeight - playerHeight) / start.x * fovRatio;
    			float z2Floor = (MapSectors[a].FloorHeight - playerHeight) / end.x * fovRatio;
    			float z2Ceil  = (MapSectors[a].CeilingHeight - playerHeight) / end.x * fovRatio;

    			int yFloor1 = Window_Height / 2 - z1Floor;
    			int yCeil1  = Window_Height / 2 - z1Ceil;
    			int yFloor2 = Window_Height / 2 - z2Floor;
    			int yCeil2  = Window_Height / 2 - z2Ceil;

				// Draw full wall quad
				DrawTriangleStrip(
    				(Vector2[]){
        				{x1, yCeil1},
        				{x2, yCeil2},
        				{x1, yFloor1},
        				{x2, yFloor2}
    				},
    				4,
    				DARKGRAY
				);

			}
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
