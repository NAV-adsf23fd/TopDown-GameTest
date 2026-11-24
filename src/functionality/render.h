#pragma once

#include "src/blocks/block.h"
#include "src/core/manager.h"
#include "src/entities/entity.h"
#include "src/entities/player.h"
#include "src/utils/utility.h"
#include "src/world/map.h"
#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

using AddressPointer = std::variant<Entity *, Block *>;

struct RenderItem {
  float x, y, z;
  AddressPointer address;
};

class RenderManagerSingleton {
  MAKE_SINGLETON(RenderManagerSingleton);

private:
  std::vector<RenderItem> renderList;
  // std::vector<Chunk *> loadedChunks;
  std::unordered_map<ci_int, std::shared_ptr<Chunk>> loadedChunks;
  std::shared_mutex loadedChunksMutex;
  std::thread chunkWorker;
  std::atomic<bool> isRunning{false};

public:
  void AddItem(RenderItem &&item, const std::string &name = "obj");
  void RemoveItem(AddressPointer address);
  void Render();
  void ChunkWorkerLoop(Player &player);
  void StartChunkWorker(Player &player);
  void StopChunkWorker();
  void Draw();
};
