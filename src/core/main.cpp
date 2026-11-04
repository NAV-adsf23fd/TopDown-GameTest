#include "config.h"
#include "game.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <raylib.h>

int main(int argc, char *argv[]) {
  RunConfig config = parseArgs(argc, argv);

  Logger::init(config.to_file, config.log_file, config.verbose_flag);

  Logger::log("Logger initialized");
  Logger::log(config.toString(), Logger::Level::DEBUG);

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

  Logger::log("Closed window");
  Logger::log("Logger closing . . .");
  Logger::close();

  return 0;
}
