#pragma once
#include "ECS/ECS.hpp"
#include <filesystem>
#include <raylib.h>
#include <sol/sol.hpp>
#include <string>

struct Game
{
  void run();

  void init();
  void setup();
  void handle_input();
  void update();
  void draw();
  void deinit();

  sol::state g_lstate;

  static constexpr int d_windowW = 720;
  static constexpr int d_windowH = 480;

  Camera3D cam;
  Vector3 cube_pos = {0.0f, 0.0f, 0.0f};

  int windowW;
  int windowH;
  int vResolutionW;
  int vResolutionH;

  bool debug = false;
  bool is_running = false;
  ECSWorld world;
};

inline bool file_exists(const std::string &name)
{
  return std::filesystem::exists(name);
}
