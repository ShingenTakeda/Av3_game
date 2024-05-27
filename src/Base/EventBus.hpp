#pragma once
#include "Base/SharedMutex.hpp"
#include <atomic>
#include <cstdint>
#include <ctime>
#include <functional>
#include <future>

using TypeId = uintptr_t;
template <typename MSG_TYPE> using Subscriber = std::function<void(MSG_TYPE)> ;

template < typename T >
static TypeId GetTypeId()
{
  static uint32_t placeHolder;
  return (reinterpret_cast<TypeId>(&placeHolder));
}

struct SubscriberHandle
{
  static std::atomic<uint32_t> common_counter;
  uint64_t counter;
  clock_t clockTime;
  TypeId eventID;

  SubscriberHandle(TypeId id) : counter(++common_counter),
  clockTime(clock()), eventID(id) {}

  uint64_t GetCounter() const {return counter;}
  clock_t GetClock() const {return clockTime;}
  TypeId GetTypeID() const {return eventID;}
  void SetValid() {counter = 0; eventID = 0;}
  bool IsValid() {return ((counter != 0) && (eventID != 0));}
};

template <typename MSG_TYPE>
struct SubscribeHandleTyped : public SubscriberHandle
{
  SubscribeHandleTyped<MSG_TYPE>() : 
  SubscriberHandle(GetTypeId<MSG_TYPE>()) {} 
};

std::atomic< uint32_t > SubscriberHandle::common_counter;

struct MsgBusRepository
{
  MsgBusRepository(): operational(true) {}
  bool operational;
  SharedMutex mutex;
};

template <int BUS_ID = 0>
struct MSGBus
{
  template <typename MSG_TYPE>
  static SubscriberHandle Subscribe(Subscriber<MSG_TYPE> subscriber);

  static void Unsubscribe(SubscriberHandle &handle);

  template <typename MSG_TYPE>
  static void PublishBlocking(const MSG_TYPE &msg);

  template <typename MSG_TYPE>
  static void PublishAsync(const MSG_TYPE &msg);

  static bool IsValidHandle();

  //static MsgBusRepository msgbus_repository;
  
  MSGBus() = delete;
  ~MSGBus() = delete;
  MSGBus( MSGBus& ) = delete;
  MSGBus( MSGBus&& ) = delete;
  MSGBus& operator= ( MSGBus& ) = delete;
  MSGBus& operator= ( MSGBus&& ) = delete;
};

bool operator <(SubscriberHandle const& lhs, SubscriberHandle const& rhs)
{
  return lhs.GetCounter() < rhs.GetCounter() ? true :
            ( lhs.GetCounter() == rhs.GetCounter() ?
                                    lhs.GetClock() < rhs.GetClock() : false ) ;
}
