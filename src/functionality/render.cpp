#include "render.h"
#include "raylib.h"
#include "src/entities/player.h"
#include "src/utils/utility.h"
#include "src/world/map.h"
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <variant>

RenderManagerSingleton::RenderManagerSingleton() {}
RenderManagerSingleton::~RenderManagerSingleton() {}

void RenderManagerSingleton::AddItem(RenderItem &&item,
                                     const std::string &name) {
  if (ChunkManager.GetChunkAt(item.x, item.y) != nullptr) {
    renderList.push_back(item);
  }
}

void RenderManagerSingleton::RemoveItem(AddressPointer address) {
  auto it = std::find_if(
      renderList.begin(), renderList.end(),
      [&address](const RenderItem &item) { return item.address == address; });

  if (it != renderList.end()) {
    std::swap(*it, renderList.back());
    renderList.pop_back();
  }
}

void RenderManagerSingleton::Render() {
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

void RenderManagerSingleton::StartChunkWorker(Player &player) {
  if (isRunning)
    return; // prevent duplicated init

  Vector3 pos = player.GetPosition();
  c_int currentChunkX = BtCcoord(static_cast<c_int>(pos.x));
  c_int currentChunkY = BtCcoord(static_cast<c_int>(pos.y));
  ChunkManager.LoadChunks(currentChunkX, currentChunkY, currentChunkX,
                          currentChunkY);
  loadedChunks[currentChunkX + 8 + (currentChunkY + 8) * 16] =
      ChunkManager.GetChunk(currentChunkX, currentChunkY);

  isRunning = true;
  chunkWorker = std::thread(&RenderManagerSingleton::ChunkWorkerLoop, this,
                            std::ref(player));
}

void RenderManagerSingleton::StopChunkWorker() {
  isRunning = false;
  if (chunkWorker.joinable())
    chunkWorker.join();
}

void RenderManagerSingleton::ChunkWorkerLoop(Player &player) {
  while (isRunning) {
    Vector3 pos = player.GetPosition();
    c_int currentChunkX = BtCcoord(static_cast<c_int>(pos.x));
    c_int currentChunkY = BtCcoord(static_cast<c_int>(pos.y));

    // TODO: optimize this

    {
      std::unique_lock<std::shared_mutex> lock(loadedChunksMutex);
      for (auto it = loadedChunks.begin(); it != loadedChunks.end();) {
        c_int dx = std::abs(it->second->x - currentChunkX);
        c_int dy = std::abs(it->second->y - currentChunkY);

        if (dx > 1 || dy > 1) {
          c_int cx = it->second->x;
          c_int cy = it->second->y;

          it = loadedChunks.erase(it);
          ChunkManager.UnloadChunks(cx, cy, cx, cy);
        } else {
          ++it;
        }
      }
    }

    {
      std::unique_lock<std::shared_mutex> lock(loadedChunksMutex);
      for (c_int x = currentChunkX - 1; x <= currentChunkX + 1; ++x) {
        for (c_int y = currentChunkY - 1; y <= currentChunkY + 1; ++y) {
          if (x < -8 || x > 7 || y < -8 || y > 7)
            continue;

          if (!ChunkManager.IsChunkLoaded(x, y)) {
            ChunkManager.LoadChunks(x, y, x, y);
            loadedChunks[x + 8 + 16 * (y + 8)] = ChunkManager.GetChunk(x, y);
          }
        }
      }
    }

    // TODO: optimize this
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

void RenderManagerSingleton::Draw() {
  std::shared_lock<std::shared_mutex> lock(loadedChunksMutex);
  for (auto it = loadedChunks.begin(); it != loadedChunks.end();) {
    DrawRectangleLines(it->second->x * 16 * 16, it->second->y * 16 * 16,
                       16 * 16, 16 * 16, YELLOW);
    // Log("drawed {}, {}, {}, {}", it->second->x, it->second->y, 16, 16);
    ++it;
  }
}
