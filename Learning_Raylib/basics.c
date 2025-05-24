#include "raylib.h"
#include <stdio.h>

bool IsHitboxRetarded(Rectangle r1, Rectangle r2) {
    return (r1.x < r2.x + r2.width) &&
           (r1.x + r1.width > r2.x) &&
           (r1.y < r2.y + r2.height) &&
           (r1.y + r1.height > r2.y);
}

int main() {
    InitWindow(800, 600, "Raylib");
    SetTargetFPS(60);

    Vector2 playerPos = {400, 300};

	Texture2D DefaultPlayerSprite = LoadTexture("Learning_Raylib/sprites/d-s.png");

	int Health = 100;

    while (!WindowShouldClose()) {
    	float dt = GetFrameTime();

		int speed = 100;

		Rectangle PlayerHitbox = {
			playerPos.x - (DefaultPlayerSprite.width * 0.2) / 2,
    		playerPos.y - (DefaultPlayerSprite.height * 0.2) / 2,
    		DefaultPlayerSprite.width * 0.2,
    		DefaultPlayerSprite.height * 0.2
		};

		Rectangle RactangleHitbox = {700, 300, 100, 50};

		if(IsHitboxRetarded(PlayerHitbox, RactangleHitbox)) Health = Health - 10;

		if(IsKeyDown(KEY_LEFT_SHIFT)) speed = 300;
		
		if(IsKeyDown(KEY_W) && playerPos.y > 0) playerPos.y -= speed * dt;
		if(IsKeyDown(KEY_S) && playerPos.y < 423) playerPos.y += speed * dt;
		if(IsKeyDown(KEY_D) && playerPos.x < 695) playerPos.x += speed * dt;
		if(IsKeyDown(KEY_A) && playerPos.x > 0) playerPos.x -= speed * dt;

		Vector2 drawPos = {
    		playerPos.x - DefaultPlayerSprite.width / 20,
    		playerPos.y
		};
		
		BeginDrawing();

		ClearBackground(WHITE);

		DrawTextureEx(DefaultPlayerSprite, drawPos, 0, 0.2, WHITE);

		DrawRectangle(700, 300, 100, 50, RED);

		char texter[100];
		sprintf(texter, "%d, %d", (int)playerPos.x, (int)playerPos.y);

		DrawText(texter, 7, 0, 20, BLACK);

		sprintf(texter, "%d", Health);

		DrawText(texter, 720, 550, 35, BLACK);
		
		EndDrawing();
	}

    CloseWindow();
    
	UnloadTexture(DefaultPlayerSprite);

	return 0;
}
