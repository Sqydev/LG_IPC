#include "raylib.h"
#include <math.h>

//Why I'm doing it again in v2, becouse It's a mess and It was my dirtynotebook or how
//do you call it in us/usa, and the same will be with v2,v3 and posibbly v4 couse I
//want to acculy learn It :)

#define WINDOW_W 1280
#define WINDOW_H 720
#define FPSS 60
#define DEBUG_MAP_W 10
#define DEBUG_MAP_H 10
#define TILE_SIZE 64.0f

int map[DEBUG_MAP_W][DEBUG_MAP_H] = {
    	{1,1,1,1,1,1,1,1,1,1},
    	{1,0,0,0,0,0,0,0,0,1},
    	{1,0,0,0,0,0,0,0,0,1},
    	{1,0,0,1,1,1,0,0,0,1},
    	{1,0,0,1,0,1,0,0,0,1},
    	{1,0,0,1,0,1,0,0,0,1},
    	{1,0,0,0,0,0,0,0,0,1},
    	{1,0,0,0,0,0,0,0,0,1},
    	{1,0,0,0,0,0,0,0,0,1},
    	{1,1,1,1,1,1,1,1,1,1},
};

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
	Vector2 Returner = {v1.x + v2.x, v1.y + v2.y};

	return Returner;
}

bool IsThereAnWall(float x, float y) {
    int mapX = (int)(x / TILE_SIZE);
    int mapY = (int)(y / TILE_SIZE);

    if (mapX < 0 || mapX >= DEBUG_MAP_W || mapY < 0 || mapY >= DEBUG_MAP_H) return true;
    return map[mapY][mapX] == 1;
}

void RayCasting(Vector2 playerPos, float playerAngle, float playerFov) {
    int numRays = 300;
    float angleStep = playerFov / numRays;

    for (int i = 0; i < numRays; i++) {
        float rayAngle = playerAngle - (playerFov / 2) + i * angleStep;

        Vector2 rayPos = playerPos;
        Vector2 rayDir = {cosf(rayAngle), sinf(rayAngle)};

        float distance = 0.0f;

        while (!IsThereAnWall(rayPos.x, rayPos.y) && distance < 1000.0f) {
            rayPos.x += rayDir.x;
            rayPos.y += rayDir.y;
            distance += 1.0f;
        }

        float correctedDist = distance * cosf(rayAngle - playerAngle);

        float wallHeight = (TILE_SIZE * (float)WINDOW_H/2) / correctedDist;

        float screenX = (float)i * ((float)WINDOW_W / numRays);
        DrawRectangle(screenX, 300 - wallHeight / 2, (float)WINDOW_W / numRays, wallHeight, GRAY);
    }
}

int main() {	
	InitWindow(WINDOW_W, WINDOW_H, "2.5D");
	SetTargetFPS(FPSS);
	DisableCursor();

	Vector2 playerPos = {5.0f * TILE_SIZE, 5.0f * TILE_SIZE};

	float playerAngle = 0.0f;

	int playerFov = 60.0f * (PI / 180);

	float mouseSenses = 0.003f;

	while(!WindowShouldClose()) {
		float dt = GetFrameTime();
		float mouseDelta = GetMouseDelta().x;
		int speed = 100;

		Vector2 dir = {
    		cosf(playerAngle) * 60,
    		sinf(playerAngle) * 60
		};

		if(IsKeyDown(KEY_LEFT_SHIFT)) speed = 300;

		playerAngle += mouseDelta * mouseSenses;

		if(IsKeyDown(KEY_W)) {
    		playerPos.x += cosf(playerAngle) * speed * dt;
    		playerPos.y += sinf(playerAngle) * speed * dt;
		}
		if(IsKeyDown(KEY_S)) {
    		playerPos.x -= cosf(playerAngle) * speed * dt;
    		playerPos.y -= sinf(playerAngle) * speed * dt;
		}
		if(IsKeyDown(KEY_D)) {
    		playerPos.x -= cosf(playerAngle - PI / 2) * speed * dt;
    		playerPos.y -= sinf(playerAngle - PI / 2) * speed * dt;
		}
		if(IsKeyDown(KEY_A)) {
    		playerPos.x -= cosf(playerAngle + PI / 2) * speed * dt;
    		playerPos.y -= sinf(playerAngle + PI / 2) * speed * dt;
		}
		
		if(IsKeyDown(KEY_X)) playerAngle += 0.05f;

		BeginDrawing();

		ClearBackground(WHITE);

		RayCasting(playerPos, playerAngle, playerFov);

		EndDrawing();
	}

	return 0;
}
