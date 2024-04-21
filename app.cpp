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
  camera.position = (Vector3){ 10.0f, 16.0f, 0.0f };
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // initialize shadeded asset
  Asset a1 = { Asset_ShadedModel };
  a1.sm = new ShadedModel;
  Model m = LoadModelFromMesh(GenMeshCone(2.0f, 2.0f, 40));
  Shader s = LoadShader("assets/lighting.vs", "assets/lighting.fs");
  a1.sm->pos = (Vector3){0.0f, 2.0f, 0.0f};
  a1.sm->albedo = {255, 140, 140};
  a1.sm->init(m, s);
  assets.push_back(a1);

  // asset 2
  Asset a2 = { Asset_ShadedModel };
  a2.sm = new ShadedModel;
  Model m2 = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
  Shader s2 = LoadShader("assets/lighting.vs", "assets/lighting.fs");
  a2.sm->pos = (Vector3){0.0f, 2.0f, 5.0f};
  a2.sm->albedo = {140, 200, 150};
  a2.sm->init(m2, s2);
  assets.push_back(a2);

  // asset 3
  Asset a3 = { Asset_ShadedModel };
  a3.sm = new ShadedModel;
  Model m3 = LoadModelFromMesh(GenMeshSphere(2.0f, 40, 40));
  Shader s3 = LoadShader("assets/lighting.vs", "assets/lighting.fs");
  a3.sm->pos = (Vector3){0.0f, 2.0f, -5.0f};
  a3.sm->albedo = {140, 120, 255};
  a3.sm->init(m3, s3);
  assets.push_back(a3);

  // post process shader
  intensityShader = LoadShader(0, "assets/intensityFilter.fs");
  blurShader = LoadShader(0, "assets/blur.fs");
  preTexture = LoadRenderTexture(800, 600);
  SetTextureWrap(preTexture.texture, TEXTURE_WRAP_CLAMP);
  lightTexture = LoadRenderTexture(800, 600);
  SetTextureWrap(lightTexture.texture, TEXTURE_WRAP_CLAMP);
}

void EventLoop::update() {
  _updateSystem();
  // take inputs
  int mouseHoverCount = 0;
  bool mouseClicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
  float ftime = GetFrameTime();

  // rotate camera
  int rotDir = 0;
  if (IsKeyDown(KEY_A)) rotDir = 1;
  if (IsKeyDown(KEY_D)) rotDir = -1;
  if (rotDir != 0) {
    Vector2 nPos = Util::rotatePointOrigin(camera.position.x, camera.position.z, rotDir * 0.02f);
    camera.position.x = nPos.x;
    camera.position.z = nPos.y;
  }
  if (IsKeyDown(KEY_W) && camera.position.y < 25.0f) camera.position.y += 0.1f;
  if (IsKeyDown(KEY_S) && camera.position.y > -10.0f) camera.position.y -= 0.1f;

  // toggle rotation
  if (IsKeyPressed(KEY_SPACE)) paused = !paused;

  // update lights
  if (!paused) {
    lightPos = {5.0f * (float)std::sin(elapsed), 6.0f, 5.0f * (float)std::cos(elapsed)};
  }

  // update post processor
  if (preTexture.texture.width != screenW || preTexture.texture.height != screenH) {
    UnloadRenderTexture(preTexture);
    preTexture = LoadRenderTexture(screenW, screenH);
    SetTextureWrap(preTexture.texture, TEXTURE_WRAP_CLAMP);
    UnloadRenderTexture(lightTexture);
    lightTexture = LoadRenderTexture(screenW, screenH);
    SetTextureWrap(lightTexture.texture, TEXTURE_WRAP_CLAMP);
  }

  // update assets
  for (Asset a: assets) {
    switch (a.type) {
      case Asset_ShadedModel:
        a.sm->updateModel((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, ftime * -12.0f, 0.0f}, lightPos);
        a.sm->updateShader(camera, screenW, screenH, camera.fovy);
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
  // pre-processing
  _preRender();
  _addBloom();
  // make render
  BeginDrawing();
    ClearBackground(BLACK);
    // draw post processed texture
    DrawTextureRec(
      preTexture.texture, 
      (Rectangle){0, 0, (float)preTexture.texture.width, (float)-preTexture.texture.height},
      (Vector2){0, 0},
      WHITE
    );
    // combine light blur for bloom effect
    BeginBlendMode(BLEND_ADD_COLORS);
      DrawTextureRec(
        lightTexture.texture, 
        (Rectangle){0, 0, (float)lightTexture.texture.width, (float)-lightTexture.texture.height},
        (Vector2){0, 0},
        WHITE
      );
    EndBlendMode();
    // debug textures
    _debugRender();
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
  UnloadShader(blurShader);
  UnloadShader(intensityShader);
  UnloadRenderTexture(preTexture);
  UnloadRenderTexture(lightTexture);
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

#pragma region Render Helpers
void EventLoop::_drawFps() {
  std::string fpst = std::to_string(fps);
  std::string fpstxt = "FPS: ";
  fpstxt.append(fpst);
  DrawTextEx(font, fpstxt.c_str(), (Vector2){2.0, 2.0}, 20, 0, GREEN);
}

void EventLoop::_preRender() {
  BeginTextureMode(preTexture);
    ClearBackground(BG_CLEAR);
    
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
      DrawSphere(lightPos, 0.1f, WHITE);
      // DrawGrid(10, 1.0f);
    EndMode3D();
  EndTextureMode();
}

void EventLoop::_addBloom() {
  // filter out high intensity areas
  BeginTextureMode(lightTexture);
    BeginShaderMode(intensityShader);
      DrawTextureRec(
        preTexture.texture, 
        (Rectangle){0, 0, (float)preTexture.texture.width, (float)-preTexture.texture.height},
        (Vector2){0, 0},
        WHITE
      );
    EndShaderMode();
  EndTextureMode();

  // blur intensity
  BeginTextureMode(lightTexture);
    BeginShaderMode(blurShader);
      DrawTextureRec(
        lightTexture.texture, 
        (Rectangle){0, 0, (float)lightTexture.texture.width, (float)-lightTexture.texture.height},
        (Vector2){0, 0},
        WHITE
      );
    EndShaderMode();
  EndTextureMode();
}

void EventLoop::_debugRender() {
  DrawTexturePro(
    preTexture.texture, 
    (Rectangle){0, 0, (float)preTexture.texture.width, (float)-preTexture.texture.height},
    (Rectangle){0, 0, (float)screenW/4, (float)screenH/4},
    (Vector2){0, 0},
    0,
    WHITE
  );
  DrawTexturePro(
    lightTexture.texture, 
    (Rectangle){0, 0, (float)lightTexture.texture.width, (float)-lightTexture.texture.height},
    (Rectangle){0, (float)screenH/4, (float)screenW/4, (float)screenH/4},
    (Vector2){0, 0},
    0,
    WHITE
  );
}
#pragma endregion Render Helpers