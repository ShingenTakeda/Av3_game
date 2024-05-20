#include "raylib.h"
#include <Base/Game.hpp>

void Game::init()
{
  if(!file_exists("ini.lua"))
  {
    windowW = d_windowW;
    windowH = d_windowH;
  }

  InitWindow(windowW, windowH, "Game");
  
  cam = {0};
  cam.position = (Vector3){10.0f, 10.0f, 10.0f};
  cam.target = (Vector3){0.0f, 0.0f, 0.0f};
  cam.up = (Vector3){0.0f, 1.0f, 0.0f};
  cam.fovy = 45.0f;
  cam.projection = CAMERA_PERSPECTIVE;

  if(IsWindowReady())
  {
    is_running = true;
  }
  SetExitKey(KEY_NULL);
  g_lstate.open_libraries(sol::lib::base, sol::lib::os, sol::lib::utf8, sol::lib::math);

  DisableCursor();

  SetTargetFPS(60);
}


void Game::handle_input()
{
  if(IsKeyPressed(KEY_F2))
  {
    is_running = false;
  }
}

void Game::update()
{
  UpdateCamera(&cam, CAMERA_FREE);
}

void Game::draw()
{
  BeginDrawing();
  
  ClearBackground(RAYWHITE);

  BeginMode3D(cam);

    DrawCube(cube_pos, 2.0f, 2.0f, 2.0f, BLUE);

    DrawGrid(10.0f, 10);

  EndMode3D();

  EndDrawing();
}

void Game::run()
{
  SetTargetFPS(60);

  while(is_running)
  {
    handle_input();
    update();
    draw();
  }
  deinit();
}

void Game::deinit()
{
  CloseWindow();
}
