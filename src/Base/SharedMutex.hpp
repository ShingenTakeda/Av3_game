#pragma once
#include <Base/Semaphore.hpp>
#include <cstdint>
#include <mutex>

struct SharedMutex
{
  SharedMutex() : shared_count(0), shared_inside_sim(1) {}

  void LockShared();
  void UnlockShared();
  void LockExclusive();
  void UnlockExclusive();

  uint32_t shared_count;
  std::mutex gen_entry;
  std::mutex share_entry_mutex;
  Semaphore shared_inside_sim;
};
