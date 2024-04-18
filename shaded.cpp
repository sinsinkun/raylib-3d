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
  _shaderLoc[0] = GetShaderLocation(shader, "model");
  _shaderLoc[1] = GetShaderLocation(shader, "view");
  _shaderLoc[2] = GetShaderLocation(shader, "projection");
  _shaderLoc[3] = GetShaderLocation(shader, "lightColor");
  _shaderLoc[4] = GetShaderLocation(shader, "lightDir");
  _shaderLoc[5] = GetShaderLocation(shader, "albedo");
}

void ShadedModel::updateModel(Vector3 dp, Vector3 dr) {
  pos = Vector3Add(pos, dp);
  rot = Vector3Add(rot, dr);
}

void ShadedModel::updateModel(Vector3 dp, Vector3 dr, Vector3 ld) {
  pos = Vector3Add(pos, dp);
  rot = Vector3Add(rot, dr);
  lightD = ld;
}

void ShadedModel::updateShader(const Camera& camera, int screenW, int screenH, float fovY) {
  double elapsed = GetTime();
  // update vertex shader
  float fovRad = fovY * 3.141592f / 180.0f;
  Vector3 rotRad = {rot.x * 3.141592f / 180.0f, rot.y * 3.141592f / 180.0f, rot.z * 3.141592f / 180.0f};
  Matrix transMat = MatrixTranslate(pos.x, pos.y, pos.z);
  Matrix rotMat = MatrixRotateXYZ(rotRad);
  Matrix modelPos = MatrixMultiply(rotMat, transMat);
  Matrix viewPos = GetCameraMatrix(camera);
  Matrix projection = MatrixPerspective(fovRad, (float)screenW/(float)screenH, 0.01f, 1000.0f);
  SetShaderValueMatrix(shader, _shaderLoc[0], modelPos);
  SetShaderValueMatrix(shader, _shaderLoc[1], viewPos);
  SetShaderValueMatrix(shader, _shaderLoc[2], projection);
  // update fragment shader
  float nlightc[3] = { (float)lightC.r / 255, (float)lightC.g / 255, (float)lightC.b / 255 };
  float nlightd[3] = { lightD.x, lightD.y, lightD.z};
  float nalbedo[3] = { (float)albedo.r / 255, (float)albedo.g / 255, (float)albedo.b / 255 };
  SetShaderValue(shader, _shaderLoc[3], nlightc, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[4], nlightd, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[5], nalbedo, SHADER_UNIFORM_VEC3);
}

void ShadedModel::render() {
  DrawModel(model, pos, 1.0f, WHITE);
}

void ShadedModel::cleanup() {
  UnloadModel(model);
  UnloadShader(shader);
}