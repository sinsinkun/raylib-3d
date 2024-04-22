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
  _shaderLoc[10] = GetShaderLocation(shader, "light0.color");
  _shaderLoc[11] = GetShaderLocation(shader, "light0.direction");
  _shaderLoc[12] = GetShaderLocation(shader, "light0.intensity");
  _shaderLoc[13] = GetShaderLocation(shader, "light1.color");
  _shaderLoc[14] = GetShaderLocation(shader, "light1.position");
  _shaderLoc[15] = GetShaderLocation(shader, "light1.attL");
  _shaderLoc[16] = GetShaderLocation(shader, "light1.attQ");
  _shaderLoc[17] = GetShaderLocation(shader, "light2.color");
  _shaderLoc[18] = GetShaderLocation(shader, "light2.position");
  _shaderLoc[19] = GetShaderLocation(shader, "light2.attL");
  _shaderLoc[20] = GetShaderLocation(shader, "light2.attQ");
  _shaderLoc[21] = GetShaderLocation(shader, "light3.color");
  _shaderLoc[22] = GetShaderLocation(shader, "light3.position");
  _shaderLoc[23] = GetShaderLocation(shader, "light3.attL");
  _shaderLoc[24] = GetShaderLocation(shader, "light3.attQ");
  _shaderLoc[25] = GetShaderLocation(shader, "light4.color");
  _shaderLoc[26] = GetShaderLocation(shader, "light4.position");
  _shaderLoc[27] = GetShaderLocation(shader, "light4.attL");
  _shaderLoc[28] = GetShaderLocation(shader, "light4.attQ");

  // setup material properties
  float nalbedo[3] = { (float)mat.albedo.r / 255, (float)mat.albedo.g / 255, (float)mat.albedo.b / 255 };
  SetShaderValue(shader, _shaderLoc[4], nalbedo, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, _shaderLoc[5], &mat.ambience, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[6], &mat.specularity, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[7], &mat.shininess, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[8], &mat.bands, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, _shaderLoc[9], &mat.diffusivity, SHADER_UNIFORM_FLOAT);
}

void ShadedModel::updateModel(Vector3 dp, Vector3 dr) {
  pos = Vector3Add(pos, dp);
  rot = Vector3Add(rot, dr);
}

void ShadedModel::updateShader(const Camera& camera, int screenW, int screenH, std::vector<Light>& lights) {
  double elapsed = GetTime();
  // update vertex shader
  float fovRad = camera.fovy * 3.141592f / 180.0f;
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
  SetShaderValue(shader, _shaderLoc[3], viewp, SHADER_UNIFORM_VEC3);

  // update lights
  int dCount = 0, pCount = 0, sCount = 0;
  for (Light l: lights) {
    switch (l.type) {
      case Light_Directional:
        if (dCount < 1) {
          dCount++;
          float a[3];
          float b[3];
          l.shaderColor(a);
          l.shaderDir(b);
          SetShaderValue(shader, _shaderLoc[10], &a, SHADER_UNIFORM_VEC3);
          SetShaderValue(shader, _shaderLoc[11], &b, SHADER_UNIFORM_VEC3);
          SetShaderValue(shader, _shaderLoc[12], &l.intensity, SHADER_UNIFORM_FLOAT);
        } else {
          std::cout << "WARNING: reached maximum directional light limit" << std::endl;
        }
        break;
      case Light_Point:
        if (pCount < 4) {
          pCount++;
          float a[3];
          float b[3];
          l.shaderColor(a);
          l.shaderPos(b);
          SetShaderValue(shader, _shaderLoc[13 + pCount*4], &a, SHADER_UNIFORM_VEC3);
          SetShaderValue(shader, _shaderLoc[14 + pCount*4], &b, SHADER_UNIFORM_VEC3);
          SetShaderValue(shader, _shaderLoc[15 + pCount*4], &l.attenL, SHADER_UNIFORM_FLOAT);
          SetShaderValue(shader, _shaderLoc[16 + pCount*4], &l.attenQ, SHADER_UNIFORM_FLOAT);
        } else {
          std::cout << "WARNING: reached maximum point light limit" << std::endl;
        }
        break;
      case Light_Spot:
        if (sCount < 2) {
          sCount++;
        } else {
          std::cout << "WARNING: reached maximum spot light limit" << std::endl;
        }
        break;
      default:
        break;
    }
  }
}

void ShadedModel::render() {
  DrawModel(model, pos, 1.0f, WHITE);
}

void ShadedModel::cleanup() {
  UnloadModel(model);
  UnloadShader(shader);
}