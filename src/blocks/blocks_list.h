#pragma once
#include "block.h"
#include "src/utils/utility.h"
#include <cstdint>
#include <string>

class SolidBlock : public Block {
private:
public:
  SolidBlock(const std::string &filePath, const uint8_t width = 1,
             const uint8_t height = 1, const uint8_t depth = 1,
             Direction direction = Direction::U,
             const bool hasTileEntity = false);
  virtual ~SolidBlock() = default;

  void Draw(float x, float y, float z) override;
  void OnInteract() override;
  void OnNeighbourChange() override;
  void OnRandomTick() override;
};
