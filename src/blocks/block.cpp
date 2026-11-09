#include "block.h"
#include "blocks_list.h"
#include "src/resources/resource.h"
#include "src/utils/utility.h"
#include <cstdint>
#include <memory>

Block::Block(const std::string &filePath, const uint8_t width,
             const uint8_t height, const uint8_t depth, Direction direction,
             const bool hasTileEntity, const bool isSolid)
    : width(width), height(height), depth(depth), hasTileEntity(hasTileEntity),
      isSolid(isSolid), direction(direction) {
  spriteSheet = TexRsManager::GetInstance().GetTexture(filePath);
}

BlockManager::BlockManager() {
  registry[BlockType::Error] =
      std::make_unique<SolidBlock>("assets/textures/ErrorBlock.png");
  registry[BlockType::Dirt] =
      std::make_unique<SolidBlock>("assets/textures/Testblock.png");
}
BlockManager::~BlockManager() {}

Block &BlockManager::GetBlock(BlockType type) const {
  auto it = registry.find(type);
  if (it != registry.end())
    return *(it->second);
  return *(registry.at(BlockType::Error));
}
