#pragma once
#include <cstdint>
#include <ostream>
#include <raylib.h>
#include <string>

// clang-format off
typedef int8_t b_int;    // type 8bits for block coord (x, y, z) in world
typedef int32_t bi_int;  // type 24bits for index block coord (x * y * z = 256 * 256 * 256) in world
typedef int8_t c_int;    // type 4bits for chunk coord (x, y) in world
typedef int8_t ci_int;   // type 8bits for index chunk coord (x * y = 16 * 16) in world
typedef int8_t bc_int;   // type 8bits for block coord (x, y, z) in chunk
typedef int16_t bci_int; // type 16bits for index block coord (x * y * z = 16 * 16 * 256) in chunk

#define CHUNK_IN_WORLD 16 * 16
#define BLOCK_IN_WORLD 256 * 256 * 256
#define BLOCK_IN_CHUNK 16 * 16 * 256
// clang-format on

#define SCREENSIZE 800, 600
#define SCREENNAME "Topdown"

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

enum class Direction : uint8_t {
  NONE = 0,
  U = 1 << 0, // Up
  D = 1 << 1, // Down
  L = 1 << 2, // Left
  R = 1 << 3, // Right
};

// I have to cast to uint8_t in output because of some language problem that
// make my Direction type doesn't work properly
inline constexpr Direction operator|(Direction flag1, Direction flag2) {
  return static_cast<Direction>(static_cast<uint8_t>(flag1) |
                                static_cast<uint8_t>(flag2));
}
inline constexpr Direction operator&(Direction flag1, Direction flag2) {
  return static_cast<Direction>(static_cast<uint8_t>(flag1) &
                                static_cast<uint8_t>(flag2));
}
inline constexpr Direction operator|=(Direction &flag1, Direction flag2) {
  flag1 = static_cast<Direction>(static_cast<uint8_t>(flag1) |
                                 static_cast<uint8_t>(flag2));
  return flag1;
}

// print Vector3
inline std::ostream &operator<<(std::ostream &out, const Vector3 &vec) {
  out << "( " << std::to_string(vec.x) << "," << std::to_string(vec.y) << ","
      << std::to_string(vec.z) << ")";
  return out;
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
uint8_t GetAnimationRow(const Direction direction);
}

Vector2 ScreenCoordinateOf(const Vector3 &position);
void Normalize2D(Vector3 &vec);
float GetAngleFromDirection(const Direction direction);
Direction GetDirectionFromAngle(const float angle);
float ShortestRotatePath(float currentAngle, const float targetAngle,
                         const float speed);

void DrawVector(const Vector2 &startPos, const float alphaPosDegrees,
                const float length, const Color &color);
