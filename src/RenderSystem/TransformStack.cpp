//
// Created by matherno on 20/11/17.
//

#include "TransformStack.h"

void TransformStack::push(const mathernogl::Transform* transform)
  {
  if (size() <= 0)
    {
    stack.emplace_back(*transform);
    }
  else
    {
    const mathernogl::Transform* top = getTop();
    const mathernogl::Matrix4* topMatrix = top->getTransformMatrix();
    const mathernogl::Matrix4* newMatrix = transform->getTransformMatrix();
    stack.emplace_back(mathernogl::Transform(*newMatrix * *topMatrix));
    }
  }

void TransformStack::pop()
  {
  if (size() > 0)
    stack.erase(--stack.end());
  }

mathernogl::Transform* TransformStack::getTop()
  {
  if (size() > 0)
    return &*(--stack.end());
  else
    return nullptr;
  }

void TransformStack::clear()
  {
  stack.clear();
  }

int TransformStack::size() const
  {
  return (int)stack.size();
  }
