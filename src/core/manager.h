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
