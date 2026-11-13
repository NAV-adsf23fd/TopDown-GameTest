#pragma once
#include "src/utils/utility.h"
#include <raylib.h>
#include <vector>

struct AABB {
private:
  Box3 box;
  std::vector<Box3> colidersList;
  CollisionFlags::Flags collisionFlag;
  bool IntersectsX(const Box3 &against) const;
  bool IntersectsY(const Box3 &against) const;
  bool IntersectsZ(const Box3 &against) const;
  bool Intersects(const Box3 &against) const;

  float GetClipX(const Box3 &against, float deltaX);
  float GetClipY(const Box3 &against, float deltaY);
  float GetClipZ(const Box3 &against, float deltaZ);

  Box3 Expand(float valueX, float valueY, float valueZ);
  Box3 Grow(float valueX, float valueY, float valueZ);
  void Move(float valueX, float valueY, float valueZ);

public:
  AABB(Vector3 &position, Vector3 &size);
  ~AABB() = default;
  void UpdateMovement(Vector3 &velocity, const float INITIALSPEED);

  void SetPosition(Vector3 &position);
  Vector3 GetPosition() const;

  void SetColidersList(const Box3 &target);
  void ClearColidersList();

  bool IsOnGround() const;
};
