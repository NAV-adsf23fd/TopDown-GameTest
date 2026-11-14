#pragma once
#include "src/blocks/block.h"
#include "src/core/manager.h"
#include "src/utils/utility.h"
#include <ostream>
#include <raylib.h>
#include <shared_mutex>
#include <sstream>
#include <string>

class Chunk {
private:
  BlockType position[BLOCK_IN_CHUNK];

public:
  c_int x, y;
  Chunk(c_int x, c_int y);
  ~Chunk() = default;

  void SetBlockAt(bc_int x, bc_int y, bc_int z, BlockType block);
  BlockType GetBlockAt(bc_int x, bc_int y, bc_int z);
};

class ChunkManager {
  MAKE_SINGLETON(ChunkManager);

private:
  Chunk *chunks[CHUNK_IN_WORLD] = {nullptr};
  std::shared_mutex chunksMutex;

public:
  Chunk *GetChunk(c_int x, c_int y);
  Chunk *GetChunkAt(b_int x, b_int y);

  void SetBlockAt(b_int x, b_int y, b_int z, BlockType block);
  BlockType GetBlockAt(b_int x, b_int y, b_int z);

  void LoadChunks(c_int x1, c_int y1, c_int x2, c_int y2);
  void LoadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2);

  void UnloadChunks(c_int x1, c_int y1, c_int x2, c_int y2);
  void UnloadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2);

  bool IsChunkLoaded(c_int x, c_int y);
  bool IsChunkLoadedAt(b_int x, b_int y);
};

inline std::ostream &operator<<(std::ostream &out, Chunk *chunk) {
  if (chunk == nullptr)
    out << "nullptr";
  else
    out << "Chunk(" << std::to_string(chunk->x) << ", "
        << std::to_string(chunk->y) << ")";
  return out;
}
