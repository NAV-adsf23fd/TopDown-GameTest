#include "utility.h"
#include <cstdint>
#include <cstdlib>
#include <math.h>

float dt = 0.0f;

Vector2 ScreenCoordinateOf(const Vector3 &position) {
  return {8 * position.x, 8 * position.y - 8 * position.z};
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

uint8_t Convention::GetAnimationRow(const Direction direction) {
  // Only use lookup tables when they are called frequently. Otherwise, use
  // if-else/switch-case statements instead
  static constexpr uint8_t row_lookup[11] = {
      0, // 0  (NONE) fallback
      0, // 1  (U)
      4, // 2  (D)
      0, // 3  fallback
      6, // 4  (L)
      7, // 5  (U|L)
      5, // 6  (D|L)
      0, // 7  fallback
      2, // 8  (R)
      1, // 9  (U|R)
      3, // 10 (D|R)
  };
  uint8_t index = static_cast<uint8_t>(direction);
  return (index < 11) ? row_lookup[index] : 0;
}

float GetAngleFromDirection(const Direction direction) {
  static constexpr uint8_t row_lookup[11] = {
      0, // 0  (NONE) fallback
      6, // 1  (U)
      2, // 2  (D)
      0, // 3  fallback
      4, // 4  (L)
      5, // 5  (U|L)
      3, // 6  (D|L)
      0, // 7  fallback
      0, // 8  (R)
      7, // 9  (U|R)
      1, // 10 (D|R)
  };
  uint8_t index = static_cast<uint8_t>(direction);
  return (index < 11) ? row_lookup[index] : 0;
}

Direction GetDirectionFromAngle(const float angle) {
  static constexpr Direction direction_lookup[] = {
      Direction::R,                // 0
      Direction::R | Direction::D, // 1
      Direction::D,                // 2
      Direction::L | Direction::D, // 3
      Direction::L,                // 4
      Direction::L | Direction::U, // 5
      Direction::U,                // 6
      Direction::R | Direction::U  // 7
  };

  if (angle < 0.0f || angle >= 8.0f)
    return Direction::NONE;

  int index = static_cast<int>(angle + 0.5f);

  if (index >= 8)
    index = 0;

  return direction_lookup[index];
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
