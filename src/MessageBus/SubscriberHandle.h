#pragma once
#include <ctime>
#include <cstdint>
#include <atomic>

#include "TypeId.h"


class SubscriberHandle
{
public:


  SubscriberHandle( TypeId _typeId ) : counter( ++commonCounter ),
                                       clockTime( clock() ),
                                       typeId( _typeId ) {}


  uint64_t GetCounter() const { return counter; }
  clock_t  GetClock()  const { return clockTime; }
  TypeId   GetTypeid() const { return typeId;  }
  void SetInvalid() { counter = 0; typeId = 0; }
  bool IsValid() { return ( ( counter != 0 ) && ( typeId != 0 ) ); }

private:
  static std::atomic< uint32_t > commonCounter;
  uint64_t counter;
  clock_t  clockTime;
  TypeId   typeId;
};

template < typename MSG_TYPE >class SubscriberHandleTyped
                                                      : public SubscriberHandle
{
public:
  SubscriberHandleTyped< MSG_TYPE >()
                                : SubscriberHandle( GetTypeId< MSG_TYPE >() ) {}
};

bool operator <(SubscriberHandle const& lhs, SubscriberHandle const& rhs)
{
  return lhs.GetCounter() < rhs.GetCounter() ? true :
            ( lhs.GetCounter() == rhs.GetCounter() ?
                                    lhs.GetClock() < rhs.GetClock() : false ) ;
}

std::atomic< uint32_t > SubscriberHandle::commonCounter;

