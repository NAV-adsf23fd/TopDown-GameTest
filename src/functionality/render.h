#pragma once

#include "src/blocks/block.h"
#include "src/entities/entity.h"
#include <variant>
#include <vector>

using AddressPointer = std::variant<Entity *, Block *>;

struct RenderItem {
  float x, y, z;
  AddressPointer address;
};

class RenderManager {
private:
  std::vector<RenderItem> renderList;
  RenderManager();
  ~RenderManager();

  RenderManager(const RenderManager &) = delete;
  RenderManager &operator=(const RenderManager &) = delete;

public:
  static RenderManager &GetInstance();
  void AddItem(RenderItem &&item);
  void RemoveItem(AddressPointer address);
  void Render();
};
