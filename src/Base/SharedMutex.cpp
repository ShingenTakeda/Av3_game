#include <Base/SharedMutex.hpp>

void SharedMutex::LockShared()
{
  gen_entry.lock();
  gen_entry.unlock();
  share_entry_mutex.lock();
  if(0 == shared_count++)
  {
    shared_inside_sim.Lock();
  }
  share_entry_mutex.unlock();
}

void SharedMutex::UnlockShared()
{
  share_entry_mutex.lock();
  if(0 == --shared_count)
  {
    shared_inside_sim.Unlock();
  }
  share_entry_mutex.unlock();
}

void SharedMutex::LockExclusive()
{
  gen_entry.lock();
  shared_inside_sim.Lock();
}

void SharedMutex::UnlockExclusive()
{
  shared_inside_sim.Unlock();
  gen_entry.unlock();
}
