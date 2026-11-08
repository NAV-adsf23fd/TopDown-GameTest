#include "blocks_list.h"
#include "block.h"
#include "raylib.h"
#include "src/utils/utility.h"
#include <cstdint>

// Solid block
SolidBlock::SolidBlock(const std::string &filePath, const uint8_t width,
                       const uint8_t height, const uint8_t depth,
                       Direction direction, const bool hasTileEntity)
    : Block(filePath, width, height, depth, direction, hasTileEntity, true) {}

void SolidBlock::Draw(float x, float y, float z) {
  DrawTextureRec(spriteSheet, Rectangle{0, 0, 16, 24},
                 ScreenCoordinateOf(Vector3{x, y, z}), WHITE);
}

void SolidBlock::OnInteract() {}
void SolidBlock::OnNeighbourChange() {}
void SolidBlock::OnRandomTick() {}
