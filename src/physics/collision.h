#pragma once

#include "src/blocks/block.h"
#include "src/core/manager.h"
#include "src/entities/entity.h"
#include <variant>
#include <vector>

using variantObj = std::variant<Entity *, Block *>;

class CollisionManager {
  MAKE_SINGLETON(CollisionManager);

private:
  std::vector<variantObj> entityObjects;

public:
};
