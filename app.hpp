#include <vector>
#include <raylib.h>
#include "shaded.hpp"

namespace App {
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
      // memory
      Camera camera = { 0 }; //3D camera
      Font font;
      std::vector<Asset> assets;
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