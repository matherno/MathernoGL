#pragma once

#include <list>
#include "maths/Transform.h"

class TransformStack
  {
private:
  std::list<mathernogl::Transform> stack;

public:
  void push(const mathernogl::Transform* transform);
  void pop();
  mathernogl::Transform* getTop();
  void clear();
  int size() const;
  };
