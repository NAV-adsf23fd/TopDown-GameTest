#pragma once
#include "lib/type/vector3.h"
#include "quill/bundled/fmt/format.h"
#include "raylib.h"
#include "src/core/manager.h"
#include "src/physics/object.h"
#include "src/utils/utility.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

enum class BlockType : uint8_t {
  None,
  Error,
  Grass,
  Stone,
  Water,
  Test,
  Air,
};

// For quill Log
// constexpr std::string_view format_as(BlockType b) noexcept {
//   switch (b) {
//   case BlockType::None:
//     return "BlockType::None";
//   case BlockType::Error:
//     return "BlockType::Error";
//   case BlockType::Grass:
//     return "BlockType::Grass";
//   case BlockType::Stone:
//     return "BlockType::Stone";
//   case BlockType::Water:
//     return "BlockType::Water";
//   case BlockType::Test:
//     return "BlockType::Test";
//   case BlockType::Air:
//     return "BlockType::Air";
//   default:
//     return "BlockType::None";
//   }
// }

// Use this instead because of some namespace problems
template <>
struct fmtquill::formatter<BlockType> : fmtquill::formatter<std::string_view> {
  auto format(BlockType type, fmtquill::format_context &ctx) const {
    std::string_view name = "Unknown";
    switch (type) {
    case BlockType::Error:
      name = "BlockType::Error";
      break;
    case BlockType::Grass:
      name = "BlockType::Grass";
      break;
    case BlockType::Stone:
      name = "BlockType::Stone";
      break;
    case BlockType::Water:
      name = "BlockType::Water";
      break;
    case BlockType::Air:
      name = "BlockType::Water";
      break;
    case BlockType::Test:
      name = "BlockType::Water";
      break;
    case BlockType::None:
      name = "BlockType::None";
      break;
    default:
      name = "Invalid BlockType";
      break;
    }
    return fmtquill::formatter<std::string_view>::format(name, ctx);
  }
};

class Block : public Object {
protected:
  uint8_t width, height, depth; // i use int because i dont like float for size
  Texture2D spriteSheet;
  bool hasTileEntity;
  bool isSolid;
  Direction direction;

public:
  Block(const std::string &filePath, const uint8_t width, const uint8_t height,
        const uint8_t depth, Direction direction, const bool hasTileEntity,
        const bool isSolid);
  virtual ~Block() = default;
  virtual void OnInteract() = 0;
  virtual void OnRandomTick() = 0;
  virtual void OnNeighbourChange() = 0;
  virtual Vector3ui8 GetSize();
};

class BlockManagerSingleton {
  MAKE_SINGLETON(BlockManagerSingleton);

private:
  std::unordered_map<BlockType, std::unique_ptr<Block>> registry;

public:
  Block &GetBlock(BlockType type) const;
};
