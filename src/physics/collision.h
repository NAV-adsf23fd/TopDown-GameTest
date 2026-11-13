#pragma once

#include "src/blocks/block.h"
#include "src/core/manager.h"
#include "src/entities/entity.h"
#include "src/world/map.h"
#include <set>
#include <variant>
#include <vector>

using objPointer = std::variant<Entity *, Block *>;

struct CollObj {
  float x, y, z;
  objPointer address;
};

class CollisionManager {
  MAKE_SINGLETON(CollisionManager);

private:
  std::vector<CollObj> entityObjects;
  std::set<Chunk *> neighborChunks;
  std::vector<BlockType> neighBlocks;

public:
  void Handle();
  void AddObj(CollObj item);
  void RemoveObj(objPointer address);
};
