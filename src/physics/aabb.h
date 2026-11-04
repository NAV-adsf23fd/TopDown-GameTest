#pragma once
#include "src/utils/utility.h"
#include <raylib.h>

struct AABB {
private:
  Box3 box;
  CollisionFlags::Flags collisionFlag;
  bool IntersectsX(Box3 against) const;
  bool IntersectsY(Box3 against) const;
  bool IntersectsZ(Box3 against) const;
  bool Intersects(Box3 against) const;

  float GetClipX(Box3 against, float deltaX);
  float GetClipY(Box3 against, float deltaY);
  float GetClipZ(Box3 against, float deltaZ);

  Box3 Expand(float valueX, float valueY, float valueZ);
  Box3 Grow(float valueX, float valueY, float valueZ);
  void Move(float valueX, float valueY, float valueZ);

public:
  AABB(Vector3 position, Vector3 size);
  ~AABB() = default;
  void UpdateMovement(Vector3 velocity);

  void SetPosition(Vector3 position);
  Vector3 GetPosition() const;

  bool IsOnGround() const;
};
