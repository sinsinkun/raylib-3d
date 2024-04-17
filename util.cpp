#include <iostream>
#include <math.h>
#include <raylib.h>
#include "util.hpp"

Vector2 Util::rotatePointOrigin(const float& x, const float& y, const float& rad) {
  float dx = x * std::cos(rad) - y * std::sin(rad);
  float dy = y * std::cos(rad) + x * std::sin(rad);
  return (Vector2){dx, dy};
}