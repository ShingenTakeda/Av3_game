#pragma once
#include <future>

#include "MessageBusDefs.h"
#include "MsgBusRepository.h"

template < int BUS_ID = 0 > class MsgBus
{
public:

  template < typename MSG_TYPE >
  static SubscriberHandle Subscribe( Subscriber< MSG_TYPE > subscriber );
  static void UnSubscribe( SubscriberHandle& handle );

  template < typename MSG_TYPE >
  static void PublishBlocking( const MSG_TYPE& msg );
  template < typename MSG_TYPE >
  static void PublishAsync( const MSG_TYPE& msg );
  static bool IsValidHandle( SubscriberHandle& handle );

private:

 static MsgBusRepository msgBusRepository;

private:

  /// Instantiation, coping, moving and deleting of MsgBus class is prohibited.

  MsgBus() = delete;
  ~MsgBus() = delete;
  MsgBus( MsgBus& ) = delete;
  MsgBus( MsgBus&& ) = delete;
  MsgBus& operator= ( MsgBus& ) = delete;
  MsgBus& operator= ( MsgBus&& ) = delete;
};

template < int BUS_ID >
template < typename MSG_TYPE >
SubscriberHandle MsgBus< BUS_ID >::Subscribe( Subscriber< MSG_TYPE > subscriber)
{
  return msgBusRepository.Subscribe< MSG_TYPE >( subscriber );
}

template < int BUS_ID >
void MsgBus< BUS_ID >::UnSubscribe( SubscriberHandle& handle )
{
  msgBusRepository.UnSubscribe( handle );
}

template < int BUS_ID >
template < typename MSG_TYPE >
void MsgBus< BUS_ID >::PublishBlocking( const MSG_TYPE& msg )
{
  msgBusRepository.Publish( msg );
}

template < int BUS_ID >
template < typename MSG_TYPE >
void MsgBus< BUS_ID >::PublishAsync( const MSG_TYPE& msg )
{
  std::async( std::launch::async,
              MsgBus< BUS_ID >::PublishBlocking< MSG_TYPE >,
              msg
             );
}

template < int BUS_ID >
bool MsgBus< BUS_ID >::IsValidHandle( SubscriberHandle& handle )
{
  return handle.IsValid();
}

template < int MSG_BUS_NUM >
MsgBusRepository MsgBus< MSG_BUS_NUM >::msgBusRepository;
