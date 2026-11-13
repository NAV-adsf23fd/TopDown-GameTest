#include "collision.h"
#include "lib/type/vector3.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/world/map.h"
#include <algorithm>
#include <cstdint>
#include <variant>

CollisionManager::CollisionManager() {};
CollisionManager::~CollisionManager() {};

void CollisionManager::AddObj(CollObj item) { entityObjects.push_back(item); }

void CollisionManager::RemoveObj(objPointer address) {
  auto it = std::find_if(
      entityObjects.begin(), entityObjects.end(),
      [&address](const CollObj &item) { return item.address == address; });

  if (it != entityObjects.end()) {
    std::swap(*it, entityObjects.back());
    entityObjects.pop_back();
  }
}

void CollisionManager::Handle() {
  for (auto obj : entityObjects) { // clang-format off
    std::visit([&obj, this](auto &&arg) { // clang-format on
          if constexpr (requires { arg->GetPosition(); }) {
            Vector3 pos = arg->GetPosition();
            neighBlocks.clear();
            arg->ClearCollider();

            for (int8_t i = -1; i <= 1; ++i) {
              for (int8_t j = -1; j <= 1; ++j) {
                for (int8_t k = -1; k <= 1; ++k) {
                  Block &block = BlockManager::GetInstance().GetBlock(
                      ChunkManager::GetInstance().GetBlockAt(
                          pos.x + i, pos.y + j, pos.z + k));
                  Vector3ui8 size = block.GetSize();
                  arg->AddCollider(Box3{obj.x, obj.x + size.x, obj.y,
                                        obj.y + size.y, obj.z, obj.z + size.z});
                }
              }
            }
          } else {
          }
        },
        obj.address);

    // neighborChunks.insert(
    //     ChunkManager::GetInstance().GetChunkAt(pos.x - 1, pos.y));
    // neighborChunks.insert(
    //     ChunkManager::GetInstance().GetChunkAt(pos.x + 1, pos.y));
    // neighborChunks.insert(
    //     ChunkManager::GetInstance().GetChunkAt(pos.x, pos.y - 1));
    // neighborChunks.insert(
    //     ChunkManager::GetInstance().GetChunkAt(pos.x, pos.y + 1));

    // neighBlocks.push_back(ChunkManager::GetInstance().GetBlockAt(
    //     pos.x + i, pos.y + j, pos.z + k));
  }
}
