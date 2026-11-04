#include "resource.h"

TexRsManager::TexRsManager() {}

TexRsManager::~TexRsManager() { UnloadAllTextures(); }

TexRsManager &TexRsManager::GetInstance() {
  static TexRsManager instance;
  return instance;
}

Texture2D &TexRsManager::GetTexture(const std::string &filePath) {
  auto it = textures.find(filePath);

  if (it != textures.end()) {
    return it->second;
  }

  textures[filePath] = LoadTexture(filePath.c_str());
  return textures[filePath];
}

void TexRsManager::UnloadAllTextures() {
  for (const auto &pair : textures) {
    UnloadTexture(pair.second);
  }
  textures.clear();
}
