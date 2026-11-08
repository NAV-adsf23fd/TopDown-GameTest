#include "render.h"
#include "raylib.h"
#include <algorithm>
#include <variant>

RenderManager::RenderManager() {}
RenderManager::~RenderManager() {}

RenderManager &RenderManager::GetInstance() {
  static RenderManager instance;
  return instance;
}

void RenderManager::AddItem(RenderItem &&item) { renderList.push_back(item); }

void RenderManager::RemoveItem(AddressPointer address) {
  auto it = std::find_if(
      renderList.begin(), renderList.end(),
      [address](const RenderItem &item) { return item.address == address; });

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
    std::visit(
        [&item](auto &&arg) { return arg->Draw(item.x, item.y, item.z); },
        item.address);
  }
}
