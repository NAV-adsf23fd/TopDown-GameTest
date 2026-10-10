#include "map.h"
#include "block.h"
#include <cstdint>
#include <iostream>
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

// TODO: optimize this
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

Chunk *ChunkManager::GetChunkAt(uint16_t x, uint16_t y) {
  return chunks[x / 16 + 16 * y / 16];
}

// TODO: Làm hệ thống log đầy đủ
void ChunkManager::SetBlockAt(uint8_t x, uint8_t y, uint8_t z,
                              BlockType block) {
  uint16_t index = x + 16 * y;
  if (chunks[index] == nullptr) {
    std::cout << "ERROR: Didn't load chunk at (" << +x << ", " << +y << ")\n";
  } else
    chunks[index]->SetBlockAt(x, y, z, block);
}

void ChunkManager::LoadChunks(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  for (uint8_t i = x1; i <= x2; ++i)
    for (uint8_t j = y1; j <= y2; ++j) {
      uint16_t index = i + j * 16;
      if (chunks[index] == nullptr) {
        chunks[index] = new Chunk(i, j);
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

void ChunkManager::DrawLoadedChunks(uint8_t x1, uint8_t y1, uint8_t x2,
                                    uint8_t y2) {

  for (uint8_t i = x1; i <= x2; ++i)
    for (uint8_t j = y1; j <= y2; ++j) {
      uint16_t index = i + 16 * j;
      if (chunks[index] != nullptr)
        chunks[i + j * 16]->Draw();
    }
}
