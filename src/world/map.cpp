#include "map.h"
#include "src/blocks/block.h"
#include "src/functionality/render.h"
#include "src/utils/logger.h"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

Chunk::Chunk(uint8_t x, uint8_t y) : x(x), y(y) {
  std::fill_n(position, 4096, BlockType::None);
}

BlockType Chunk::GetBlockAt(uint8_t x, uint8_t y, uint8_t z) {
  return position[x + y * 16 + z * 256];
}

std::vector<BlockType> Chunk::GetBlockList() {
  return std::vector<BlockType>(position, position + 4096);
}

void Chunk::SetBlockAt(uint8_t x, uint8_t y, uint8_t z, BlockType block) {
  position[x + y * 16 + z * 256] = block;
  RenderManager::GetInstance().AddItem(
      RenderItem{(float)x, (float)y, (float)z,
                 &(BlockManager::GetInstance().GetBlock(block))});
  Logger::qlog(Logger::Level::INFO, "Set block at block_coord(",
               std::to_string(x), ",", std::to_string(y), ",",
               std::to_string(z), ")");
}

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {}

ChunkManager &ChunkManager::GetInstance() {
  static ChunkManager instance;
  return instance;
}

Chunk *ChunkManager::GetChunkAt(uint16_t x, uint16_t y) {
  return chunks[x / 16 + 16 * y / 16];
}

void ChunkManager::SetBlockAt(uint8_t x, uint8_t y, uint8_t z,
                              BlockType block) {
  uint16_t index = x / 16 + y / 16 * 16;
  Chunk *chunk = chunks[index];
  if (chunk == nullptr) { // clang-format off
    Logger::qlog(Logger::Level::ERROR, "Didn't load chunk at block_coord(",std::to_string(x),",",std::to_string(y),")"); // clang-format on
  } else
    chunk->SetBlockAt(x - chunk->x, y - chunk->y, z, block);
}

void ChunkManager::LoadChunks(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  for (uint8_t i = x1; i <= x2; ++i)
    for (uint8_t j = y1; j <= y2; ++j) {
      uint16_t index = i + j * 16;
      if (chunks[index] == nullptr) {
        chunks[index] = new Chunk(i, j);
        Logger::qlog(Logger::Level::INFO, "Loaded chunk at chunk_coord(",
                     std::to_string(i), ",", std::to_string(j), ")");
      }
    }
}

void ChunkManager::UnloadChunks(uint8_t x1, uint8_t y1, uint8_t x2,
                                uint8_t y2) {
  for (uint8_t i = x1; i <= x2; ++i)
    for (uint8_t j = y1; j <= y2; ++j) {
      uint16_t index = i + j * 16;
      if (chunks[index] != nullptr) {
        delete chunks[index];
        chunks[index] = nullptr;
      }
    }
}
