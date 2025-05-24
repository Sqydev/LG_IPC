#include "raylib.h"
#include <stdio.h>

int main() {
    InitWindow(800, 600, "Raylib");
    SetTargetFPS(60);

    Vector2 playerPos = {400, 300};

	Texture2D DefaultPlayerSprite = LoadTexture("Learning_Raylib/sprites/d-s.png");

    while (!WindowShouldClose()) {
    	float dt = GetFrameTime();

		int speed = 100;

		if(IsKeyDown(KEY_LEFT_SHIFT)) speed = 300;
		
		if(IsKeyDown(KEY_W) && playerPos.y > 0) playerPos.y -= speed * dt;
		if(IsKeyDown(KEY_S) && playerPos.y < 600) playerPos.y += speed * dt;
		if(IsKeyDown(KEY_D) && playerPos.x < 800) playerPos.x += speed * dt;
		if(IsKeyDown(KEY_A) && playerPos.x > 0) playerPos.x -= speed * dt;

		Vector2 drawPos = {
    		playerPos.x - DefaultPlayerSprite.width / 20,
    		playerPos.y
		};
		
		BeginDrawing();

		ClearBackground(WHITE);

		char texter[100];
		sprintf(texter, "%d, %d", (int)playerPos.x, (int)playerPos.y);

		DrawTextureEx(DefaultPlayerSprite, drawPos, 0, 0.2, WHITE);

		DrawText(texter, 7, 0, 20, BLACK);
		
		EndDrawing();
	}

    CloseWindow();
    
	UnloadTexture(DefaultPlayerSprite);

	return 0;
}

