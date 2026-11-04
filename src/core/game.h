#pragma once
#include "src/entities/player.h"

class Game {
private:
  Player player;

public:
  Game();
  ~Game() = default;
  void Update();
  void Draw();
  void DrawDebug() const;
};
