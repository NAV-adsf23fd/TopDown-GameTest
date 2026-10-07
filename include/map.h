#pragma once
#include "block.h"
#include <cstdint>
#include <raylib.h>

class Chunk {
private:
  uint8_t x, y;
  BlockType position[16 * 16 * 16];

public:
  Chunk(uint8_t x, uint8_t y);
  ~Chunk() = default;

  void SetBlockAt(uint8_t x, uint8_t y, uint8_t z, BlockType block);
  BlockType GetBlockAt(uint8_t x, uint8_t y, uint8_t z);
  void Draw();
};

class ChunkManager { // Map
private:
  Chunk *chunks[256] = {0};

  ChunkManager();
  ~ChunkManager();

  ChunkManager(const ChunkManager &) = delete;
  ChunkManager &operator=(const ChunkManager &) = delete;

public:
  static ChunkManager &GetInstance();

  Chunk *GetChunkAt(uint8_t x, uint8_t y);
};
