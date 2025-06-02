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

//Yes, I'm that lazy
Vector2 Vector2SumAndScale(Vector2 v1, Vector2 v2, float Scale) {
	Vector2 Returner = {v1.x + v2.x * Scale, v1.y + v2.y * Scale};
	return Returner;
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

		DrawCircleV(playerPos, 10, RED);
		DrawLineV(playerPos, Vector2SumAndScale(playerPos, playerDir, 25.0f), BLUE);

		for(int i = 0; i < TestSec.SideDefsNumb; i++) {
			Vector2 start = TestSec.SideDefsCords[i];
			Vector2 end = TestSec.SideDefsCords[(i + 1) % TestSec.SideDefsNumb];

			DrawLineV(start, end, BLACK);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
