#include "player.h"
#include "entity.h"
#include "raylib.h"
#include "src/functionality/render.h"
#include "src/physics/collision.h"
#include "src/resources/animation.h"
#include "src/utils/logger.h"
#include "src/utils/utility.h"
#include <mutex>
#include <raymath.h>

Player::Player(Vector3 position) : Entity(position, Vector3{16, 16, 24}) {
  currentState = EntityState::IDLE;
  currentDirection = Direction::D;
  maxHp = 100;
  hp = maxHp;
  currentAngle = 2;
  targetAngle = currentAngle;

  animations = {
      Animation("assets/textures/temp.png", 16, 24, 0.3, currentDirection),
  };

  RenderManager::GetInstance().AddItem(RenderItem{position.x, position.y,
                                                  position.z,
                                                  static_cast<Entity *>(this)},
                                       "player");
  CollisionManager::GetInstance().AddObj(
      CollObj{position.x, position.y, position.z, static_cast<Entity *>(this)});

  name = "player";
}

void Player::Update() {
  Move(currentDirection);
  UpdateAnimation(GetDirectionFromAngle(currentAngle));
}

void Player::Draw(float, float, float) {
  for (auto &animation : animations) {
    animation.Draw(ScreenCoordinateOf(collisionBox.GetPosition()));
  }
}

void Player::DrawDebug() const {
  Vector3 tpos = collisionBox.GetPosition();
  std::string s1 = "Pos: \nx: " + std::to_string(tpos.x) +
                   "\ny: " + std::to_string(tpos.y) +
                   "\nz: " + std::to_string(tpos.z);
  DrawText(s1.c_str(), 650, 10, 20, BLACK);

  Vector3 vel = velocity;
  std::string s2 = "Vel: \nx: " + std::to_string(vel.x) +
                   "\ny: " + std::to_string(vel.y) +
                   "\nz: " + std::to_string(vel.z);
  DrawText(s2.c_str(), 500, 10, 20, BLACK);

  std::string s3 = "isOnGround: " + std::to_string(collisionBox.IsOnGround());
  DrawText(s3.c_str(), 600, 530, 20, BLACK);
}

void Player::Move(Direction direction) {
  // std::unique_lock<std::mutex> lock(positionMutex);
  velocity = {0, 0, velocity.z};
  if ((direction & Direction::U) != Direction::NONE)
    velocity.y -= 1;
  if ((direction & Direction::D) != Direction::NONE)
    velocity.y += 1;
  if ((direction & Direction::L) != Direction::NONE)
    velocity.x -= 1;
  if ((direction & Direction::R) != Direction::NONE)
    velocity.x += 1;

  Normalize2D(velocity);

  if (direction != Direction::NONE)
    targetAngle = GetAngleFromDirection(direction);
  currentAngle = ShortestRotatePath(currentAngle, targetAngle, ANGULAR_SPEED);

  bool isOnGround = collisionBox.IsOnGround();
  if (isOnGround) {
    if (IsKeyDown(KEY_SPACE)) {
      velocity.z = JUMPSPEED;
    } else {
      velocity.z = 0;
    }
  } else {
    velocity.z += physics::GRAVITY * dt;
  }
  collisionBox.UpdateMovement(velocity, INITIALSPEED * dt);
}

void Player::SetDirection(Direction direction) {
  // std::unique_lock<std::mutex> lock(positionMutex);
  currentDirection = direction;
}
Vector3 Player::GetPosition() {
  // std::unique_lock<std::mutex> lock(positionMutex);
  return collisionBox.GetPosition();
}
float Player::GetAngle() { return currentAngle; }

bool Player::CanChangeState(EntityState newState) { return true; }
void Player::ChangeState(EntityState newState) {}

void Player::AddCollider(const Box3 &target) {
  collisionBox.SetColidersList(target);
}
