#include "blocks_list.h"
#include "block.h"
#include "raylib.h"
#include "src/utils/utility.h"
#include <cstdint>

// Solid block
SolidBlock::SolidBlock(const uint8_t width, const uint8_t height,
                       const uint8_t depth, const bool hasTileEntity,
                       uint8_t direction, const std::string &filePath)
    : Block(width, height, depth, hasTileEntity, true, direction, filePath) {}

void SolidBlock::Draw(float x, float y, float z) {
  DrawTextureRec(spriteSheet, Rectangle{0, 0, 16, 24},
                 ScreenCoordinateOf(Vector3{x, y, z}), WHITE);
}

void SolidBlock::OnInteract() {}
void SolidBlock::OnNeighbourChange() {}
void SolidBlock::OnRandomTick() {}
