#pragma once
#include "map.h"
#include "player.h"

class Game {
private:
  Player player;
  Chunk chunk1;

public:
  Game();
  ~Game() = default;
  void Update();
  void Draw();
  void DrawDebug() const;
};
