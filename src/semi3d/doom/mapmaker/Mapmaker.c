#include <raylib.h>

#define screenWidth 800
#define screenHeight 400

void CreateSector(Vector2 newSectorsPos) {
	// You know what's here
}

int main() {
	InitWindow(screenWidth, screenHeight, "Mapmaker");
	SetTargetFPS(60);

	// Setup camera
	Camera2D camera = {0};
	camera.target = (Vector2){0, 0};
	camera.offset = (Vector2){(float)screenWidth / 2, (float)screenHeight / 2};
	camera.zoom = 1.0f;

	while(!WindowShouldClose()) {
		// Get mouse position in the WORLD's perspective(not screen's perspective)
		Vector2 mousePosInWorld = GetScreenToWorld2D(GetMousePosition(), camera);

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
			CreateSector(mousePosInWorld);
		}

		BeginDrawing();
		ClearBackground(WHITE);

		// Draw it in camera's perspective
		BeginMode2D(camera);
        
		DrawCircleV(mousePosInWorld, 20, BLACK);
        
		EndMode2D();

		EndDrawing();
	}
}
