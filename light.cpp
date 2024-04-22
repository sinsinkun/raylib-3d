#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "light.hpp"

using namespace App;

void Light::update(Vector3 npos) {
  pos = npos;
}

void Light::update(Vector3 dpos, Vector3 ddir) {
  pos = Vector3Add(pos, dpos);
  direction = Vector3Add(direction, ddir);
}

void Light::render() {
  if (!visible) return;
  DrawModel(render_cache, pos, 1.0f, color);
}

void Light::cleanup() {
  if (IsModelReady(render_cache)) {
    UnloadModel(render_cache);
  }
}

void Light::calcAttenuation(float dist) {
  if (dist < 10.0f) {
    attenL = 0.8;
    attenQ = 1.8;
  } else if (dist < 20.0f) {
    attenL = 0.2;
    attenQ = 0.2;
  } else if (dist < 50.0f) {
    attenL = 0.1;
    attenQ = 0.03;
  } else if (dist < 100.0f) {
    attenL = 0.05;
    attenQ = 0.008;
  } else if (dist < 200.0f) {
    attenL = 0.02;
    attenQ = 0.002;
  } else if (dist < 500.0f) {
    attenL = 0.01;
    attenQ = 0.0003;
  } else {
    attenL = 0.001f;
    attenQ = 0.00001f;
  }
}

void Light::shaderColor(float (&arr)[3]) {
  arr[0] = (float)color.r / 255;
  arr[1] = (float)color.g / 255;
  arr[2] = (float)color.b / 255;
}

void Light::shaderPos(float (&arr)[3]) {
  arr[0] = pos.x;
  arr[1] = pos.y;
  arr[2] = pos.z;
}

void Light::shaderDir(float (&arr)[3]) {
  arr[0] = direction.x;
  arr[1] = direction.y;
  arr[2] = direction.z;
}