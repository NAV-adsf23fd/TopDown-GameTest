#pragma once
#include "src/physics/aabb.h"
#include "src/resources/animation.h"
#include <cstdint>
#include <raylib.h>
#include <vector>

enum class EntityState : int8_t {
  IDLE = 0,
  RUN = 1,
  ATTACK = 2,
  DIE = 3,
};

class Entity {
protected:
  AABB collisionBox;
  Vector3 velocity;
  Vector3 acceleration;

  EntityState currentState;
  uint8_t currentDirection;

  int hp, maxHp;

  std::vector<Animation> animations;
  virtual void UpdateAnimation(uint8_t direction);

private:
  virtual bool CanChangeState(EntityState newState) = 0;
  virtual void ChangeState(EntityState newState) = 0;

public:
  Entity(Vector3 position, Vector3 size);
  Entity(Vector3 position, float width, float height, float depth);
  Entity(float x, float y, float z, float width, float height, float depth);
  virtual ~Entity() = default;

  virtual void Draw() = 0;
  virtual void Update() = 0;
};
