#pragma once
#include "entt/entity/fwd.hpp"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <entt/entt.hpp>

extern int s_componentCounter;
typedef uint64_t EntityID;
const int MAX_COMPONENTS = 32;
using ComponentMask = std::bitset<MAX_COMPONENTS>;
using ECSWorld = entt::registry;

typedef uint64_t EntityIndex;
typedef uint64_t EntityVersion;

inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
  // Shift the index up 32, and put the version in the bottom
  return ((EntityID)index << 32) | ((EntityID)version);
}
inline EntityIndex GetEntityIndex(EntityID id)
{
  // Shift down 32 so we lose the version and get our index
  return id >> 32;
}
inline EntityVersion GetEntityVersion(EntityID id)
{
  // Cast to a 32 bit int to get our version number (loosing the top 32 bits)
  return (EntityVersion)id;
}
inline bool IsEntityValid(EntityID id)
{
  // Check if the index is our invalid index
  return (id >> 32) != EntityIndex(-1);
}

#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

template <class T>
int GetId()
{
  static int s_componentID = s_componentCounter++;
  return s_componentID;
}

struct ComponentPool
{
  ComponentPool(size_t element_size)
  {
    element_size = element_size;
    pData = new char[element_size * MAX_COMPONENTS];
  }

  ~ComponentPool()
  {
    delete[] pData;
  }

  inline void *get(size_t index)
  {
    return pData + index + element_size;
  }

  char *pData = nullptr;
  size_t element_size = {0};
};

struct World
{
  struct EntityDesc
  {
    EntityID id;
    ComponentMask mask;
  };

  std::vector<EntityDesc> entities;
  std::vector<EntityIndex> free_entites;
  std::vector<ComponentPool*> componentPools;
  
  EntityID NewEntity();
  void DestroyEntity(EntityID id);

  template <typename T>
  void AssignComponent(EntityID id)
  {
    int componentID = GetId<T>();
    entities[id].mask.set(componentID);
  }

  template <typename T>
  T *AssignComponent(EntityID id)
  {
    int componentId = GetId<T>();

    if(componentPools.size() <= componentId)
    {
      componentPools.resize(componentId + 1, nullptr);
    }

    if(componentPools[componentId] == nullptr)
    {
      componentPools[componentId] = new ComponentPool(sizeof(T));
    }

    T *pComponent = new (componentPools[componentId]->get(id)) T();

    entities[id].mask.set(componentId);
    return pComponent;
  }

  template <typename T>
  T *get(EntityID id)
  {
    int componentId = GetId<T>();
    if(!entities[id].mask.test(componentId))
    {
      return nullptr;
    }

    T *pComponent = static_cast<T>(componentPools[componentId]->get(id));
    return pComponent;
  }
};
