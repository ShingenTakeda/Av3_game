/*!*****************************************************************************
* @file   Semaphore.h
*
* @brief  Implementation of counting Semaphore
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

/*!*****************************************************************************
* @class Semaphore
*
* @brief Counting Semaphore.
*
*******************************************************************************/

class Semaphore
{
public:

  /*!***************************************************************************
  * @brief   Constructor
  *
  *
  * @param   initCount   Initial Semaphore count.
  *
  *****************************************************************************/

  explicit Semaphore( unsigned int initCount ) : count( initCount ) {}

  /*!***************************************************************************
  * @brief   Destructor
  *
  *****************************************************************************/
  ~Semaphore() = default;

  /*!***************************************************************************
  * @brief   Lock the semaphore
  *          On positive counter returns immediately.
  *          On zero - blocked till countered increased.
  *
  *****************************************************************************/

  void Lock()
  {
    std::unique_lock< std::mutex > lock( mtx );
    cond.wait( lock, [this] { return ( 0 != count ); } );
    --count;
  }

  /*!***************************************************************************
  * @brief   UnLock the semaphore
  *          Increases semaphore count.
   *
  *****************************************************************************/

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

#endif /* SEMAPHORE_H_ */
