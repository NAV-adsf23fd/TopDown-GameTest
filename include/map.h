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

  void DrawLoadedChunks(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
  // Why not put the drawing code inside LoadChunks()?
  // > Because I don't want to run unnecessary chunk-loading
  // code  just to handle the drawing. So, I separated them
  // so that Draw() can run continuously, while LoadChunks()
  // runs only occasionally.
};
