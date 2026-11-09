#pragma once

#include "src/blocks/block.h"
#include "src/core/manager.h"
#include "src/entities/entity.h"
#include <variant>
#include <vector>

using AddressPointer = std::variant<Entity *, Block *>;

struct RenderItem {
  float x, y, z;
  AddressPointer address;
};

class RenderManager {
  MAKE_SINGLETON(RenderManager);

private:
  std::vector<RenderItem> renderList;

public:
  void AddItem(RenderItem &&item);
  void RemoveItem(AddressPointer address);
  void Render();
};
