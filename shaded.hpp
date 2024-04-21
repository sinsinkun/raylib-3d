#pragma once

#include <raylib.h>

namespace App {

  /// @brief Define material properties for model
  /// @param albedo base color of material
  /// @param ambience Ambient brightness (0.0 - 1.0)
  /// @param specularity Strength of specular reflection (0.0 - 1.0)
  /// @param shininess Reflectiveness of surface (power of 2)
  /// @param bands Number of bands to divide shades into (0 or 2+)
  struct Material {
    Color albedo = {100, 100, 100};
    float ambience = 0.2f;
    float specularity = 0.5f;
    float shininess = 32.0f;
    float bands = 0.0f;
  };

  /// @brief Model with basic shading properties applied
  class ShadedModel {
    public:
      // assets
      Model model;
      Shader shader;
      Vector3 pos = {0.0f, 0.0f, 0.0f};
      Vector3 rot = {0.0f, 0.0f, 0.0f}; // in deg
      Color lightC = {250, 250, 250};
      Vector3 lightP = {0.0f, 0.0f, 0.0f};
      // methods
      void init(Model model, Vector3 position, Material material);
      void updateModel(Vector3 dposition, Vector3 drotation);
      void updateModel(Vector3 dposition, Vector3 drotation, Vector3 lightDir);
      void updateShader(const Camera& camera, int screenW, int screenH, float fovY);
      void render();
      void cleanup();
    private:
      int _shaderLoc[10];
  };
}