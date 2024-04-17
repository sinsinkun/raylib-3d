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
  shader = LoadShader("assets/lighting.vs", "assets/lighting.fs");
  model = LoadModelFromMesh(GenMeshCube(4.0f, 4.0f, 4.0f));
  model.materials[0].shader = shader;
  
  // setup shader variable references
  shaderLoc[0] = GetShaderLocation(shader, "model");
  shaderLoc[1] = GetShaderLocation(shader, "view");
  shaderLoc[2] = GetShaderLocation(shader, "projection");
  shaderLoc[3] = GetShaderLocation(shader, "lightColor");
  shaderLoc[4] = GetShaderLocation(shader, "lightDir");
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

  // update shader variables
  Matrix modelPos = MatrixTranslate(0.0f, 2.0f, 0.0f);
  Matrix viewPos = GetCameraMatrix(camera);
  Matrix projection = MatrixPerspective(1.0472, (float)screenW/(float)screenH, 0.1f, 1000.0f);
  SetShaderValueMatrix(shader, shaderLoc[0], modelPos);
  SetShaderValueMatrix(shader, shaderLoc[1], viewPos);
  SetShaderValueMatrix(shader, shaderLoc[2], projection);
  SetShaderValue(shader, shaderLoc[3], lightC, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, shaderLoc[4], lightDir, SHADER_UNIFORM_VEC3);

  // update mouse state
  if (mouseHoverCount > 0) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  else SetMouseCursor(MOUSE_CURSOR_ARROW);
}

void EventLoop::render() {
  BeginDrawing();
    ClearBackground(BLACK);
    if (IsWindowFocused()) {
      // draw background
      DrawRectangle(0, 0, screenW, screenH, (Color){80, 120, 120, 255});

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
        DrawModel(model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        // Debug
        DrawCubeWires((Vector3){0.0f, 2.0f, 0.0f}, 4.0f, 4.0f, 4.0f, WHITE);
        DrawSphere((Vector3){5.0f, 0.0f, 0.0f}, 0.1f, GREEN);
        DrawGrid(10, 1.0f);
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
  assets.clear();
  UnloadModel(model);
  UnloadShader(shader);
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
