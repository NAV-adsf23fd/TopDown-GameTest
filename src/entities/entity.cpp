#include "entity.h"
#include "raylib.h"
#include "src/resources/animation.h"
#include "src/utils/utility.h"

Entity::Entity(Vector3 position, Vector3 size) : collisionBox(position, size) {
  velocity = Vector3{0, 0, 0};
  acceleration = Vector3{0, 0, 0};
  currentState = EntityState::IDLE;
}

Entity::Entity(Vector3 position, float width, float height, float depth)
    : Entity(position, Vector3{width, height, depth}) {}

Entity::Entity(float x, float y, float z, float width, float height,
               float depth)
    : Entity(Vector3{x, y, z}, Vector3{width, height, depth}) {}

// void Entity::Draw() {
//   for (auto animation : animations) {
//     Vector3 position = collisionBox.GetPosition();
//     animation.Draw(ScreenCoordinateOf(position));
//   }
// }

void Entity::UpdateAnimation(Direction direction) {
  if (direction == Direction::NONE)
    return;
  for (auto &animation : animations) {
    animation.SetDirection(direction);
    animation.Update();
  }
}

void Entity::ChangeState(EntityState newState) {
  if (CanChangeState(newState)) {
    currentState = newState;
  }
}
