#include "config.h"
#include "game.h"
#include "src/functionality/camera.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <raylib.h>

int main(int argc, char *argv[]) {
  RunConfig config = parseArgs(argc, argv);

  Logger::init(config.to_file, config.log_file, config.verbose_flag);

  Logger::log("Logger initialized");
  Logger::log(config.toString(), Logger::Level::DEBUG);

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

  Logger::log("Closed window");
  Logger::log("Logger closing . . .");
  Logger::close();

  return 0;
}
