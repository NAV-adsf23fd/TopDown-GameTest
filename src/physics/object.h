#pragma once
#include <string>

class Object {
protected:
public:
  std::string name = "obj";
  void virtual Draw(float x, float y, float z) = 0;
};
