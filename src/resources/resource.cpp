#include "resource.h"
#include "raylib.h"

TexRsManager::TexRsManager() {}

TexRsManager::~TexRsManager() { UnloadAllTextures(); }

Texture2D &TexRsManager::GetTexture(const std::string &filePath) {
  auto it = textures.find(filePath);

  if (it != textures.end()) {
    return it->second;
  }

  textures[filePath] = LoadTexture(filePath.c_str());
  SetTextureFilter(textures[filePath], TEXTURE_FILTER_POINT);
  return textures[filePath];
}

void TexRsManager::UnloadAllTextures() {
  for (const auto &pair : textures) {
    UnloadTexture(pair.second);
  }
  textures.clear();
}
