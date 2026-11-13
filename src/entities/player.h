#pragma once
#include "entity.h"
#include "src/utils/utility.h"
#include <mutex>
#include <raylib.h>

class Player : Entity {
private:
  const float INITIALSPEED = 5;
  const float JUMPSPEED = 5;
  const float STATICFRICTION = -0.5f;
  const float AIRFRICTION = -0.5f;

  float currentAngle;
  float targetAngle;
  const float ANGULAR_SPEED = 16;
  // my own angle unit from 0 -> 7 (R -> DR -> D -> DL -> L -> UL -> U -> UR), 8
  // direction -> angle: 0,1,2,3,4,5,6,7), speed = n means player can rotate n
  // direction 1 seconds. Use GetAngleFromDirection

  std::mutex positionMutex;

  void Move(Direction direction);
  // void UpdateAnimation() override;
  bool CanChangeState(EntityState newState) override;
  void ChangeState(EntityState newState) override;

public:
  Player(Vector3 position);
  ~Player() override = default;

  void SetDirection(Direction direction) override;
  Vector3 GetPosition() override;
  float GetAngle();
  void AddCollider(const Box3 &target) override;

  void Update() override;
  void Draw(float, float, float) override;
  void DrawDebug() const; // temp
};
