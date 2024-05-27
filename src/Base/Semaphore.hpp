#pragma once
#include <cstdint>
#include <mutex>
#include <condition_variable>

struct Semaphore
{
  explicit Semaphore(uint32_t init_count) : count(init_count) {}
  ~Semaphore() = default;

  void Lock();
  void Unlock();
  
  uint32_t  count;
  std::condition_variable cond;
  std::mutex mtx;
};
