#pragma once
#include <map>

#include "MessageBusDefs.h"
#include "TypeId.h"
#include "SharedMutex.h"
#include "SubscriberHandle.h"
#include "MsgTypeContainer.h"

class MsgBusRepository
{
public:

  MsgBusRepository()  : operational( true ) {}

  ~MsgBusRepository()
  {
    mutex.LockExclusive();

    for (auto iter: repositoryMap )
    {
      delete iter.second;
    }

    operational = false;

    mutex.UnlockExclusive();
  }

  template < typename MSG_TYPE >
  SubscriberHandle Subscribe( Subscriber< MSG_TYPE > subscriber )
  {
    TypeId typeId = GetTypeId< MSG_TYPE >();

    mutex.LockExclusive();

    SubscriberHandleTyped< MSG_TYPE > handle;

    if ( operational )
    {
      auto ret = repositoryMap.insert(
                              MsgBusRepositoryMapPair( typeId, nullptr ) );

      /// Check if this is the first subscriber for the MSG_TYPE.
      if ( ret.second == true )
      {
        ret.first->second = new MsgTypeContainer< MSG_TYPE >;
      }

      MsgTypeContainer< MSG_TYPE >*
      container = static_cast<MsgTypeContainer< MSG_TYPE >*>(ret.first->second);

      /// Add subscriber to the container.
      container->Add( handle, subscriber);
    }

    else
    {
      handle.SetInvalid();
    }

    mutex.UnlockExclusive();

    return handle;
  }

  void UnSubscribe( SubscriberHandle& handle )
  {
    mutex.LockExclusive();

    if( operational && handle.IsValid() )
    {
      TypeId typeId = handle.GetTypeid();

      auto iter = repositoryMap.find( typeId );

      if ( iter != repositoryMap.end() )
      {
        MsgTypeContainerBase* container =iter->second;

        container->Remove( handle );

        /// Check if this is the last subscriber in the container
        if( container->Empty() )
        {
          repositoryMap.erase( iter );

          delete container;
        }
      }
    }

	handle.SetInvalid();

    mutex.UnlockExclusive();
  }

  template < typename MSG_TYPE > void Publish( const MSG_TYPE& msg )
  {
    TypeId typeId = GetTypeId< MSG_TYPE >();

    mutex.LockShared();

    if( operational )
    {
      auto iter = repositoryMap.find( typeId );

      if ( iter != repositoryMap.end() )
      {
        MsgTypeContainer< MSG_TYPE >*
        container = static_cast< MsgTypeContainer< MSG_TYPE >* >(iter->second);

        container->Publish( msg );
      }
    }

    mutex.UnlockShared();
  }

  /// Disable coping and moving.

  MsgBusRepository( MsgBusRepository& )  = delete;
  MsgBusRepository( MsgBusRepository&& ) = delete;
  MsgBusRepository& operator= ( MsgBusRepository& ) = delete;
  MsgBusRepository& operator= ( MsgBusRepository&& ) = delete;

private:

  using MsgBusRepositoryMap     = std::map< TypeId, MsgTypeContainerBase* >;
  using MsgBusRepositoryMapPair = std::pair< TypeId, MsgTypeContainerBase* >;

  bool operational;
  MsgBusRepositoryMap repositoryMap;

  /// Multiple Readers - Single Writer Lock.
  SharedMutex         mutex;
};
