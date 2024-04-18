#pragma once

#include <raylib.h>

namespace App {
  class ShadedModel {
    public:
      // assets
      Model model;
      Shader shader;
      Vector3 pos = {0.0f, 0.0f, 0.0f};
      Vector3 rot = {30.0f, 60.0f, 0.0f}; // in deg
      Color albedo = {255, 255, 255};
      Color lightC = {100, 100, 160};
      Vector3 lightD = {3.0f, 5.0f, 1.0f}; // direction, no magnitude
      // methods
      void init(Model m, Shader s);
      void updateModel(Vector3 position, Vector3 rotation);
      void updateShader(const Camera& camera, int screenW, int screenH, float fovY);
      void render();
      void cleanup();
    private:
      int _shaderLoc[6];
  };
}