#pragma once
#include "raylib.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

enum class BlockType : uint8_t {
  None,
  Error,
  Dirt,
  Stone,
  Air,
};

class Block {
protected:
  uint8_t width, height, depth; // i use int because i dont like float for size
  Texture2D spriteSheet;
  std::string name;
  bool hasTileEntity;
  bool isSolid;
  uint8_t direction;

public:
  Block(const uint8_t width, const uint8_t height, const uint8_t depth,
        const bool hasTileEntity, const bool isSolid, uint8_t direction,
        const std::string &filePath);
  virtual ~Block() = default;
  virtual void Draw(float x, float y, float z) = 0;
  virtual void OnInteract() = 0;
  virtual void OnRandomTick() = 0;
  virtual void OnNeighbourChange() = 0;
};

class BlockManager {
private:
  std::unordered_map<BlockType, std::unique_ptr<Block>> registry;

  BlockManager();
  ~BlockManager();

  BlockManager(const BlockManager &) = delete;
  BlockManager &operator=(const BlockManager &) = delete;

public:
  static BlockManager &GetInstance();
  Block &GetBlock(BlockType type) const;
};
