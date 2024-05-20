#pragma once
#include <raylib.h>
#include <sol/sol.hpp>
#include <GUI/GUI.hpp>

struct App
{
  void init();
  void deinit();
  void run();

  void handle_input();
  void update();
  void draw();

  sol::state lua;
  Rectangle window;

  bool is_running = false;
};
