#include <vector>
#include <raylib.h>

namespace App {
  enum AssetType {
    ANone,
  };
  struct Asset {
    AssetType type = AssetType::ANone;
  };

  class EventLoop {
    public:
      // global states
      Camera camera = { 0 }; //3D camera
      Shader shader;
      Font font;
      int screenW = 0;
      int screenH = 0;
      Vector2 screenCenter = { 0.0, 0.0 };
      int fps = 0;
      float deltaTime = 0.0f;
      double elapsed = 0.0;
      Vector2 mousePos = { 0.0, 0.0 };
      // memory
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