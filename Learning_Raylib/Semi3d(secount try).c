#include "raylib.h"
#include <math.h>
#include <stdbool.h>

#define TILE_SIZE 64

//Btw, there are farlands

//Global important vals(Like thoes In settings)
int WindowScale = 1;
int Window_Width = 960;
int Window_Height = 540;
int Fpss = 60;
int RaysNumb = 300;
int RenderDistans = 100000;
float playerFov = 60 * (PI / 180);
float mouseSensitivity = 0.003f;

int DebugMap[11][11] = {
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,0,1,0,0,0,1},
	{1,0,0,1,1,0,1,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,1,1,0,1,1,0,0,1},
	{1,0,0,0,1,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1},
};

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
	Vector2 returner = {v1.x + v2.x, v1.y + v2.y};
	return returner;
}

bool IsItAWall(Vector2 EndPos, int LevelId) {
	//Zrób jakoś żeby można było w sprawdzaniu  dać mapę i szybko to dodać(może macro?)
	int OnMapX = EndPos.x / TILE_SIZE;
	int OnMapY = EndPos.y / TILE_SIZE;

	//Zrób rzeby na różnych mapach działało
	return DebugMap[OnMapX][OnMapY];
}

void RayCasting(Vector2 playerPos, float playerAngle, int LevelId) {
	for(int i = 0; i < RaysNumb; i++) {
        float rayAngle = playerAngle - (playerFov / 2) + i * (playerFov / RaysNumb);
		float distance = 0.0f;

		for(int j = 1; j <= RenderDistans; j++) {
			Vector2 rayDir = {cosf(rayAngle), sinf(rayAngle)};

			Vector2 EndPos = {playerPos.x + rayDir.x * j, playerPos.y + rayDir.y * j};
			

			if(IsItAWall(EndPos, LevelId) == false) {
				continue;
			}
			else {
				distance = j;
				break;
			}
		}
        float correctedDis = distance * cosf(rayAngle - playerAngle);

        float wallHeight = (TILE_SIZE * (float)(Window_Height * WindowScale) / 2) / correctedDis;
				
        float screenX = (float)i * ((float)(Window_Width * WindowScale) / RaysNumb);

        DrawRectangle(screenX, 300 - wallHeight / 2, (float)(Window_Height * WindowScale) / RaysNumb, wallHeight, GRAY);
	}
}

int main() {
	InitWindow(Window_Width * WindowScale, Window_Height * WindowScale, "2.5D, version 2");
	SetTargetFPS(Fpss);
	DisableCursor();

	//Important vals that are dynamic and game can chainge them(playerPos when going
	//to new level)
	int PlayMode = 0;
	int LevelId = 0; //Level 0 will always be debug level
	float playerAngle = 0.0f;
	Vector2 playerPos = {340.0f, 340.0f};
	Vector2 playerDir = {0, 0};
	float playerFov = 60.0f;

	while(!WindowShouldClose()) {
		//thoes ones used everywhere and updated every seccount
		float dt = GetFrameTime();
		float dm = GetMouseDelta().x;

		if(PlayMode == 0) {
			//Menu, later
			//Debug setup
			PlayMode = 1;
			LevelId = 0;
		}
		//Elseif and not else couse I don't know if there will be any new Mode
		else if(PlayMode == 1 || PlayMode == 2) {
			//Level universal calculations
			playerAngle += dm * mouseSensitivity;
			
			Vector2 playerDir = {
				cosf(playerAngle),
				sin(playerAngle)
			};

			//Keyboard calculations
			int playerSpeed = 100;

			if(IsKeyDown(KEY_LEFT_SHIFT)) playerSpeed = 300;
			if(IsKeyDown(KEY_W)) {
    			playerPos.x += cosf(playerAngle) * playerSpeed * dt;
    			playerPos.y += sinf(playerAngle) * playerSpeed * dt;
			}
			if(IsKeyDown(KEY_S)) {
    			playerPos.x -= cosf(playerAngle) * playerSpeed * dt;
    			playerPos.y -= sinf(playerAngle) * playerSpeed * dt;
			}
			if(IsKeyDown(KEY_D)) {
    			playerPos.x -= cosf(playerAngle - PI / 2) * playerSpeed * dt;
    			playerPos.y -= sinf(playerAngle - PI / 2) * playerSpeed * dt;
			}
			if(IsKeyDown(KEY_A)) {
    			playerPos.x -= cosf(playerAngle + PI / 2) * playerSpeed * dt;
    			playerPos.y -= sinf(playerAngle + PI / 2) * playerSpeed * dt;
			}

			if(IsKeyPressed(KEY_TAB) && PlayMode == 1) PlayMode = 2;
			if(IsKeyPressed(KEY_TAB) && PlayMode == 2) PlayMode = 1;

			if(LevelId == 0) {
				//All the calcs are in Minimap & Playing Modes in the same time
				//(universal or something)

				//Drawing part
				BeginDrawing();
				ClearBackground(WHITE);
				if(PlayMode == 1) {
					RayCasting(playerPos, playerAngle, 0);
				}
				else {
					//Minimap stuff
					DrawCircleV(playerPos, 25, RED);
					RayCasting(playerPos, playerAngle, LevelId);
				}

				EndDrawing();
			}
		}
	}

	return 0;
}
