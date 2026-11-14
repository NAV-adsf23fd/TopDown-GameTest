#pragma once
#include "src/physics/object.h"
#include <cstdint>

class Bullet : Object {
private:
  uint8_t damage;

public:
  Bullet();
  ~Bullet();
};
