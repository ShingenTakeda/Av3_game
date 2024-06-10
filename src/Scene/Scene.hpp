#pragma once

#include <raylib.h>
#include <Base/Game.hpp>
struct Scene2D
{
  void init();
  void handle_input();
  void draw();
  void update();
  void deinit();

  Camera2D camera;
  Game *game;
};

struct Scene3D
{
  void init();
  void handle_input();
  void draw();
  void update();
  void deinit();

  Camera3D camera;
  Game *game;
};
