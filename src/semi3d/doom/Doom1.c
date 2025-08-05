#include "raylib.h"
#include <math.h>

#define Window_Width (960 * WindowScale)
#define Window_Height (540 * WindowScale)

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

	Vector2 playerPos = {150, 150};
	float playerAngle = 0.0f;
	float playerFov = 90 * DEG2RAD;

	Sector TestSec = {
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
			playerPos.x -= cosf(playerAngle + PI / 2) * playerSpeed * dt;
			playerPos.y -= sinf(playerAngle + PI / 2) * playerSpeed * dt;
		}
		if(IsKeyDown(KEY_A)) {
			playerPos.x += cosf(playerAngle + PI / 2) * playerSpeed * dt;
			playerPos.y += sinf(playerAngle + PI / 2) * playerSpeed * dt;
		}

		BeginDrawing();
		ClearBackground(WHITE);

		for(int i = 0; i < TestSec.SideDefsNumb; i++) {
		    Vector2 start = ConvertSecPosRelativeToPlayer(TestSec, playerPos, i, playerAngle);
			Vector2 end = ConvertSecPosRelativeToPlayer(TestSec, playerPos, (i + 1) % TestSec.SideDefsNumb, playerAngle);

			// Skip walls behind player
			if (start.x <= 0 && end.x <= 0) continue;

			// Projection
			float fovRatio = (float)Window_Width / playerFov;
			int screenCenterX = Window_Width / 2;

			int x1 = screenCenterX + (start.y * fovRatio / start.x);
			int x2 = screenCenterX + (end.y * fovRatio / end.x);

			// Calc wall height on screen
			float wallHeight1 = (TestSec.CeilingHeight - TestSec.FloorHeight) / start.x * fovRatio;
			float wallHeight2 = (TestSec.CeilingHeight - TestSec.FloorHeight) / end.x * fovRatio;

			// Calc y pos dependent on position
			int yFloor1 = Window_Height / 2 + (wallHeight1 / 2);
			int yCeil1  = Window_Height / 2 - (wallHeight1 / 2);

			int yFloor2 = Window_Height / 2 + (wallHeight2 / 2);
			int yCeil2  = Window_Height / 2 - (wallHeight2 / 2);
			
			// Draw
			DrawTriangle((Vector2){x1, yCeil1}, (Vector2){x2, yCeil2}, (Vector2){x1, yFloor1}, DARKGRAY);
			DrawTriangle((Vector2){x1, yFloor1}, (Vector2){x2, yFloor2}, (Vector2){x2, yCeil2}, DARKGRAY);

		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
