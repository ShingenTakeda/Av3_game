#include <ECS/ECS.hpp>

EntityID World::NewEntity()
{
  if(!free_entites.empty())
  {
    EntityIndex new_index = free_entites.back();
    free_entites.pop_back();
    EntityID newID = CreateEntityId(new_index, GetEntityVersion(entities[new_index].id));
    entities[new_index].id = newID;
    return entities[new_index].id;
  }

  entities.push_back({CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask()});
  return entities.back().id;
}

void World::DestroyEntity(EntityID id)
{
  EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id + 1));
  entities[GetEntityIndex(id)].id = newID;
  entities[GetEntityIndex(id)].mask.reset();
  free_entites.push_back(GetEntityIndex(id));
}
