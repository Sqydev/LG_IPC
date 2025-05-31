#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <threads.h>

#define TILE_SIZE 128
#define Window_Width 960 * WindowScale
#define Window_Height 540 * WindowScale
#define RaysNumb 960 * WindowScale

//Btw, there are farlands

//Global important vals(Like thoes In settings)
float WindowScale = 1;
int Fpss = 60;
int RenderDistans = 100000;
float playerFov = 70 * (PI / 180);
float mouseSensitivity = 0.003f;

int DebugMap[100][100] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,7,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,10,0,0,0,0,1,0,0,0,4,0,5,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,4,4,0,5,5,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,3,3,0,2,2,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,9,0,0,0,0,1,0,0,0,3,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,6,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int TextureStep = 63;
Texture2D Textures[100];

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
	Vector2 returner = {v1.x + v2.x, v1.y + v2.y};
	return returner;
}

int ScanForWall(Vector2 EndPos, int LevelId) {
	//Zrób jakoś żeby można było w sprawdzaniu  dać mapę i szybko to dodać(może macro?)
	int OnMapX = EndPos.x / TILE_SIZE;
	int OnMapY = EndPos.y / TILE_SIZE;

	//Zrób rzeby na różnych mapach działało
	if(LevelId == 0) return DebugMap[OnMapX][OnMapY];
	else return 0;
}

void RayCasting(Vector2 playerPos, float playerAngle, int LevelId) {
	for(int i = 0; i < RaysNumb; i++) {
        float rayAngle = playerAngle - (playerFov / 2) + i * (playerFov / RaysNumb);
		float distance = 0.0f;
		int type;
		float textureoffset;

		for(int j = 1; j <= RenderDistans; j++) {
			Vector2 rayDir = {cosf(rayAngle), sinf(rayAngle)};

			Vector2 EndPos = {playerPos.x + rayDir.x * j, playerPos.y + rayDir.y * j};

			if(ScanForWall(EndPos, LevelId) == 0) {
				continue;
			}
			else {
				float offsetX = fmod(EndPos.x, TILE_SIZE);
    			float offsetY = fmod(EndPos.y, TILE_SIZE);
				
				distance = j;
				type = ScanForWall(EndPos, LevelId);

				if (offsetX < 1.0f || offsetX > TILE_SIZE - 1.0f) {
        			textureoffset = offsetY;
					
					if(rayDir.x < 0) {
        				textureoffset = TILE_SIZE - textureoffset;
    				}
    			}
				else {
        			textureoffset = offsetX;
					
					if(rayDir.y > 0) {
        				textureoffset = TILE_SIZE - textureoffset;
    				}
    			}
				break;
			}
		}
        float correctedDis = distance * cosf(rayAngle - playerAngle);

        float wallHeight = (TILE_SIZE * (float)(Window_Height) / 2) / correctedDis;
		
		float screenX = (float)i * ((float)Window_Width / RaysNumb);
				
		//Remember deepshit that src is how to cut it and drc is the place to pase it
		float textureX = (textureoffset / TILE_SIZE) * Textures[type].width;
		Rectangle src = {textureX, 0, 1, Textures[type].height};
		Rectangle drc = {screenX, Window_Height / 2 - wallHeight / 2, (float)(Window_Width) / RaysNumb, wallHeight};
		DrawTexturePro(Textures[type], src, drc, (Vector2){0, 0}, 0.0f, WHITE);
	}
}

int main() {
	//Znajdz gdzie jest win_w/h * winScale i usuń winscale
	InitWindow(Window_Width, Window_Height, "2.5D, version 2");
	SetTargetFPS(Fpss);
	DisableCursor();

	Textures[1] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/GRAY.png");
	Textures[2] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/RED.png");
	Textures[3] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/GREEN.png");
	Textures[4] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/BLUE.png");
	Textures[5] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/YELLOW.png");
	Textures[6] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/I'mNotGonnaSugarcoteIt(cutted).jpg");
	Textures[7] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/Minos_chilling.png");
	Textures[8] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/miblox_prime.jpg");
	Textures[9] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/Is_that_minos_prime.png");
	Textures[10] = LoadTexture("/home/sqyd/projekty/c/LG_IPC/Learning_Raylib/sprites(3d)/Yes_that_is_minos_prime.png");

	//Important vals that are dynamic and game can chainge them(playerPos when going
	//to new level)
	int PlayMode = 0;
	int LevelId = 0; //Level 0 will always be debug level
	float playerAngle = 0.0f;
	Vector2 playerPos = {TILE_SIZE * 15.5f, TILE_SIZE * 15.5f};
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

			if(IsKeyDown(KEY_LEFT_SHIFT)) playerSpeed = 500;
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
					RayCasting(playerPos, playerAngle, LevelId);
				}
				else {
					//Minimap stuff
					DrawCircleV(playerPos, 25, RED);
				}

				EndDrawing();
			}
		}
	}
	UnloadTexture(Textures[5]);

	return 0;
}
