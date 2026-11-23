#include "map.h"
#include "src/blocks/block.h"
#include "src/functionality/render.h"
#include "src/physics/collision.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <sys/types.h>

Chunk::Chunk(c_int x, c_int y) : x(x), y(y) {
  std::fill_n(position, BLOCK_IN_CHUNK, BlockType::None);
}

BlockType Chunk::GetBlockAt(bc_int x, bc_int y, bc_int z) {
  std::shared_lock<std::shared_mutex> lock(blockMutex);
  return position[x + y * 16 + z * 16 * 16];
}

void Chunk::SetBlockAt(bc_int x, bc_int y, bc_int z, BlockType block) {
  std::unique_lock<std::shared_mutex> lock(blockMutex);
  position[x + y * 16 + z * 16 * 16] = block;
  lock.unlock();

  RenderManager::GetInstance().AddItem(
      RenderItem{(float)x, (float)y, (float)z,
                 &(BlockManager::GetInstance().GetBlock(block))});
  CollisionManager::GetInstance().AddObj(
      CollObj{(float)x, (float)y, (float)z,
              &(BlockManager::GetInstance().GetBlock(block))});
  Log("Set block at block_coord({}, {})", std::to_string(x), std::to_string(y));
}

ChunkManager::ChunkManager() {}
ChunkManager::~ChunkManager() {}

Chunk *ChunkManager::GetChunk(c_int x, c_int y) {
  std::shared_lock<std::shared_mutex> lock(chunksMutex);
  return chunks[x + y * 16].get();
}

Chunk *ChunkManager::GetChunkAt(b_int x, b_int y) {
  return GetChunk(x / 16, y / 16);
}

void ChunkManager::SetBlockAt(b_int x, b_int y, b_int z, BlockType block) {
  Chunk *chunk = GetChunkAt(x, y);
  if (chunk == nullptr) {
    LogE("Didnt load chunk at block_coord({}, {})", std::to_string(x),
         std::to_string(y));
  } else {
    chunk->SetBlockAt(x - chunk->x * 16, y - chunk->y * 16, z, block);
  }
}

BlockType ChunkManager::GetBlockAt(b_int x, b_int y, b_int z) {
  Chunk *chunk = GetChunkAt(x, y);
  if (chunk == nullptr) {
    return BlockType::None;
  } else {
    return chunk->GetBlockAt(x - chunk->x * 16, y - chunk->y * 16, z);
  }
}

void ChunkManager::LoadChunks(c_int x1, c_int y1, c_int x2, c_int y2) {
  for (c_int i = x1; i <= x2; ++i)
    for (c_int j = y1; j <= y2; ++j) {
      ci_int index = i + j * 16;
      {
        std::shared_lock<std::shared_mutex> lock(chunksMutex);
        if (chunks[index] != nullptr)
          continue;
      }

      std::unique_ptr<Chunk> newChunk = std::make_unique<Chunk>(i, j);

      {
        std::unique_lock<std::shared_mutex> lock(chunksMutex);
        if (chunks[index] == nullptr)
          chunks[index] = std::move(newChunk);
      }

      if (newChunk == nullptr)
        Log("Loaded chunk at chunk_coord({},{})", std::to_string(i),
            std::to_string(j));
    }
}

void ChunkManager::LoadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2) {
  LoadChunks(x1 / 16, y1 / 16, x2 / 16, y2 / 16);
}

void ChunkManager::UnloadChunks(c_int x1, c_int y1, c_int x2, c_int y2) {
  for (c_int i = x1; i <= x2; ++i)
    for (c_int j = y1; j <= y2; ++j) {
      ci_int index = i + j * 16;

      // Declare separate pointer into which moving chunk pointer to make heavy
      // Chunk Destructor run outside unique_lock
      std::unique_ptr<Chunk> chunkToUnload;
      {
        std::unique_lock<std::shared_mutex> lock(chunksMutex);
        if (chunks[index] != nullptr)
          chunkToUnload = std::move(chunks[index]);
      }

      if (chunkToUnload != nullptr)
        Log("Unloaded chunk at chunk_coord({},{})", std::to_string(i),
            std::to_string(j));
      // chunkToUnload will call Chunk's Destructor automatically when it exits
      // this scope
    }
}

void ChunkManager::UnloadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2) {
  UnloadChunks(x1 / 16, y1 / 16, x2 / 16, y2 / 16);
}

bool ChunkManager::IsChunkLoaded(c_int x, c_int y) {
  std::shared_lock<std::shared_mutex> lock(chunksMutex);
  return chunks[x + 16 * y].get() != nullptr;
}

bool ChunkManager::IsChunkLoadedAt(b_int x, b_int y) {
  return IsChunkLoaded(x / 16, y / 16);
}
