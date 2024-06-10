#include "Base/Helpers.hpp"
#include "ECS/ECS.hpp"
#include "raylib.h"
#include <sol/sol.hpp>
#include "sol/error.hpp"
#include "sol/load_result.hpp"
#include "sol/optional_implementation.hpp"
#include <Base/Game.hpp>
#include <fmtlog/fmtlog.h>
#include <LuaHelpers/LHelpers.hpp>
#include <Systems/Systems.hpp>
#include <MessageBus/MessageBus.h>


void Game::init()
{

  g_lstate.open_libraries(sol::lib::base, sol::lib::os, sol::lib::utf8, sol::lib::math);
  if(!file_exists("ini.lua"))
  {
    windowW = d_windowW;
    windowH = d_windowH;
    FMTLOG(fmtlog::INF, "ini.lua not found!");
  }
  else
  {
    g_lstate.script_file("ini.lua");
    sol::table config = g_lstate["config"];
  
    if(!config.valid())
    {
      FMTLOG(fmtlog::WRN,"Error loading the lua script");
      windowW = d_windowW;
      windowH = d_windowH;
      FMTLOG(fmtlog::INF,"Window width and height: {} | {}", windowW, windowH);
    }
    else
    {
      FMTLOG(fmtlog::INF, "Config script loaded!");
      windowW = config["width"];
      windowH = config["height"];
      FMTLOG(fmtlog::INF,"Window width and height: {} | {}", windowW, windowH);
    }
  }

  InitWindow(windowW, windowH, "Game");
  InitAudioDevice();

  if(IsWindowReady() && IsAudioDeviceReady())
  {
    is_running = true;
  }
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  DisableCursor();
  
  fmtlog::poll();
}

void Game::handle_input()
{
  
}

void Game::update()
{
  
}

void Game::draw()
{
  BeginDrawing();
  
  ClearBackground(MAGENTA);

  if(scene_running == false)
  {
    DrawText("No scene loaded!", windowW / 2 - 100, windowH / 2, 50, RAYWHITE);
  }
  EndDrawing();
}

void Game::run()
{
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
  CloseAudioDevice();
  CloseWindow();
}

void Game::scene_loader(std::string scene)
{
  world.clear();
  sol::load_result script = g_lstate.load_file("./scenes/" + scene + ".lua");

  if(!script.valid())
  {
    sol::error err = script;
    std::string err_msg = err.what();
    FMTLOG(fmtlog::ERR, err_msg);
    fmtlog::poll();
    return;
  }
  
  sol::table scene_table = g_lstate["scene"];
  
  sol::table entities = scene_table["entities"];
  int i = 0;
  while(true)
  {
    sol::optional<sol::table> hasEntity = entities[i];
    
    const auto entity = world.create();
    sol::optional<sol::table> has_components = entities["components"];
    {
      
    }
  }
}
