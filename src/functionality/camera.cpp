#include "camera.h"
#include "raylib.h"
#include "raymath.h"

PlayerCamera::Mode PlayerCamera::cameraMode = PlayerCamera::Mode::TARGET;
Camera2D PlayerCamera::camera = {0};

void PlayerCamera::Init(Vector2 playerPos) {
  camera.zoom = 1.0f;
  camera.target = playerPos;
  camera.offset = (Vector2){GetScreenHeight() / 2.0f, GetScreenWidth() / 2.0f};
}

void PlayerCamera::CameraFunctionality(Vector2 playerPos) {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    cameraMode = PlayerCamera::Mode::FREE;
    Vector2 mouseDelta = GetMouseDelta();

    if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f) {
      mouseDelta = Vector2Scale(mouseDelta, -1.0f / camera.zoom);
      camera.target = Vector2Add(camera.target, mouseDelta);
    }

  } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    cameraMode = PlayerCamera::Mode::TARGET;
  }

  if (cameraMode == Mode::FREE) {

  } else if (cameraMode == Mode::TARGET) {
    camera.target = playerPos;
    camera.offset =
        (Vector2){GetScreenHeight() / 2.0f, GetScreenWidth() / 2.0f};
  }
  camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
  camera.zoom = fmax(camera.zoom, 1.0f);
  camera.zoom = fmin(camera.zoom, 3.0f);
}

void PlayerCamera::CameraUpdate(Vector2 playerPos) {
  CameraFunctionality(playerPos);
}
