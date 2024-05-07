#pragma once
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

struct GenIndex
{
  uint64_t index = 0;
  uint64_t generation = 0;
};

struct GenerationalIndexAllocator
{
  struct AllocatorEntry
  {
    bool is_alive = false;
    uint64_t generation = 0;
  };

  std::vector<AllocatorEntry> m_entries;
  std::vector<uint64_t> m_free_indices;

  GenIndex allocate();
  void deallocate(GenIndex index);
  bool is_alive(GenIndex index) const;
};

template <typename T>
struct GenIndexArray
{
  struct Entry
  {
    uint64_t generation;
    T value;
  };

  std::vector<std::optional<Entry>> m_entries;

  void set(GenIndex index, T value)
  {
    while(m_entries.size() <= m_entries[index.index])
    {
      m_entries.push_back(std::nullopt);
    }

    uint64_t prev_gen = 0;

    if(auto prev_entry = m_entries[index.index])
    {
      prev_gen = prev_entry->generation;
    }

    if(prev_gen > index.generation)
    {
      exit(1);
    }

    m_entries[index.index] = std::optional<Entry>{{index.generation, value}};
  }

  void remove(GenIndex index)
  {
    if(index.index < m_entries.size())
    {
      m_entries[index.index] = std::nullopt;
    }
  }

  T *get(GenIndex index)
  {
    if(index.index >= m_entries.size())
    {
      return nullptr;
    }

    if(auto &entry = m_entries[index.index])
    {
      if(entry->generation == index.generation)
      {
        return &entry->value;
      }
    }

    return nullptr;
  }

  const T *get(GenIndex index) const
  {
    return const_cast<const T*>(const_cast<GenIndexArray*>(this)->get(index));
  }

  std::vector<GenIndex> get_all_valid_indices(const GenerationalIndexAllocator &allocator) const
  {
    std::vector<GenIndex> result;

    for(int i = 0; i < m_entries.size(); ++i)
    {
      const auto &entry = m_entries[i];
      if(!entry)
      {
        continue;
      }

      GenIndex index = {i, entry->generation};

      if(allocator.is_alive(index))
      {
        result.push_back(index);
      }
    }

    return result;
  }

  std::optional<std::tuple<GenIndex, std::reference_wrapper<const T>>> get_first_valid_entry(const GenerationalIndexAllocator &allocator) const
  {
    for(auto i = 0; i < m_entries.size(); ++i)
    {
      const auto &entry = m_entries[i];
      if(!entry)
      {
        continue;
      }

      GenIndex index = {i, entry->generation};

      if(allocator.is_alive(index))
      {
        return std::make_tuple(index, std::ref(entry->value));
      }
    }

    return std::nullopt;
  }
};
