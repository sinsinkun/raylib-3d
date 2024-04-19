#include <iostream>
#include <random>
#include <math.h>
#include <raylib.h>
#include "util.hpp"

Vector2 Util::rotatePointOrigin(const float& x, const float& y, const float& rad) {
  float dx = x * std::cos(rad) - y * std::sin(rad);
  float dy = y * std::cos(rad) + x * std::sin(rad);
  return (Vector2){dx, dy};
}

int Util::randomNumber(const int min, const int max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  return dist(rng);
};