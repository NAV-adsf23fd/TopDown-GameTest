#include "collision.h"
#include "lib/type/vector3.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/world/map.h"
#include <algorithm>
#include <cstdint>
#include <variant>

CollisionManagerSingleton::CollisionManagerSingleton() {};
CollisionManagerSingleton::~CollisionManagerSingleton() {};

void CollisionManagerSingleton::AddObj(CollObj item) {
  entityObjects.push_back(item);
}

void CollisionManagerSingleton::RemoveObj(objPointer address) {
  auto it = std::find_if(
      entityObjects.begin(), entityObjects.end(),
      [&address](const CollObj &item) { return item.address == address; });

  if (it != entityObjects.end()) {
    std::swap(*it, entityObjects.back());
    entityObjects.pop_back();
  }
}

void CollisionManagerSingleton::Handle() {
  for (auto obj : entityObjects) { // clang-format off
    std::visit([&obj, this](auto &&arg) { // clang-format on
          if constexpr (requires { arg->GetPosition(); }) {
            Vector3 pos = arg->GetPosition();
            neighBlocks.clear();
            arg->ClearCollider();

            // TODO: optimize this
            for (int8_t i = -1; i <= 1; ++i) {
              for (int8_t j = -1; j <= 1; ++j) {
                for (int8_t k = -1; k <= 1; ++k) {
                  BlockType blocktype =
                      ChunkManager.GetBlockAt(pos.x + i, pos.y + j, pos.z + k);
                  if (blocktype != BlockType::Error) {
                    Block &block = BlockManager.GetBlock(blocktype);
                    Vector3ui8 size = block.GetSize();
                    arg->AddCollider(Box3{pos.x + i, pos.x + i + size.x,
                                          pos.y + j, pos.y + j + size.y,
                                          pos.z + k, pos.z + k + size.z});
                  }
                }
              }
            }
          } else {
          }
        },
        obj.address);

    // neighborChunks.insert(
    //     ChunkManager.GetChunkAt(pos.x - 1, pos.y));
    // neighborChunks.insert(
    //     ChunkManager.GetChunkAt(pos.x + 1, pos.y));
    // neighborChunks.insert(
    //     ChunkManager.GetChunkAt(pos.x, pos.y - 1));
    // neighborChunks.insert(
    //     ChunkManager.GetChunkAt(pos.x, pos.y + 1));

    // neighBlocks.push_back(ChunkManager.GetBlockAt(
    //     pos.x + i, pos.y + j, pos.z + k));
  }
}
