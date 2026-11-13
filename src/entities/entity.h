#pragma once
#include "src/physics/aabb.h"
#include "src/physics/object.h"
#include "src/resources/animation.h"
#include "src/utils/utility.h"
#include <cstdint>
#include <raylib.h>
#include <vector>

enum class EntityState : int8_t {
  IDLE = 0,
  RUN = 1,
  ATTACK = 2,
  DIE = 3,
};

class Entity : public Object {
protected:
  AABB collisionBox;
  Vector3 velocity;
  Vector3 acceleration;

  EntityState currentState;
  Direction currentDirection;

  int hp, maxHp;

  std::vector<Animation> animations;
  virtual void UpdateAnimation(Direction direction);

private:
  virtual bool CanChangeState(EntityState newState) = 0;
  virtual void ChangeState(EntityState newState) = 0;

public:
  Entity(Vector3 position, Vector3 size);
  Entity(Vector3 position, float width, float height, float depth);
  Entity(float x, float y, float z, float width, float height, float depth);
  virtual ~Entity() = default;

  virtual void Update() = 0;
  virtual void SetDirection(Direction direction) = 0;
  virtual Vector3 GetPosition() = 0;
  virtual void AddCollider(const Box3 &target) = 0;
  void ClearCollider();
};
