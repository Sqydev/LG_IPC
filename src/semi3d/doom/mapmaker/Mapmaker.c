#include <raylib.h>
#include <stdlib.h>

#define screenWidth 800
#define screenHeight 400
	
typedef struct Sector {
		float FloorHeight;
		float CeilingHeight;
		char FloorTexturePath[200];
		char CeilingTexturePath[200];
		float LightLevel;
		int SideDefsNumb;
		Vector2 SideDefsCords[100];
	} Sector;

int NumbOfMapSectors = 0;

void CreateSector(Vector2 newSectorsPos, Sector *MapSectors) {
}

int main() {
	InitWindow(screenWidth, screenHeight, "Mapmaker");
	SetTargetFPS(60);

	// Setup camera
	Camera2D camera = {0};
	camera.target = (Vector2){0, 0};
	camera.offset = (Vector2){(float)screenWidth / 2, (float)screenHeight / 2};
	camera.zoom = 1.0f;
	
	Sector *MapSectors = malloc(0 * sizeof(Sector));

	while(!WindowShouldClose()) {
		// Get mouse position in the WORLD's perspective(not screen's perspective)
		Vector2 mousePosInWorld = GetScreenToWorld2D(GetMousePosition(), camera);


		BeginDrawing();
		ClearBackground(WHITE);

		// Draw it in camera's perspective
		BeginMode2D(camera);
        
		DrawCircleV(mousePosInWorld, 20, BLACK);
        
		EndMode2D();

		EndDrawing();
	}
}
