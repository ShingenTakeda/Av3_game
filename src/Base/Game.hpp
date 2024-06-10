#pragma once
#include <filesystem>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <ECS/ECS.hpp>
#include <MediaStore/MediaStorage.hpp>
struct Game
{
  void run();

  void init();
  void setup();
  void handle_input();
  void update();
  void draw();
  void deinit();

  void scene_loader(std::string scene);

  sol::state g_lstate;

  int windowW;
  int windowH;
  int vResolutionW;
  int vResolutionH;

  std::unique_ptr<MediaStorage> media_storage;

  bool debug = false;
  bool is_running = false;
  bool scene_running = false;
  ECSWorld world;
};

inline bool file_exists(const std::string &name)
{
  return std::filesystem::exists(name);
}
