#include "player.h"
#include "entity.h"
#include "raylib.h"
#include "src/resources/animation.h"
#include "src/utils/utility.h"
#include <cstdint>
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
}

void Player::Update() {
  Move(currentDirection);
  UpdateAnimation(GetDirectionFromAngle(currentAngle));
}

void Player::Draw() {
  // Texture2D &texture = TextureManager::Get("player0");
  // Vector2 screenPosition = ScreenCoordinateOf(collisionBox.GetPosition());
  // DrawTextureV(texture, screenPosition, WHITE);

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

  DrawVector(ScreenCoordinateOf(collisionBox.GetPosition()), currentAngle, 10,
             RED);
}

void Player::Move(uint8_t direction) {
  velocity = {0, 0, velocity.z};
  if (direction & Direction::U)
    velocity.y -= 1;
  if (direction & Direction::D)
    velocity.y += 1;
  if (direction & Direction::L)
    velocity.x -= 1;
  if (direction & Direction::R)
    velocity.x += 1;

  Normalize2D(velocity);

  if (direction != 0)
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
  collisionBox.UpdateMovement({velocity.x * INITIALSPEED * dt,
                               velocity.y * INITIALSPEED * dt,
                               velocity.z * dt});
}

void Player::SetDirection(uint8_t direction) { currentDirection = direction; }
Vector3 Player::GetPosition() { return collisionBox.GetPosition(); }

bool Player::CanChangeState(EntityState newState) { return true; }
void Player::ChangeState(EntityState newState) {}
