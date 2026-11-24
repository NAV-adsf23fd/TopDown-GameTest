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

  CollisionManager.Handle();
}

void Game::Draw() {
  RenderManager.Render();
  DrawVector(ScreenCoordinateOf(player->GetPosition()), player->GetAngle(), 10,
             RED);
  // DrawRectangleLines(-127 * 16, -127 * 16, 256 * 16, 256 * 16, RED);
  for (int i = -128; i <= 127; i += 16)
    for (int j = -128; j <= 127; j += 16) {
      DrawRectangleLines(i * 16, j * 16, 16 * 16, 16 * 16, GRAY);
    }

  DrawVector({-100, 0}, 0.0f, 200, BLACK);
  DrawVector({0, -100}, 90.0f, 200, BLACK);

  RenderManager.Draw();
}

void Game::DrawDebug() const { player->DrawDebug(); }

Game::Game() {
  ChunkManager.LoadChunks(0, 0, 0, 0);
  player = std::make_unique<Player>(Vector3{1, 1, 0});
  // ChunkManager.SetBlockAt(0, 0, 0, BlockType::Error);
  // ChunkManager.SetBlockAt(0, 1, 0, BlockType::Error);
  // ChunkManager.SetBlockAt(1, 1, 0, BlockType::Error);
  // ChunkManager.SetBlockAt(1, 1, 1, BlockType::Test);
  // ChunkManager.SetBlockAt(1, 0, 0, BlockType::Error);

  PlayerCamera::Init(ScreenCoordinateOf(player->GetPosition()));
}

void Game::StartWorker() { RenderManager.StartChunkWorker(*player); }

void Game::StopWorker() { RenderManager.StopChunkWorker(); }
