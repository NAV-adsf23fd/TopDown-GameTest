#include "game.h"
#include "block.h"
#include "map.h"
#include "raylib.h"
#include "utility.h"
#include <cstdint>

bool operator!=(const Vector3 &lhs, const Vector3 &rhs) {
  return (lhs.x != rhs.x || lhs.y != rhs.y);
}

void Game::Update() {
  uint8_t flags = Direction::NONE;
  if (IsKeyDown(KEY_W))
    flags |= Direction::U;
  if (IsKeyDown(KEY_S))
    flags |= Direction::D;
  if (IsKeyDown(KEY_A))
    flags |= Direction::L;
  if (IsKeyDown(KEY_D))
    flags |= Direction::R;

  player.SetDirection(flags);

  Vector3 oldPosition = player.GetPosition();

  player.Update();

  // TODO: hiện đang dùng nhiều uint8_t để test, càn cập nhật lại vì vị trí có
  // thể âm
  Vector3 newPosition = player.GetPosition();
  if (oldPosition != newPosition) {
    uint8_t px = static_cast<uint8_t>(newPosition.x) / 16;
    uint8_t py = static_cast<uint8_t>(newPosition.y) / 16;
    uint8_t px_o = static_cast<uint8_t>(oldPosition.x) / 16;
    uint8_t py_o = static_cast<uint8_t>(oldPosition.y) / 16;
    ChunkManager::GetInstance().LoadChunks(px - 1, py - 1, px + 1, py + 1);

    if ((px != px_o) || (py != py_o))
      ChunkManager::GetInstance().UnloadChunks(px_o - 1, py_o - 1, px_o + 1,
                                               py_o + 1);
  }
}

void Game::Draw() {
  player.Draw();
  ChunkManager::GetInstance().DrawLoadedChunks(0, 0, 1, 1);
}

void Game::DrawDebug() const { player.DrawDebug(); }

Game::Game() : player({1, 1, 0}) {
  ChunkManager::GetInstance().LoadChunks(0, 0, 1, 1);
  ChunkManager::GetInstance().SetBlockAt(0, 0, 0, BlockType::Error);
  // ChunkManager::GetInstance().SetBlockAt(0, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 1, BlockType::Dirt);
  // ChunkManager::GetInstance().SetBlockAt(1, 0, 0, BlockType::Error);
}
