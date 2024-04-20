#include <vector>
#include <raylib.h>
#include "shaded.hpp"

namespace App {
  const Color BG_CLEAR = {20, 20, 40, 255};
  enum AssetType {
    Asset_None,
    Asset_ShadedModel,
  };
  struct Asset {
    Asset(AssetType itype) {
      type = itype;
    }
    AssetType type = Asset_None;
    ShadedModel* sm;
  };

  class EventLoop {
    public:
      // global states
      int screenW = 0;
      int screenH = 0;
      Vector2 screenCenter = { 0.0, 0.0 };
      int fps = 0;
      float deltaTime = 0.0f;
      double elapsed = 0.0;
      Vector2 mousePos = { 0.0, 0.0 };
      bool paused = false;
      // memory
      Camera camera = { 0 }; //3D camera
      Font font;
      std::vector<Asset> assets;
      Vector3 lightPos = {5.0f, 10.0f, 0.0f};
      Shader blurShader;
      Shader intensityShader;
      RenderTexture2D preTexture;
      RenderTexture2D lightTexture;
      // methods
      void init();
      void update();
      void render();
      void cleanup();
    private:
      void _updateSystem();
      void _drawFps();
  };
}