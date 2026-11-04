#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class TexRsManager { // Singleton pattern OOP
private:
  std::unordered_map<std::string, Texture2D> textures;

  TexRsManager();
  ~TexRsManager();

  // Avoid copy data
  TexRsManager(const TexRsManager &) = delete;
  TexRsManager &operator=(const TexRsManager &) = delete;

public:
  // We can't create instance normally in singleton, so we must have static
  // function for that
  static TexRsManager &GetInstance();
  Texture2D &GetTexture(const std::string &filePath);
  void UnloadAllTextures();
};
