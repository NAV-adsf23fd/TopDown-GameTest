#include "game.h"
#include "utility.h"
#include <raylib.h>

int main() {
  InitWindow(800, 600, "topdown");
  SetTargetFPS(60);
  Game game;

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();

    ClearBackground(RAYWHITE);
    game.Update();
    game.Draw();
    game.DrawDebug();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
