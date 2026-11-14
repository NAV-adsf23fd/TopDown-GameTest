#include "map.h"
#include "src/blocks/block.h"
#include "src/functionality/render.h"
#include "src/physics/collision.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <mutex>
#include <string>
#include <sys/types.h>

Chunk::Chunk(c_int x, c_int y) : x(x), y(y) {
  std::fill_n(position, BLOCK_IN_CHUNK, BlockType::None);
}

BlockType Chunk::GetBlockAt(bc_int x, bc_int y, bc_int z) {
  return position[x + y * 16 + z * 16 * 16];
}

void Chunk::SetBlockAt(bc_int x, bc_int y, bc_int z, BlockType block) {
  position[x + y * 16 + z * 16 * 16] = block;
  RenderManager::GetInstance().AddItem(
      RenderItem{(float)x, (float)y, (float)z,
                 &(BlockManager::GetInstance().GetBlock(block))});
  CollisionManager::GetInstance().AddObj(
      CollObj{(float)x, (float)y, (float)z,
              &(BlockManager::GetInstance().GetBlock(block))});
  Logger::qlog(Logger::Level::INFO, "Set block at block_coord(",
               std::to_string(z), ")");
}

ChunkManager::ChunkManager() {}
ChunkManager::~ChunkManager() {}

Chunk *ChunkManager::GetChunk(c_int x, c_int y) {
  // std::shared_lock lock(chunksMutex);
  return chunks[x + y * 16];
}

Chunk *ChunkManager::GetChunkAt(b_int x, b_int y) {
  return GetChunk(x / 16, y / 16);
}

void ChunkManager::SetBlockAt(b_int x, b_int y, b_int z, BlockType block) {
  Chunk *chunk = GetChunkAt(x, y);
  if (chunk == nullptr) { // clang-format off
    Logger::qlog(Logger::Level::ERROR, "Didn't load chunk at block_coord(",std::to_string(x),",",std::to_string(y),")"); // clang-format on
  } else
    chunk->SetBlockAt(x - chunk->x * 16, y - chunk->y * 16, z, block);
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
  // std::unique_lock lock(chunksMutex);
  for (c_int i = x1; i <= x2; ++i)
    for (c_int j = y1; j <= y2; ++j) {
      ci_int index = i + j * 16;
      if (chunks[index] == nullptr) {
        chunks[index] = new Chunk(i, j);
        Logger::qlog(Logger::Level::INFO, "Loaded chunk at chunk_coord(",
                     std::to_string(i), ",", std::to_string(j), ")");
      }
    }
}

void ChunkManager::LoadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2) {
  LoadChunks(x1 / 16, y1 / 16, x2 / 16, y2 / 16);
}

void ChunkManager::UnloadChunks(c_int x1, c_int y1, c_int x2, c_int y2) {
  // std::unique_lock lock(chunksMutex);
  for (c_int i = x1; i <= x2; ++i)
    for (c_int j = y1; j <= y2; ++j) {
      ci_int index = i + j * 16;
      if (chunks[index] != nullptr) {
        delete chunks[index];
        chunks[index] = nullptr;
      }
    }
}

void ChunkManager::UnloadChunksAt(b_int x1, b_int y1, b_int x2, b_int y2) {
  UnloadChunks(x1 / 16, y1 / 16, x2 / 16, y2 / 16);
}

bool ChunkManager::IsChunkLoaded(c_int x, c_int y) {
  // std::unique_lock lock(chunksMutex);
  return chunks[x + 16 * y] != nullptr;
}

bool ChunkManager::IsChunkLoadedAt(b_int x, b_int y) {
  return IsChunkLoaded(x / 16, y / 16);
}
