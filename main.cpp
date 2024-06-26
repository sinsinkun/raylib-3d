#include <iostream>
#include <raylib.h>
#include "app.hpp"

/// @brief Entry point
/// @param argc arguments count
/// @param argv arguments array
/// @return exit code
int main(int argc, char* argv[]) {
  // --- INITIALIZATION ---
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(800, 600, "3D Test");
  // InitAudioDevice();
  SetWindowMinSize(400, 300);
  Image icon = LoadImage("assets/logo.png");
  SetWindowIcon(icon);
  SetTargetFPS(120);
  // SetMasterVolume(0.2);

  App::EventLoop e;
  e.init();

  // --- EVENT LOOP ---
  std::cout << "\n -- << -- Starting -- >> -- \n" << std::endl;
  while (!WindowShouldClose()) {
    e.update();
    e.render();
  }
  std::cout << "\n -- << -- Exiting -- >> -- \n" << std::endl;

  // --- CLEAN UP ---
  // CloseAudioDevice();
  e.cleanup();
  CloseWindow();
}
