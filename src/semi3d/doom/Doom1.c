#include "raylib.h"
#include <math.h>

#define Window_Width (960 * WindowScale)
#define Window_Height (540 * WindowScale)
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
	// Do this to make player the center of the cord's
	Vector2 CordsRelativeToPlayer = {sector.SideDefsCords[ArrayPlace].x - playerPos.x, sector.SideDefsCords[ArrayPlace].y - playerPos.y};

	// Do cos and sin of playerAngle
	float cosAngle = cosf(-playerAngle);
	float sinAngle = sinf(-playerAngle);

	// Rotate converted cord's so they are also Rotated like the player
	Vector2 RotatedCordsRelativeToPlayer = {
		CordsRelativeToPlayer.x * cosAngle - CordsRelativeToPlayer.y * sinAngle,
		CordsRelativeToPlayer.x * sinAngle + CordsRelativeToPlayer.y * cosAngle
	};

	return RotatedCordsRelativeToPlayer;
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
				// Convert cord's of sector's edges used in this iteration to know where are they in player's perspective
				// start.x and end.x are showing how far is sector's edge and start.y and end.y are showing how much to the side is sectors edge
				// And they are named start and end cuz it's rendering only 2 "walls" of sector per iteration
			    Vector2 start = ConvertSecPosRelativeToPlayer(MapSectors[a], playerPos, i, playerAngle);
    			Vector2 end = ConvertSecPosRelativeToPlayer(MapSectors[a], playerPos, (i + 1) % MapSectors[a].SideDefsNumb, playerAngle);
    			
				// This is to fix bug that wall is all over the screen when player is loocking is spec. location
				// And this version fixes grafick bug when you're IN sector too:)
				if ((start.x <= 0 && end.x > 0) || (start.x > 0 && end.x <= 0)) continue;

				// Skip walls behind the player
    			if (start.x <= 0 && end.x <= 0) continue;

				// Find center of screen
    			int screenCenterX = Window_Width / 2;
				// fovRatio is how many pixels of the screen are for 1 playerFov step. So f.e 10 pixels per 1 fov step
				float fovRatio = (float)Window_Width / playerFov;

				// Calculate how far from screenCenterX in x accis should be start point of side of sector and a seccount one
				// And looking at comment above start and end init this formula makes perfect sence cuz it's screenCenterX + (how much to the side * how much pixels are for degree / how far)
    			int x1 = screenCenterX + (start.y * fovRatio / start.x);
    			int x2 = screenCenterX + (end.y * fovRatio / end.x);

    			// Z perspective(to explain)
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

		// Print Fps
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
