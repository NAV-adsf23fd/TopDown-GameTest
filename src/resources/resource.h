#pragma once
#include "raylib.h"
#include "src/core/manager.h"
#include <string>
#include <unordered_map>

class TexRsManager {
  MAKE_SINGLETON(TexRsManager);

private:
  std::unordered_map<std::string, Texture2D> textures;

public:
  Texture2D &GetTexture(const std::string &filePath);
  void UnloadAllTextures();
};
