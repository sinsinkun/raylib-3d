#pragma once
#include <iostream>
#include <raylib.h>

namespace App {
  
  enum LightType {
    Light_Point,
    Light_Directional,
    Light_Spot
  };

  class Light {
    public:
      /// @brief specify point light values
      Light(int lid, LightType ltype, Vector3 lpos, Color lcolor, float influence, bool lvisible) {
        if (ltype != Light_Point) {
          std::cout << "Wrong light initializer" << ltype << std::endl;
        }
        id = lid;
        type = ltype;
        pos = lpos;
        color = lcolor;
        visible = lvisible;
        calcAttenuation(influence);
        render_cache = LoadModelFromMesh(GenMeshSphere(0.1, 10, 10));
      }
      /// @brief specify directional light values
      Light(int lid, LightType ltype, Vector3 ldir, Color lcolor, float lint) {
        if (ltype != Light_Directional) {
          std::cout << "Wrong light initializer" << ltype << std::endl;
        }
        id = lid;
        type = ltype;
        direction = ldir;
        color = lcolor;
        intensity = lint;
      }
      /// @brief specify spot light values
      Light(int lid, LightType ltype, Vector3 lpos, Vector3 ldir, Color lcolor, float influence, float coa, bool lvisible) {
        if (ltype != Light_Spot) {
          std::cout << "Wrong light initializer" << ltype << std::endl;
        }
        id = lid;
        type = ltype;
        pos = lpos;
        direction = ldir;
        color = lcolor;
        cutOffAngle = coa;
        visible = lvisible;
        calcAttenuation(influence);
        render_cache = LoadModelFromMesh(GenMeshSphere(0.1, 10, 10));
      }
      // properties
      LightType type = Light_Point;
      Vector3 pos = {0.0f, 0.0f, 0.0f};
      Vector3 direction = {1.0f, 0.0f, 0.0f};
      Color color = {0, 0, 0};
      int id = 0;
      float attenL = 0.2f;
      float attenQ = 0.2f;
      float intensity = 0.0f;
      float cutOffAngle = 0.0f; // in radians
      bool visible = false;
      // methods
      void update(Vector3 npos);
      void update(Vector3 dpos, Vector3 ddir);
      void render();
      void cleanup();
      void calcAttenuation(float dist);
      void shaderColor(float (&arr)[3]);
      void shaderPos(float (&arr)[3]);
      void shaderDir(float (&arr)[3]);
    private:
      Model render_cache;
  };
}