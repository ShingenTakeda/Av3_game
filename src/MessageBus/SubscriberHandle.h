/*!*****************************************************************************
* @file   SubscriberHandle.h
*
* @brief  Implementation of Subscriber handle
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef SUBSCRIBERHANDLE_H_
#define SUBSCRIBERHANDLE_H_

#include <ctime>
#include <cstdint>
#include <atomic>

#include "TypeId.h"

/*!*****************************************************************************
* @class  SubscriberHandle
*
* @brief  Represents unique handle for subscriber.
*         Generated using atomic counter and clockTime( to handle counter
*         overflow )
*
*******************************************************************************/

class SubscriberHandle
{
public:

  /*!***************************************************************************
  * @brief   Constructor
  *
  * @param   _typeId	id for the type that handle will be generated for
  *
  *****************************************************************************/

  SubscriberHandle( TypeId _typeId ) : counter( ++commonCounter ),
                                       clockTime( clock() ),
                                       typeId( _typeId ) {}

  /*!***************************************************************************
  * @brief   Get Counter
  *
  * @return  atomic counter value
  *
  *****************************************************************************/

  uint64_t GetCounter() const { return counter; }

  /*!***************************************************************************
  * @brief   Get Clock
  *
  * @return  clock value
  *
  *****************************************************************************/
  clock_t  GetClock()  const { return clockTime; }

  /*!***************************************************************************
  * @brief   Get TypeId
  *
  * @return  type ID
  *
  *****************************************************************************/
  TypeId   GetTypeid() const { return typeId;  }

  /*!***************************************************************************
  * @brief   SetInvalid  Handle values
  *
  *****************************************************************************/

  void SetInvalid() { counter = 0; typeId = 0; }

  /*!***************************************************************************
  * @brief   Check handle validity
  *
  * @return  true - valid, false else
  *
  *****************************************************************************/
  bool IsValid() { return ( ( counter != 0 ) && ( typeId != 0 ) ); }

private:
  static std::atomic< uint32_t > commonCounter;
  uint64_t counter;
  clock_t  clockTime;
  TypeId   typeId;
};

/*!*****************************************************************************
* @class  SubscriberHandleTyped
*
* @brief  Generate Subscriber handle using MSG_TYPE as an inpu template param
*
* @tparam  MSG_TYPE   message type
*
*******************************************************************************/

template < typename MSG_TYPE >class SubscriberHandleTyped
                                                      : public SubscriberHandle
{
public:
  SubscriberHandleTyped< MSG_TYPE >()
                                : SubscriberHandle( GetTypeId< MSG_TYPE >() ) {}
};

/*!*****************************************************************************
* @brief   Comparator.
*          Used in ordered Data Structures with imdexed by Subscriber handle.
*
* @param  lhs   left value for comparison
*
* @param  rhs   right value for comparison
*
*******************************************************************************/

bool operator <(SubscriberHandle const& lhs, SubscriberHandle const& rhs)
{
  return lhs.GetCounter() < rhs.GetCounter() ? true :
            ( lhs.GetCounter() == rhs.GetCounter() ?
                                    lhs.GetClock() < rhs.GetClock() : false ) ;
}

std::atomic< uint32_t > SubscriberHandle::commonCounter;

#endif /* SUBSCRIBERHANDLE_H_ */
