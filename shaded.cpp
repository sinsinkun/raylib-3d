#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "shaded.hpp"

using namespace App;

void ShadedModel::init() {
  model.materials[0].shader = shader;
  
  // setup shader variable references
  shaderLoc[0] = GetShaderLocation(shader, "model");
  shaderLoc[1] = GetShaderLocation(shader, "view");
  shaderLoc[2] = GetShaderLocation(shader, "projection");
  shaderLoc[3] = GetShaderLocation(shader, "lightColor");
  shaderLoc[4] = GetShaderLocation(shader, "lightDir");
}

void ShadedModel::update(const Camera& camera, int screenW, int screenH, float fovY) {
  // convert fovY to rad
  float fovRad = fovY * 3.141592f / 180.0f;
  // update shader variables
  Matrix modelPos = MatrixTranslate(pos.x, pos.y, pos.z);
  Matrix viewPos = GetCameraMatrix(camera);
  Matrix projection = MatrixPerspective(fovRad, (float)screenW/(float)screenH, 0.01f, 1000.0f);
  SetShaderValueMatrix(shader, shaderLoc[0], modelPos);
  SetShaderValueMatrix(shader, shaderLoc[1], viewPos);
  SetShaderValueMatrix(shader, shaderLoc[2], projection);
  SetShaderValue(shader, shaderLoc[3], lightC, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, shaderLoc[4], lightDir, SHADER_UNIFORM_VEC3);
}

void ShadedModel::render() {
  if (IsShaderReady(shader)) {
    DrawModel(model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
  } else {
    std::cout << "ERR: shader is not ready" << std::endl;
    exit(-10);
  }
}

void ShadedModel::cleanup() {
  UnloadModel(model);
  UnloadShader(shader);
}