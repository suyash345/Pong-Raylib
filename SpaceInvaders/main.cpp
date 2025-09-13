#include <raylib.h>

int main() {

	const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

	while (WindowShouldClose() == false) {
		BeginDrawing();
        ClearBackground(BLACK);

        DrawLine(SCREEN_WIDTH/2,0,SCREEN_WIDTH/2,SCREEN_HEIGHT,WHITE);
        DrawCircle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,20,WHITE);
        DrawRectangle(10,SCREEN_HEIGHT/2 - 60,25,120,WHITE);
        DrawRectangle(SCREEN_WIDTH-35, SCREEN_HEIGHT / 2 - 60, 25, 120, WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

