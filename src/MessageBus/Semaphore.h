#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:

  explicit Semaphore( unsigned int initCount ) : count( initCount ) {}

  ~Semaphore() = default;

  void Lock()
  {
    std::unique_lock< std::mutex > lock( mtx );
    cond.wait( lock, [this] { return ( 0 != count ); } );
    --count;
  }

  void Unlock()
  {
    std::lock_guard< std::mutex > lock( mtx );
    ++count;
    cond.notify_one();
  }

private:

  unsigned int              count;
  std::condition_variable   cond;
  std::mutex                mtx;
};
