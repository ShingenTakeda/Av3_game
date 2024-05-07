#include <Allocators/GenArena.hpp>
#include <cstdint>

GenIndex GenerationalIndexAllocator::allocate()
{
  if(m_free_indices.size() > 0)
  {
    uint64_t index = m_free_indices.back();
    m_free_indices.pop_back();

    m_entries[index].generation += 1;
    m_entries[index].is_alive = true;

    return {index, m_entries[index].generation};
  }
  else 
  {
    m_entries.push_back({true, 0});
    return {static_cast<uint64_t>(m_entries.size()) - 1, 0};
  }
}

void GenerationalIndexAllocator::deallocate(GenIndex index)
{
  if(is_alive(index))
  {
    m_entries[index.index].is_alive = false;
    m_free_indices.push_back(index.index);
  }
}

bool GenerationalIndexAllocator::is_alive(GenIndex index) const
{
  return index.index < m_entries.size() && 
  m_entries[index.index].generation == index.generation &&
  m_entries[index.index].is_alive;
}


