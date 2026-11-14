#include "render.h"
#include "raylib.h"
#include "src/entities/player.h"
#include "src/world/map.h"
#include <algorithm>
#include <cstdint>
#include <string>
#include <thread>
#include <variant>

RenderManager::RenderManager() {}
RenderManager::~RenderManager() {}

void RenderManager::AddItem(RenderItem &&item, const std::string &name) {
  if (ChunkManager::GetInstance().GetChunkAt(item.x, item.y) != nullptr) {
    renderList.push_back(item);
  }
}

void RenderManager::RemoveItem(AddressPointer address) {
  auto it = std::find_if(
      renderList.begin(), renderList.end(),
      [&address](const RenderItem &item) { return item.address == address; });

  if (it != renderList.end()) {
    std::swap(*it, renderList.back());
    renderList.pop_back();
  }
}

void RenderManager::Render() {
  std::sort(renderList.begin(), renderList.end(),
            [](const RenderItem &a, const RenderItem &b) {
              Vector3 pa = std::visit(
                  [&a](auto &&arg) {
                    if constexpr (requires { arg->GetPosition(); }) {
                      return arg->GetPosition();
                    } else
                      return Vector3{a.x, a.y, a.z};
                  },
                  a.address);
              Vector3 pb = std::visit(
                  [&b](auto &&arg) {
                    if constexpr (requires { arg->GetPosition(); }) {
                      return arg->GetPosition();
                    } else
                      return Vector3{b.x, b.y, b.z};
                  },
                  b.address);
              if (std::abs(pa.z - pb.z) <= 0.001f) {
                return pa.y < pb.y;
              } else
                return pa.z < pb.z;
            });
  for (auto item : renderList) {
    std::visit([&item](auto &&arg) { arg->Draw(item.x, item.y, item.z); },
               item.address);
  }
}

void RenderManager::StartChunkWorker(Player &player) {
  if (isRunning)
    return; // prevent duplicated init

  Vector3 pos = player.GetPosition();
  uint8_t currentChunkX = static_cast<uint8_t>(pos.x) / 16;
  uint8_t currentChunkY = static_cast<uint8_t>(pos.y) / 16;
  ChunkManager::GetInstance().LoadChunks(currentChunkX, currentChunkY,
                                         currentChunkX, currentChunkY);
  loadedChunks[currentChunkX + currentChunkY * 16] =
      ChunkManager::GetInstance().GetChunk(currentChunkX, currentChunkY);

  isRunning = true;
  chunkWorker =
      std::thread(&RenderManager::ChunkWorkerLoop, this, std::ref(player));
}

void RenderManager::StopChunkWorker() {
  isRunning = false;
  if (chunkWorker.joinable())
    chunkWorker.join();
}

void RenderManager::ChunkWorkerLoop(Player &player) {
  while (isRunning) {

    Vector3 pos =
        player.GetPosition(); // can de y vi co the thread race, them mutex
    uint8_t currentChunkX = static_cast<uint8_t>(pos.x) / 16;
    uint8_t currentChunkY = static_cast<uint8_t>(pos.y) / 16;

    for (auto it = loadedChunks.begin(); it != loadedChunks.end();) {
      uint8_t dx = std::abs(it->second->x - currentChunkX);
      uint8_t dy = std::abs(it->second->y - currentChunkY);

      if (dx > 1 || dy > 1) {
        uint8_t cx = it->second->x;
        uint8_t cy = it->second->y;

        it = loadedChunks.erase(it);
        ChunkManager::GetInstance().UnloadChunks(cx, cy, cx, cy);
      } else {
        ++it;
      }
    }

    for (int x = currentChunkX - 1; x <= currentChunkX + 1; ++x) {
      for (int y = currentChunkY - 1; y <= currentChunkY + 1; ++y) {
        if (!ChunkManager::GetInstance().IsChunkLoaded(x, y)) {
          ChunkManager::GetInstance().LoadChunks(x, y, x, y);
          loadedChunks[x + 16 * y] = ChunkManager::GetInstance().GetChunk(x, y);
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}
