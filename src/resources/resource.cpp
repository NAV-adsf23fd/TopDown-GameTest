#include "resource.h"
#include "raylib.h"

TexRsManagerSingleton::TexRsManagerSingleton() {}

TexRsManagerSingleton::~TexRsManagerSingleton() { UnloadAllTextures(); }

Texture2D &TexRsManagerSingleton::GetTexture(const std::string &filePath) {
  auto it = textures.find(filePath);

  if (it != textures.end()) {
    return it->second;
  }

  textures[filePath] = LoadTexture(filePath.c_str());
  SetTextureFilter(textures[filePath], TEXTURE_FILTER_POINT);
  return textures[filePath];
}

void TexRsManagerSingleton::UnloadAllTextures() {
  for (const auto &pair : textures) {
    UnloadTexture(pair.second);
  }
  textures.clear();
}
