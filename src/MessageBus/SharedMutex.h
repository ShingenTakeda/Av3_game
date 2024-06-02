/*!*****************************************************************************
* @file   SharedMutex.h
*
* @brief  Declaration of SharedMutex class
*         Shared Mutex is a class implemenation of Multiple Reader - Single
*         Writer lock.
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef SHAREDMUTEX_H_
#define SHAREDMUTEX_H_

#include <mutex>
#include "Semaphore.h"

/*!*****************************************************************************
* @class  SharedMutex
*
* @brief  Mutex implementation that allows multiple Shared Locks, and single
*         Exclusive Lock. Also known as Multiple Readers -Single Writer Lock
*
*******************************************************************************/

class SharedMutex
{
public:

  /*!***************************************************************************
  * @brief   Constructor
  *
  *****************************************************************************/

  SharedMutex() : sharedCount( 0 ), sharedInsideSem( 1 ) {}

  /*!***************************************************************************
  * @brief   Destructor
  *
  *****************************************************************************/

  ~SharedMutex() = default;

  /*!***************************************************************************
  * @brief   Take Shared Lock
  *
  *****************************************************************************/

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

  /*!***************************************************************************
  * @brief   Release Shared lock
  *
  *****************************************************************************/

  void UnlockShared()
  {
    sharedEntryMutex.lock();
    if( 0 == --sharedCount )
    {
      sharedInsideSem.Unlock();
    }
    sharedEntryMutex.unlock();
  }


  /*!***************************************************************************
  * @brief   Take Exclusive Lock
  *
  *****************************************************************************/

  void LockExclusive()
  {
    genEntryMutex.lock();
    sharedInsideSem.Lock();
  }


  /*!***************************************************************************
  * @brief   Release Exclusive Lock
  *
  *****************************************************************************/

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


#endif /* SHAREDMUTEX_H_ */
