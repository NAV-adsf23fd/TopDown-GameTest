#pragma once
#include "lib/type/vector3.h"
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

class Block : public Object {
protected:
  uint8_t width, height, depth; // i use int because i dont like float for size
  Texture2D spriteSheet;
  std::string name;
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

class BlockManager {
  MAKE_SINGLETON(BlockManager);

private:
  std::unordered_map<BlockType, std::unique_ptr<Block>> registry;

public:
  Block &GetBlock(BlockType type) const;
};
