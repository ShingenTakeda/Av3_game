#pragma once
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

  int windowW;
  int windowH;

  bool debug = false;
  bool is_running = false;
};

inline bool file_exists(const std::string &name)
{
  return std::filesystem::exists(name);
}
