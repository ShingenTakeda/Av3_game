#include <Base/Game.hpp>

void Game::init()
{
  if(!file_exists("ini.lua"))
  {
    windowW = d_windowW;
    windowH = d_windowH;
  }

  InitWindow(windowW, windowH, "Game");
  
  if(IsWindowReady())
  {
    is_running = true;
  }
  SetExitKey(KEY_NULL);
  g_lstate.open_libraries(sol::lib::base, sol::lib::os, sol::lib::utf8, sol::lib::math);
}


void Game::handle_input()
{
  if(IsKeyPressed(KEY_F2))
  {
    is_running = false;
  }
}

void Game::update()
{}

void Game::draw()
{
  BeginDrawing();
  ClearBackground(MAGENTA);
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
