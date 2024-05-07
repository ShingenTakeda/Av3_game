#pragma once
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <vector>

extern int s_componentCounter;
typedef uint64_t EntityID;
const int MAX_COMPONENTS = 32;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

typedef uint64_t EntityIndex;
typedef uint64_t EntityVersion;

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
  std::vector<ComponentPool*> componentPools;
  
  EntityID NewEntity();

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
