#include <GUI_Editor/GUI.hpp>
#include "raylib.h"
#include <Base/app.hpp>

void App::init()
{
  InitWindow(960, 720, "Editor");

  if(IsWindowReady())
  {
    is_running = true;
  }

  SetExitKey(KEY_NULL);
  lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::utf8, sol::lib::math);

  SetTargetFPS(60);
}

void App::handle_input()
{
  if(IsKeyPressed(KEY_F2))
  {
    is_running = false;
  }
}

void App::update()
{
}

void App::draw()
{
  BeginDrawing();
  
  ClearBackground(RAYWHITE);
  
  GuiPanel((Rectangle){ 370, 25, 225, 140 }, "Menu");
  EndDrawing();
}

void App::run()
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

void App::deinit()
{
  CloseWindow();
}
