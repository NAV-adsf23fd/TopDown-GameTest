#include "game.h"
#include "block.h"
#include "map.h"
#include "utility.h"

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

  player.Update();
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
