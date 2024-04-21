#pragma once

#include <raylib.h>

namespace App {
  class ShadedModel {
    public:
      // assets
      Model model;
      Shader shader;
      Vector3 pos = {0.0f, 0.0f, 0.0f};
      Vector3 rot = {0.0f, 0.0f, 0.0f}; // in deg
      Color albedo = {255, 255, 255};
      Color lightC = {250, 250, 250};
      Vector3 lightP = {0.0f, 0.0f, 0.0f}; // direction, no magnitude
      // methods
      void init(Model m, Shader s);
      void updateModel(Vector3 dposition, Vector3 drotation);
      void updateModel(Vector3 dposition, Vector3 drotation, Vector3 lightDir);
      void updateShader(const Camera& camera, int screenW, int screenH, float fovY);
      void render();
      void cleanup();
    private:
      int _shaderLoc[6];
  };
}