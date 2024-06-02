#pragma once
#include <map>

#include "MessageBusDefs.h"
#include "SubscriberHandle.h"

class MsgTypeContainerBase
{
public:

  MsgTypeContainerBase() = default;
  virtual ~MsgTypeContainerBase() = default;
  MsgTypeContainerBase( MsgTypeContainerBase& ) = delete;
  MsgTypeContainerBase( MsgTypeContainerBase&& ) = delete;
  MsgTypeContainerBase& operator= ( MsgTypeContainerBase& ) = delete;
  MsgTypeContainerBase& operator= ( MsgTypeContainerBase&& ) = delete;

  virtual void Remove( SubscriberHandle handle ) = 0;
  virtual bool Empty() = 0;
};


template < typename MSG_TYPE >
class MsgTypeContainer : public MsgTypeContainerBase
{
public:

  void Add( SubscriberHandle handle, Subscriber< MSG_TYPE > subscriber  )
  {
    containerMap.insert( MsgBusContainerMapPair( handle, subscriber ) );
  }

  void Remove( SubscriberHandle handle )
  {
    containerMap.erase( handle );
  }

  bool Empty()
  {
    return containerMap.empty();
  }

  void Publish( const MSG_TYPE& msg )
  {
    for (auto& iter: containerMap )
    {
      iter.second( msg );
    }
  }

  /// Default Contructor and Destructor
  //  Deleted Move and Copy Constractors and Assign Operators

  MsgTypeContainer() = default;
#ifdef __GNUC__
  virtual ~MsgTypeContainer() noexcept = default;
#else
  virtual ~MsgTypeContainer() = default;
#endif
  MsgTypeContainer(MsgTypeContainer&) = delete;
  MsgTypeContainer(MsgTypeContainer&&) = delete;
  MsgTypeContainer& operator= (MsgTypeContainer&) = delete;
  MsgTypeContainer& operator= (MsgTypeContainer&&) = delete;

private:

  using MsgBusContainerMap = std::map< SubscriberHandle,
                                       Subscriber< MSG_TYPE >
                                     >;

  using MsgBusContainerMapPair = std::pair< SubscriberHandle,
                                            Subscriber< MSG_TYPE >
                                          >;

  MsgBusContainerMap containerMap;
};
