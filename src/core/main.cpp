#include "game.h"
#include "src/functionality/camera.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <raylib.h>

int main(int argc, char *argv[]) {
  Logger::INIT_LOGGER();

  Log("Logger initialized");
  Log("Game starting");

  InitWindow(SCREENSIZE, SCREENNAME);
  SetTargetFPS(60);
  Game game;

  game.StartWorker();

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    game.Update();

    BeginDrawing();
    {
      ClearBackground(RAYWHITE);

      BeginMode2D(PlayerCamera::camera);
      {
        game.Draw();
      }
      EndMode2D();

      game.DrawDebug();
    }
    EndDrawing();
  }

  game.StopWorker();

  CloseWindow();

  Log("Closed window");

  return 0;
}
