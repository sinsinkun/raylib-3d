#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "shaded.hpp"

using namespace App;

void ShadedModel::init(Model m, Shader s) {
  model = m;
  shader = s;
  model.materials[0].shader = shader;
  
  // setup shader variable references
  shaderLoc[0] = GetShaderLocation(shader, "model");
  shaderLoc[1] = GetShaderLocation(shader, "view");
  shaderLoc[2] = GetShaderLocation(shader, "projection");
  shaderLoc[3] = GetShaderLocation(shader, "lightColor");
  shaderLoc[4] = GetShaderLocation(shader, "lightDir");
  shaderLoc[5] = GetShaderLocation(shader, "albedo");
}

void ShadedModel::update(const Camera& camera, int screenW, int screenH, float fovY) {
  double elapsed = GetTime();
  // convert fovY to rad
  float fovRad = fovY * 3.141592f / 180.0f;
  // update shader variables
  Matrix transMat = MatrixTranslate(pos.x, pos.y + std::cos(elapsed), pos.z + std::sin(elapsed));
  Matrix rotMat = MatrixRotateXYZ((Vector3){ (float)elapsed*0.5f, (float)elapsed*0.5f, 0.0f });
  Matrix modelPos = MatrixMultiply(rotMat, transMat);
  Matrix viewPos = GetCameraMatrix(camera);
  Matrix projection = MatrixPerspective(fovRad, (float)screenW/(float)screenH, 0.01f, 1000.0f);
  SetShaderValueMatrix(shader, shaderLoc[0], modelPos);
  SetShaderValueMatrix(shader, shaderLoc[1], viewPos);
  SetShaderValueMatrix(shader, shaderLoc[2], projection);
  SetShaderValue(shader, shaderLoc[3], lightC, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, shaderLoc[4], lightDir, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, shaderLoc[5], albedo, SHADER_UNIFORM_VEC3);
}

void ShadedModel::render() {
  DrawModel(model, pos, 1.0f, WHITE);
}

void ShadedModel::cleanup() {
  UnloadModel(model);
  UnloadShader(shader);
}