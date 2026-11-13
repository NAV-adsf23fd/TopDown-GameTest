#pragma once
#include "src/entities/player.h"
#include <memory>

class Game {
private:
  std::unique_ptr<Player> player;

public:
  Game();
  ~Game() = default;
  void Update();
  void Draw();
  void DrawDebug() const;
  void StartWorker();
  void StopWorker();
};
