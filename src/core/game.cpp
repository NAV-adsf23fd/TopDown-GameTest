#include "game.h"
#include "lib/type/vector3.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/entities/player.h"
#include "src/functionality/camera.h"
#include "src/functionality/render.h"
#include "src/physics/collision.h"
#include "src/utils/logger.h"
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
  Vector2 playerpos = ScreenCoordinateOf(player->GetPosition());
  Vector3i8 playersize = player->GetSize();
  Log("Playerpos = ({}, {})", playerpos.x, playerpos.y);
  Log("Playersize = ({}, {}, {})", playersize.x, playersize.y, playersize.z);

  DrawVector(playerpos, player->GetAngle(), 10, RED);
  // DrawPixel(playerpos.x, playerpos.y, RED);
  DrawRectangleLinesEx(
      {playerpos.x, playerpos.y, playersize.x * 16, playersize.y * 16}, 1, RED);
  DrawRectangleLinesEx({playerpos.x, (playerpos.y - 8 * playersize.z),
                        playersize.x * 16, playersize.y * 16},
                       1, BLUE);

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
  ChunkManager.SetBlockAt(0, 0, 0, BlockType::Error);
  ChunkManager.SetBlockAt(0, 1, 0, BlockType::Error);
  ChunkManager.SetBlockAt(1, 1, 0, BlockType::Error);
  ChunkManager.SetBlockAt(1, 1, 1, BlockType::Test);
  ChunkManager.SetBlockAt(1, 0, 0, BlockType::Error);
  ChunkManager.SetBlockAt(2, 1, 2, BlockType::Test);
  ChunkManager.SetBlockAt(3, 1, 3, BlockType::Test);
  ChunkManager.SetBlockAt(4, 1, 4, BlockType::Test);
  ChunkManager.SetBlockAt(2, 1, 1, BlockType::Test);

  PlayerCamera::Init(ScreenCoordinateOf(player->GetPosition()));
}

void Game::StartWorker() { RenderManager.StartChunkWorker(*player); }

void Game::StopWorker() { RenderManager.StopChunkWorker(); }
