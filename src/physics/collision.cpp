#include "collision.h"
#include "lib/type/vector3.h"
#include "raylib.h"
#include "src/blocks/block.h"
#include "src/utils/logger.h"
#include "src/world/map.h"
#include <algorithm>
#include <cmath>
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

            Vector3i8 intpos = {std::floor(pos.x), std::floor(pos.y),
                                std::floor(pos.z)};
            for (int8_t i = -1; i <= 2; ++i) {
              for (int8_t j = -1; j <= 2; ++j) {
                for (int8_t k = -1; k <= 2; ++k) {
                  BlockType blocktype = ChunkManager.GetBlockAt(
                      intpos.x + i, intpos.y + j, intpos.z + k);
                  // Log("Blocktype is {}", blocktype);
                  if (blocktype != BlockType::None) {
                    Block &block = BlockManager.GetBlock(blocktype);
                    Vector3ui8 size = block.GetSize();
                    // Log("This block's size is ({}, {}, {})", size.x, size.y,
                    //     size.z);
                    arg->AddCollider(Box3{intpos.x + i, intpos.x + i + size.x,
                                          intpos.y + j, intpos.y + j + size.y,
                                          intpos.z + k, intpos.z + k + size.z});
                    Log("Added collider ({}+{}, {}+{}+{}, {}+{}, {}+{}+{}, "
                        "{}+{}, {}+{}+{}",
                        intpos.x, i, intpos.x, i, size.x, intpos.y, j, intpos.y,
                        j, size.y, intpos.z, k, intpos.z, k, size.z);
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
