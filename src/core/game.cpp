#include "game.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/entities/player.h"
#include "src/functionality/camera.h"
#include "src/functionality/render.h"
#include "src/physics/collision.h"
#include "src/utils/utility.h"
#include "src/world/map.h"
#include <memory>

bool operator!=(const Vector3 &lhs, const Vector3 &rhs) {
  return (lhs.x != rhs.x || lhs.y != rhs.y);
}

void Game::Update() {
  Direction flags = Direction::NONE;
  if (IsKeyDown(KEY_W))
    flags |= Direction::U;
  if (IsKeyDown(KEY_S))
    flags |= Direction::D;
  if (IsKeyDown(KEY_A))
    flags |= Direction::L;
  if (IsKeyDown(KEY_D))
    flags |= Direction::R;

  player->SetDirection(flags);
  player->Update();

  PlayerCamera::CameraUpdate(ScreenCoordinateOf(player->GetPosition()));

  CollisionManager::GetInstance().Handle();
}

void Game::Draw() {
  RenderManager::GetInstance().Render();
  DrawVector(ScreenCoordinateOf(player->GetPosition()), player->GetAngle(), 10,
             RED);
  DrawVector({-100, 0}, 0.0f, 200, BLACK);
  DrawVector({0, -100}, 90.0f, 200, BLACK);
}

void Game::DrawDebug() const { player->DrawDebug(); }

Game::Game() {
  ChunkManager::GetInstance().LoadChunks(0, 0, 0, 0);
  player = std::make_unique<Player>(Vector3{1, 1, 0});
  ChunkManager::GetInstance().SetBlockAt(0, 0, 0, BlockType::Error);
  // ChunkManager::GetInstance().SetBlockAt(0, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 1, BlockType::Test);
  ChunkManager::GetInstance().SetBlockAt(1, 0, 0, BlockType::Error);

  PlayerCamera::Init(ScreenCoordinateOf(player->GetPosition()));
}

void Game::StartWorker() {
  RenderManager::GetInstance().StartChunkWorker(*player);
}

void Game::StopWorker() { RenderManager::GetInstance().StopChunkWorker(); }
