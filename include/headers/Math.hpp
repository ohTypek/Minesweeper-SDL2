#pragma once

#include<iostream>

class Vector2f {
public:
  float x;
  float y;

  Vector2f(float x, float y) : x(x), y(y) {}
  void print() {
    std::cout << "X: " << x << " Y: " << y << std::endl;
  }
};