#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "shaded.hpp"

using namespace App;

void ShadedModel::init(Model m, Vector3 position, Material mat) {
  model = m;
  pos = position;
  shader = LoadShader("assets/phong.vs", "assets/phong.fs");
  model.materials[0].shader = shader;
  
  // setup shader variable references
  _shaderLoc[0] = GetShaderLocation(shader, "model");
  _shaderLoc[1] = GetShaderLocation(shader, "view");
  _shaderLoc[2] = GetShaderLocation(shader, "projection");
  _shaderLoc[3] = GetShaderLocation(shader, "viewPos");
  _shaderLoc[4] = GetShaderLocation(shader, "material.albedo");
  _shaderLoc[5] = GetShaderLocation(shader, "material.ambience");
  _shaderLoc[6] = GetShaderLocation(shader, "material.specularity");
  _shaderLoc[7] = GetShaderLocation(shader, "material.shininess");
  _shaderLoc[8] = GetShaderLocation(shader, "material.bands");
  _shaderLoc[9] = GetShaderLocation(shader, "material.diffusivity");
  _shaderLoc[10] = GetShaderLocation(shader, "light1.color");
  _shaderLoc[11] = GetShaderLocation(shader, "light1.position");
  _shaderLoc[12] = GetShaderLocation(shader, "light1.attL");
  _shaderLoc[13] = GetShaderLocation(shader, "light1.attQ");
  _shaderLoc[14] = GetShaderLocation(shader, "light2.color");
  _shaderLoc[15] = GetShaderLocation(shader, "light2.position");
  _shaderLoc[16] = GetShaderLocation(shader, "light2.attL");
  _shaderLoc[17] = GetShaderLocation(shader, "light2.attQ");
  _shaderLoc[18] = GetShaderLocation(shader, "light0.color");
  _shaderLoc[19] = GetShaderLocation(shader, "light0.direction");
  _shaderLoc[20] = GetShaderLocation(shader, "light0.intensity");

  // setup material properties
  float nalbedo[3] = { (float)mat.albedo.r / 255, (float)mat.albedo.g / 255, (float)mat.albedo.b / 255 };
  SetShaderValue(shader, _shaderLoc[4], nalbedo, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[5], &mat.ambience, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[6], &mat.specularity, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[7], &mat.shininess, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[8], &mat.bands, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[9], &mat.diffusivity, SHADER_UNIFORM_FLOAT);

  // setup light properties
  float attenuationL = 0.2f;
  float attenuationQ = 0.2f;
  SetShaderValue(shader, _shaderLoc[12], &attenuationL, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[13], &attenuationQ, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[16], &attenuationL, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[17], &attenuationQ, SHADER_UNIFORM_FLOAT);
}

void ShadedModel::updateModel(Vector3 dp, Vector3 dr) {
  pos = Vector3Add(pos, dp);
  rot = Vector3Add(rot, dr);
}

void ShadedModel::updateModel(Vector3 dp, Vector3 dr, Vector3 ld, Color lc) {
  pos = Vector3Add(pos, dp);
  rot = Vector3Add(rot, dr);
  lightP = ld;
  lightC = lc;
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
  float viewp[3] = { camera.position.x, camera.position.y, camera.position.z };
  float nlightc[3] = { (float)lightC.r / 255, (float)lightC.g / 255, (float)lightC.b / 255 };
  float nlightd[3] = { lightP.x, lightP.y, lightP.z};
  float nlightd2[3] = { -lightP.x, lightP.y, -lightP.z };
  SetShaderValue(shader, _shaderLoc[3], viewp, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[10], nlightc, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[11], nlightd, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[14], nlightc, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[15], nlightd2, SHADER_UNIFORM_VEC3);
}

void ShadedModel::render() {
  DrawModel(model, pos, 1.0f, WHITE);
}

void ShadedModel::cleanup() {
  UnloadModel(model);
  UnloadShader(shader);
}