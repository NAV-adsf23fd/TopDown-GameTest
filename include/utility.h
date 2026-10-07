#pragma once
#include <cstdint>
#include <raylib.h>

extern float dt; // delta time

struct Box3 {
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;
};

// enum class Direction : uint8_t {
//   U = 1 << 0, // Up
//   D = 1 << 1, // Down
//   L = 1 << 2, // Left
//   R = 1 << 3, // Right
// UL = U | L, // Up Left
// UR = U | R, // Up Right
// DL = D | L, // Down Left
// DR = D | R  // Down Right
// };

// inline constexpr Direction operator|(Direction flag1, Direction flag2) {
//   return static_cast<Direction>(
//       static_cast<std::underlying_type_t<Direction>>(flag1) |
//       static_cast<std::underlying_type_t<Direction>>(flag2));
// }

enum Direction : uint8_t {
  NONE = 0,
  U = 1 << 0, // Up
  D = 1 << 1, // Down
  L = 1 << 2, // Left
  R = 1 << 3, // Right
};

// I have to cast to uint8_t in output because of some language problem that
// make my Direction type doesn't work properly
inline constexpr uint8_t operator|(Direction flag1, Direction flag2) {
  return static_cast<uint8_t>(static_cast<uint8_t>(flag1) |
                              static_cast<uint8_t>(flag2));
}

namespace CollisionFlags {
enum Flags : int8_t {
  None = 0,
  Px = 1 << 0,
  Nx = 1 << 1,
  Py = 1 << 2,
  Ny = 1 << 3,
  Pz = 1 << 4,
  Nz = 1 << 5
};
inline bool HasFlag(Flags a, Flags b) { return (a & b) != None; }
inline void AddFlag(Flags &a, Flags b) { a = (Flags)(a | b); }
inline void RemoveFlag(Flags &a, Flags b) { a = (Flags)(a & ~b); }
} // namespace CollisionFlags

namespace physics {
extern const float GRAVITY;
}

namespace Convention {
int GetAnimationRow(const uint8_t direction);
}

Vector2 ScreenCoordinateOf(const Vector3 &position);
void Normalize2D(Vector3 &vec);
float GetAngleFromDirection(const uint8_t direction);
uint8_t GetDirectionFromAngle(const float angle);
float ShortestRotatePath(float currentAngle, const float targetAngle,
                         const float speed);

void DrawVector(const Vector2 &startPos, const float alphaPosDegrees,
                const float length, const Color &color);
