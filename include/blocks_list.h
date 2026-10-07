#pragma once
#include "block.h"
#include <cstdint>
#include <string>

class SolidBlock : public Block {
private:
public:
  SolidBlock(const uint8_t width, const uint8_t height, const uint8_t depth,
             const bool hasTileEntity, uint8_t direction,
             const std::string &filePath);
  virtual ~SolidBlock() = default;

  void Draw(float x, float y, float z) override;
  void OnInteract() override;
  void OnNeighbourChange() override;
  void OnRandomTick() override;
};
