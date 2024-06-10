#pragma once
#include <map>
#include <string>
#include <Base/Helpers.hpp>
struct MediaStorage
{
  void ClearAssets();
  void AddTexture();
  void AddImage();
  void AddSounds();
  void AddMeshes();
  void AddText();

  void ImageToTexture();

  std::map<std::string, Texture> textures;
  std::map<std::string, Image> images;
  std::map<std::string, Font> fonts;
  std::map<std::string, Mesh> meshes;
  std::map<std::string, std::string> text;
};
