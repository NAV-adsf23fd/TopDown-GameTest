#pragma once

#include "raylib.h"

namespace PlayerCamera {
enum class Mode : bool {
  FREE,
  TARGET,
};

inline int ZOOM_SPPED = 10;
// prevent duplicate local shadow variable at Struc types
extern Mode cameraMode;
extern Camera2D camera;

void Init(Vector2 playerPos);
void CameraFunctionality(Vector2 playerPos);
void CameraUpdate(Vector2 playerPos);
} // namespace PlayerCamera
