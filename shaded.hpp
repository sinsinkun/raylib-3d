#pragma once

#include <raylib.h>

namespace App {
  class ShadedModel {
    public:
      // assets
      Model model;
      Shader shader;
      Vector3 pos = {0.0f, 0.0f, 0.0f};
      int shaderLoc[5];
      float lightC[3] = {0.4f, 0.4f, 0.5f}; // normalized
      float lightDir[3] = {7.0f, 10.0f, 3.0f};
      // methods
      void init(Model m, Shader s);
      void update(const Camera& camera, int screenW, int screenH, float fovY);
      void render();
      void cleanup();
    private:
  };
}