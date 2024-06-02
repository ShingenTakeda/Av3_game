#pragma once
#include <mutex>
#include "Semaphore.h"

class SharedMutex
{
public:

  SharedMutex() : sharedCount( 0 ), sharedInsideSem( 1 ) {}
  ~SharedMutex() = default;

  void LockShared()
  {
    genEntryMutex.lock();
    genEntryMutex.unlock();
    sharedEntryMutex.lock();
    if( 0 == sharedCount++ )
    {
      sharedInsideSem.Lock();
    }
    sharedEntryMutex.unlock();
  }

  void UnlockShared()
  {
    sharedEntryMutex.lock();
    if( 0 == --sharedCount )
    {
      sharedInsideSem.Unlock();
    }
    sharedEntryMutex.unlock();
  }


  void LockExclusive()
  {
    genEntryMutex.lock();
    sharedInsideSem.Lock();
  }

  void UnlockExclusive()
  {
    sharedInsideSem.Unlock();
    genEntryMutex.unlock();
  }

private:

  unsigned int            sharedCount;
  std::mutex              genEntryMutex;
  std::mutex              sharedEntryMutex;
  Semaphore               sharedInsideSem;
};

