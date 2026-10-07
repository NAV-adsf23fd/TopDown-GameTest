#include "aabb.h"
#include <vector>

AABB::AABB(Vector3 position, Vector3 size)
    : collisionFlag(CollisionFlags::None) {
  box.minX = position.x;
  box.maxX = position.x + size.x;
  box.minY = position.y;
  box.maxY = position.y + size.y;
  box.minZ = position.z;
  box.maxZ = position.z + size.z;
}

Box3 AABB::Expand(float valueX, float valueY, float valueZ) {
  Box3 returnValue = box;

  if (valueX > 0)
    returnValue.maxX += valueX;
  else
    returnValue.minX += valueX;

  if (valueY > 0)
    returnValue.maxY += valueY;
  else
    returnValue.minY += valueY;

  if (valueZ > 0)
    returnValue.maxZ += valueZ;
  else
    returnValue.minZ += valueZ;

  return returnValue;
}

Box3 AABB::Grow(float valueX, float valueY, float valueZ) {
  Box3 returnValue = box;

  returnValue.minX -= valueX;
  returnValue.minY -= valueY;
  returnValue.minZ -= valueZ;

  returnValue.maxX += valueX;
  returnValue.maxY += valueY;
  returnValue.maxZ += valueZ;

  return returnValue;
}

void AABB::Move(float valueX, float valueY, float valueZ) {
  box.minX += valueX;
  box.maxX += valueX;
  box.minY += valueY;
  box.maxY += valueY;
  box.minZ += valueZ;
  box.maxZ += valueZ;
}

bool AABB::IntersectsX(Box3 against) const {
  return box.minX < against.maxX && box.maxX > against.minX;
}

bool AABB::IntersectsY(Box3 against) const {
  return box.minY < against.maxY && box.maxY > against.minY;
}

bool AABB::IntersectsZ(Box3 against) const {
  return box.minZ < against.maxZ && box.maxZ > against.minZ;
}

bool AABB::Intersects(Box3 against) const {
  return IntersectsX(against) && IntersectsY(against) && IntersectsZ(against);
}

float AABB::GetClipX(Box3 against, float deltaX) {
  if (IntersectsY(against) && IntersectsZ(against)) {
    if (deltaX > 0 && box.maxX <= against.minX) {
      float clip = against.minX - box.maxX;
      if (deltaX > clip)
        deltaX = clip;
    }

    if (deltaX < 0 && box.minX >= against.maxX) {
      float clip = against.maxX - box.minX;
      if (deltaX < clip)
        deltaX = clip;
    }
    return deltaX;
  }
  return deltaX;
}

float AABB::GetClipY(Box3 against, float deltaY) {
  if (IntersectsX(against) && IntersectsZ(against)) {
    if (deltaY > 0 && box.maxY <= against.minY) {
      float clip = against.minY - box.maxY;
      if (deltaY > clip)
        deltaY = clip;
    }

    if (deltaY < 0 && box.minY >= against.maxY) {
      float clip = against.maxY - box.minY;
      if (deltaY < clip)
        deltaY = clip;
    }
    return deltaY;
  }
  return deltaY;
}

float AABB::GetClipZ(Box3 against, float deltaZ) {
  if (IntersectsY(against) && IntersectsX(against)) {
    if (deltaZ > 0 && box.maxZ <= against.minZ) {
      float clip = against.minZ - box.maxZ;
      if (deltaZ > clip)
        deltaZ = clip;
    }

    if (deltaZ < 0 && box.minZ >= against.maxZ) {
      float clip = against.maxZ - box.minZ;
      if (deltaZ < clip)
        deltaZ = clip;
    }
    return deltaZ;
  }
  return deltaZ;
}

bool AABB::IsOnGround() const {
  return CollisionFlags::HasFlag(collisionFlag, CollisionFlags::Nz) ||
         box.minZ <= 0;
}

void AABB::SetPosition(Vector3 position) {
  float sizeX = box.maxX - box.minX;
  float sizeY = box.maxY - box.minY;
  float sizeZ = box.maxZ - box.minZ;

  box.minX = position.x;
  box.minY = position.y;
  box.minZ = position.z;
  box.maxX = position.x + sizeX;
  box.maxY = position.y + sizeY;
  box.maxZ = position.z + sizeZ;
}

Vector3 AABB::GetPosition() const { return {box.minX, box.minY, box.minZ}; }

void AABB::UpdateMovement(Vector3 velocity) {
  collisionFlag = CollisionFlags::None;

  Vector3 moveVector = velocity;
  Vector3 originalVector = velocity;

  std::vector<Box3> colidersList = {};

  for (Box3 collider : colidersList) {
    moveVector.x = GetClipX(collider, moveVector.x);
  }
  Move(moveVector.x, 0, 0);

  for (Box3 collider : colidersList) {
    moveVector.y = GetClipY(collider, moveVector.y);
  }
  Move(0, moveVector.y, 0);

  for (Box3 collider : colidersList) {
    moveVector.z = GetClipZ(collider, moveVector.z);
  }
  Move(0, 0, moveVector.z);

  if (moveVector.z != originalVector.z) {
    if (originalVector.z < 0)
      CollisionFlags::AddFlag(collisionFlag, CollisionFlags::Nz);
    else
      CollisionFlags::AddFlag(collisionFlag, CollisionFlags::Pz);
  }

  if (moveVector.x != originalVector.x)
    velocity.x = 0;

  if (moveVector.y != originalVector.y)
    velocity.y = 0;

  if (moveVector.z != originalVector.z)
    velocity.z = 0;
}
