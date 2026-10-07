#include "utility.h"
#include <cstdint>
#include <cstdlib>
#include <math.h>

float dt = 0.0f;

Vector2 ScreenCoordinateOf(const Vector3 &position) {
  return {16 * position.x, 16 * position.y - 16 * position.z};
}

void Normalize2D(Vector3 &vec) {
  float length2D = sqrt(vec.x * vec.x + vec.y * vec.y);
  if (length2D != 0.0f) {
    vec.x /= length2D;
    vec.y /= length2D;
  }
}

namespace physics {
const float GRAVITY = -20.0f;
}

int Convention::GetAnimationRow(const uint8_t direction) {
  switch (direction) {
  case Direction::U:
    return 0;
  case Direction::U | Direction::R:
    return 1;
  case Direction::R:
    return 2;
  case Direction::D | Direction::R:
    return 3;
  case Direction::D:
    return 4;
  case Direction::D | Direction::L:
    return 5;
  case Direction::L:
    return 6;
  case Direction::U | Direction::L:
    return 7;
  default:
    return -1;
  }
}

float GetAngleFromDirection(const uint8_t direction) {
  switch (direction) {
  case Direction::U:
    return 6;
  case Direction::U | Direction::R:
    return 7;
  case Direction::R:
    return 0;
  case Direction::D | Direction::R:
    return 1;
  case Direction::D:
    return 2;
  case Direction::D | Direction::L:
    return 3;
  case Direction::L:
    return 4;
  case Direction::U | Direction::L:
    return 5;
  default:
    return -1;
  }
}

uint8_t GetDirectionFromAngle(const float angle) {
  if ((angle >= 7.5f && angle < 8.0f) || (angle < 0.5f && angle >= 0)) {
    return Direction::R;
  } else if (angle >= 0.5f && angle < 1.5f) {
    return Direction::R | Direction::D;
  } else if (angle >= 1.5f && angle < 2.5f) {
    return Direction::D;
  } else if (angle >= 2.5f && angle < 3.5f) {
    return Direction::L | Direction::D;
  } else if (angle >= 3.5f && angle < 4.5f) {
    return Direction::L;
  } else if (angle >= 4.5f && angle < 5.5f) {
    return Direction::L | Direction::U;
  } else if (angle >= 5.5f && angle < 6.5f) {
    return Direction::U;
  } else if (angle >= 6.5f && angle < 7.5f) {
    return Direction::R | Direction::U;
  } else {
    return Direction::NONE;
  }
}

float ShortestRotatePath(float currentAngle, const float targetAngle,
                         const float speed) {
  float angleDiff = targetAngle - currentAngle;

  // normalize to (-4, 4]
  if (angleDiff > 4)
    angleDiff -= 8;
  if (angleDiff <= -4)
    angleDiff += 8;

  if (abs(angleDiff) > 0.001f) { // float never = 0 so > 0 alway True
    float step = speed * dt;

    if (step > abs(angleDiff)) {
      currentAngle = targetAngle;
    } else {
      if (angleDiff > 0)
        currentAngle += step;
      if (angleDiff < 0)
        currentAngle -= step;
    }
  }
  // Normalize to [0, 8)
  if (currentAngle >= 8.0f)
    currentAngle -= 8.0f;
  if (currentAngle < 0.0f)
    currentAngle += 8.0f;
  return currentAngle;
}

void DrawVector(const Vector2 &startPos, const float alphaPosDegrees,
                const float length, const Color &color) {
  // 1. Chuyển đổi góc từ Độ sang Radian
  float alphaRadians = alphaPosDegrees * DEG2RAD * 360 / 8;

  // 2. Tính toán vị trí điểm kết thúc (endPos)
  Vector2 endPos;
  endPos.x = startPos.x + length * cosf(alphaRadians);
  endPos.y =
      startPos.y + length * sinf(alphaRadians); // + vì trục Y hướng xuống

  // 3. Vẽ đoạn thẳng từ gốc đến ngọn
  DrawLineV(startPos, endPos, color);
}
