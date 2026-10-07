#include "map.h"
#include "block.h"
#include <cstdint>
#include <sys/types.h>

Chunk::Chunk(uint8_t x, uint8_t y) : x(x), y(y) {
  std::fill_n(position, 4096, BlockType::None);
}

BlockType Chunk::GetBlockAt(uint8_t x, uint8_t y, uint8_t z) {
  return position[x + y * 16 + z * 256];
}

void Chunk::SetBlockAt(uint8_t x, uint8_t y, uint8_t z, BlockType block) {
  position[x + y * 16 + z * 256] = block;
}

void Chunk::Draw() {
  for (uint8_t i_x = 0; i_x < 16; ++i_x)
    for (uint8_t i_y = 0; i_y < 16; ++i_y)
      for (uint8_t i_z = 0; i_z < 16; ++i_z) {
        if (GetBlockAt(i_x, i_y, i_z) != BlockType::None)
          BlockManager::GetInstance()
              .GetBlock(GetBlockAt(i_x, i_y, i_z))
              .Draw(x + i_x, y + i_y, i_z);
      }
}

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {}

ChunkManager &ChunkManager::GetInstance() {
  static ChunkManager instance;
  return instance;
}

Chunk *ChunkManager::GetChunkAt(uint8_t x, uint8_t y) {
  return chunks[x + 16 * y];
}
