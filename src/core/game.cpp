#include "game.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/entities/player.h"
#include "src/functionality/camera.h"
#include "src/functionality/render.h"
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

  Vector3 oldPosition = player->GetPosition();

  player->Update();

  // TODO: hiện đang dùng nhiều uint8_t để test, càn cập nhật lại vì vị trí có
  // thể âm
  Vector3 newPosition = player->GetPosition();

  // if (oldPosition != newPosition) {
  //   int8_t px = static_cast<int8_t>(newPosition.x) / 16;
  //   int8_t py = static_cast<int8_t>(newPosition.y) / 16;
  //   int8_t px_o = static_cast<int8_t>(oldPosition.x) / 16;
  //   int8_t py_o = static_cast<int8_t>(oldPosition.y) / 16;
  //   ChunkManager::GetInstance().LoadChunks(px - 1, py - 1, px + 1, py + 1);
  //
  //   if ((px != px_o) || (py != py_o))
  //     ChunkManager::GetInstance().UnloadChunks(px_o - 1, py_o - 1, px_o + 1,
  //                                              py_o + 1);
  // }

  PlayerCamera::CameraUpdate(ScreenCoordinateOf(player->GetPosition()));
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
  player = std::make_unique<Player>(Vector3{1, 1, 0});
  ChunkManager::GetInstance().LoadChunks(0, 0, 0, 0);
  ChunkManager::GetInstance().SetBlockAt(0, 0, 0, BlockType::Error);
  // ChunkManager::GetInstance().SetBlockAt(0, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 0, BlockType::Error);
  ChunkManager::GetInstance().SetBlockAt(1, 1, 1, BlockType::Dirt);
  ChunkManager::GetInstance().SetBlockAt(1, 0, 0, BlockType::Error);

  PlayerCamera::Init(ScreenCoordinateOf(player->GetPosition()));
}
