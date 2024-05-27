#include <Base/Semaphore.hpp>
#include <mutex>

void Semaphore::Lock()
{
  std::unique_lock<std::mutex> lock(mtx);
  cond.wait(lock, [this]{return (0 != count);});
  --count;
}

void Semaphore::Unlock()
{
  std::lock_guard<std::mutex> lock(mtx);
  ++count;
  cond.notify_one();
}
