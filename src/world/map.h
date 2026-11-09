#pragma once
#include "src/blocks/block.h"
#include <cstdint>
#include <raylib.h>
#include <vector>

class Chunk {
private:
  BlockType position[16 * 16 * 16];

public:
  uint8_t x, y;
  Chunk(uint8_t x, uint8_t y);
  ~Chunk() = default;

  void SetBlockAt(uint8_t x, uint8_t y, uint8_t z, BlockType block);
  BlockType GetBlockAt(uint8_t x, uint8_t y, uint8_t z);
  std::vector<BlockType> GetBlockList();
};

class ChunkManager { // Map
private:
  Chunk *chunks[256] = {nullptr};

  ChunkManager();
  ~ChunkManager();

  ChunkManager(const ChunkManager &) = delete;
  ChunkManager &operator=(const ChunkManager &) = delete;

public:
  static ChunkManager &GetInstance();

  Chunk *GetChunkAt(uint16_t x, uint16_t y);
  void SetBlockAt(uint8_t x, uint8_t y, uint8_t z, BlockType block);

  void LoadChunks(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
  void UnloadChunks(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
};
