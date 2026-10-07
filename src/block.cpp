#include "block.h"
#include "blocks_list.h"
#include "resource.h"
#include <cstdint>
#include <memory>

Block::Block(const uint8_t width, const uint8_t height, const uint8_t depth,
             const bool hasTileEntity, const bool isSolid, uint8_t direction,
             const std::string &filePath)
    : width(width), height(height), depth(depth), hasTileEntity(hasTileEntity),
      isSolid(isSolid), direction(direction) {
  spriteSheet = TexRsManager::GetInstance().GetTexture(filePath);
}

BlockManager::BlockManager() {
  registry[BlockType::Error] = std::make_unique<SolidBlock>(
      1, 1, 1, "ErrorBlock", false, "assets/textures/ErrorBlock.png");
  registry[BlockType::Dirt] = std::make_unique<SolidBlock>(
      1, 1, 1, "SolidBlock", false, "assets/textures/Testblock.png");
}
BlockManager::~BlockManager() {}

BlockManager &BlockManager::GetInstance() {
  static BlockManager instance;
  return instance;
}

Block &BlockManager::GetBlock(BlockType type) const {
  auto it = registry.find(type);
  if (it != registry.end())
    return *(it->second);
  return *(registry.at(BlockType::Error));
}
