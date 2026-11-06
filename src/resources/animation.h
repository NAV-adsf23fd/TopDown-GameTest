#pragma once
#include "src/utils/utility.h"
#include <cstdint>
#include <raylib.h>
#include <string>
#include <unordered_map>

class AnimRsManager { // Singleton pattern OOP
private:
  std::unordered_map<std::string, Texture2D> textures;

  AnimRsManager();
  ~AnimRsManager();

  // Avoid copy data
  AnimRsManager(const AnimRsManager &) = delete;
  AnimRsManager &operator=(const AnimRsManager &) = delete;

public:
  // We can't create instance normally in singleton, so we must have static
  // function for that
  static AnimRsManager &GetInstance();
  Texture2D &GetTexture(const std::string &filePath);
  void UnloadAllTextures();
};

class Animation {
private:
  Texture2D spriteSheet;

  int firstFrame;
  int currentFrame;
  int lastFrame;
  Direction currentDirection;

  float frameWidth;
  float frameHeight;
  Rectangle frameRect;

  float frameSpeed;
  float frameTimer;

  // std::string textureName;
  // int maxFrames;
  // float frameTimer;
  // float frameSpeed;
  // Rectangle frameRec;
  //
  // int currentDirection;
  //
  // float frameWidth;
  // float frameHeight;
  //
  // int baseRow;

public:
  Animation(const std::string &filePath, float width, float height,
            float baseTime, Direction direction);
  ~Animation() = default;

  void Play(uint8_t direction);
  void Pause();
  void Resume();

  void Update();
  void Draw(Vector2 position);

  void SetDirection(Direction direction);
  int GetCurrentFrame() const;
};
