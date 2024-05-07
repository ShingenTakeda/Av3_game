#include <ECS/ECS.hpp>

EntityID World::NewEntity()
{
  entities.push_back({entities.size(), ComponentMask()});
  return entities.back().id;
}

