#pragma once

#define MAKE_SINGLETON(ClassName)                                              \
private:                                                                       \
  ClassName();                                                                 \
  ~ClassName();                                                                \
  ClassName(const ClassName &) = delete;                                       \
  ClassName &operator=(const ClassName &) = delete;                            \
                                                                               \
public:                                                                        \
  static ClassName &GetInstance() {                                            \
    static ClassName instance;                                                 \
    return instance;                                                           \
  }

#define TexRsManager TexRsManagerSingleton::GetInstance()
#define AnimRsManager AnimRsManagerSingleton::GetInstance()
#define BlockManager BlockManagerSingleton::GetInstance()
#define ChunkManager ChunkManagerSingleton::GetInstance()
#define RenderManager RenderManagerSingleton::GetInstance()
#define CollisionManager CollisionManagerSingleton::GetInstance()
