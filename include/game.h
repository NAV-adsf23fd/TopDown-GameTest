#pragma once
#include "player.h"

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
