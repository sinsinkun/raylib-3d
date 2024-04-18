#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include <raymath.h>
#include "app.hpp"
#include "util.hpp"
#include "shaded.hpp"

#define GLSL_VERSION 330

using namespace App;

void EventLoop::init() {
  // initialize font
  font = LoadFontEx("assets/roboto.ttf", 60, 0, 0);
  // Generate mipmap levels to use bi/trilinear filtering
  GenTextureMipmaps(&font.texture);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

  // initialize camera
  camera.position = (Vector3){ 10.0f, 6.0f, 0.0f };
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // initialize shadeded asset
  Asset a1 = { Asset_ShadedModel };
  a1.sm = new ShadedModel;
  Model m = LoadModelFromMesh(GenMeshCube(4.0f, 4.0f, 4.0f));
  Shader s = LoadShader("assets/lighting.vs", "assets/lighting.fs");
  a1.sm->pos = (Vector3){0.0f, 2.0f, 0.0f};
  a1.sm->init(m, s);
  assets.push_back(a1);
}

void EventLoop::update() {
  _updateSystem();
  // take inputs
  int mouseHoverCount = 0;
  bool mouseClicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

  // rotate camera
  int rotDir = 0;
  if (IsKeyDown(KEY_A)) rotDir = 1;
  if (IsKeyDown(KEY_D)) rotDir = -1;
  if (rotDir != 0) {
    Vector2 nPos = Util::rotatePointOrigin(camera.position.x, camera.position.z, rotDir * 0.02f);
    camera.position.x = nPos.x;
    camera.position.z = nPos.y;
  }
  if (IsKeyDown(KEY_W) && camera.position.y < 10.0f) camera.position.y += 0.1f;
  if (IsKeyDown(KEY_S) && camera.position.y > -5.0f) camera.position.y -= 0.1f;

  // update assets
  for (Asset a: assets) {
    switch (a.type) {
      case Asset_ShadedModel:
        a.sm->update(camera, screenW, screenH, camera.fovy);
        break;
      case Asset_None:
      default:
        break;
    }
  }

  // update mouse state
  if (mouseHoverCount > 0) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  else SetMouseCursor(MOUSE_CURSOR_ARROW);
}

void EventLoop::render() {
  BeginDrawing();
    ClearBackground(BLACK);
    if (IsWindowFocused()) {
      // draw background
      DrawRectangle(0, 0, screenW, screenH, (Color){40, 40, 60, 255});

      BeginMode3D(camera);
        // draw assets
        for (Asset a: assets) {
          switch (a.type) {
            case Asset_ShadedModel:
              a.sm->render();
              break;
            case Asset_None:
            default:
              break;
          }
        }
        // Debug
        // DrawCubeWires((Vector3){0.0f, 2.0f, 0.0f}, 4.0f, 4.0f, 4.0f, WHITE);
        // DrawSphere((Vector3){5.0f, 0.0f, 0.0f}, 0.1f, GREEN);
        // DrawGrid(10, 1.0f);
      EndMode3D();

    } else {
      DrawText("Pay Attention to me", screenCenter.x - 170, screenCenter.y - 40, 34, RED);
    }
    // draw FPS overlay
    _drawFps();
  EndDrawing();
}

void EventLoop::cleanup() {
  // destroy assets
  for (Asset a: assets) {
    switch (a.type) {
      case Asset_ShadedModel:
        a.sm->cleanup();
        delete a.sm;
        break;
      case Asset_None:
      default:
        break;
    }
  }
  UnloadFont(font);
}

void EventLoop::_updateSystem() {
  fps = GetFPS();
  screenW = GetScreenWidth();
  screenH = GetScreenHeight();
  elapsed = GetTime();
  screenCenter = { (float)screenW/2, (float)screenH/2 };
  mousePos = GetMousePosition();
  deltaTime = GetFrameTime();
}

void EventLoop::_drawFps() {
  std::string fpst = std::to_string(fps);
  std::string fpstxt = "FPS: ";
  fpstxt.append(fpst);
  DrawTextEx(font, fpstxt.c_str(), (Vector2){2.0, 2.0}, 20, 0, GREEN);
}
