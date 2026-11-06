#include "animation.h"
#include "raylib.h"
#include "src/utils/utility.h"
#include <cstdint>

// AnimRsManager

AnimRsManager::AnimRsManager() {}

AnimRsManager::~AnimRsManager() { UnloadAllTextures(); }

AnimRsManager &AnimRsManager::GetInstance() {
  static AnimRsManager instance;
  return instance;
}

Texture2D &AnimRsManager::GetTexture(const std::string &filePath) {
  auto it = textures.find(filePath);

  if (it != textures.end()) {
    return it->second;
  }

  textures[filePath] = LoadTexture(filePath.c_str());
  SetTextureFilter(textures[filePath], TEXTURE_FILTER_POINT);
  return textures[filePath];
}

void AnimRsManager::UnloadAllTextures() {
  for (const auto &pair : textures) {
    UnloadTexture(pair.second);
  }
  textures.clear();
}

// Animation

Animation::Animation(const std::string &filePath, float width, float height,
                     float baseTime, Direction direction) {
  spriteSheet = AnimRsManager::GetInstance().GetTexture(filePath);

  firstFrame = 0;
  lastFrame = spriteSheet.width / width;
  currentFrame = firstFrame;
  currentDirection = direction;

  frameWidth = width;
  frameHeight = height;
  frameRect = Rectangle{0, 0, width, height};

  frameSpeed = baseTime;
  frameTimer = 0.0f;
}

void Animation::Play(uint8_t direction) {
  // if (baseRow != startRow || maxFrames != frames) {
  //   currentFrame = 0;
  //   baseRow = startRow;
  //   maxFrames = frames;
  //   frameSpeed = speed;
  //   frameTimer = 0.0f;
  if (currentFrame != firstFrame) {
    currentFrame = firstFrame;
    frameTimer = 0.0f;
  }
}

void Animation::Update() {
  frameTimer += dt;
  if (frameTimer >= frameSpeed) {
    frameTimer = 0.0f;
    currentFrame++;

    if (currentFrame >= lastFrame) {
      currentFrame = firstFrame;
    }
  }
  // temp
  frameRect.y = currentFrame * frameHeight;
  frameRect.x = Convention::GetAnimationRow(currentDirection) * frameWidth;
}

void Animation::Draw(Vector2 position) {
  DrawTextureRec(spriteSheet, frameRect, position, WHITE);
}

void Animation::SetDirection(Direction direction) {
  currentDirection = direction;
}

int Animation::GetCurrentFrame() const { return currentFrame; }
