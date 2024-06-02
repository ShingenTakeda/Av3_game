/*!*****************************************************************************
* @file   MsgBusRepository.h
*
* @brief  Repository of all callable targets for specific bus.
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef MSGBUSREPOSITORY_H_
#define MSGBUSREPOSITORY_H_

#include <map>

#include "MessageBusDefs.h"
#include "TypeId.h"
#include "SharedMutex.h"
#include "SubscriberHandle.h"
#include "MsgTypeContainer.h"

/*!*****************************************************************************
* @class MsgBusRepository
*
* @brief Repository of all callable targets for specific bus.
*
*******************************************************************************/

class MsgBusRepository
{
public:

  /*!***************************************************************************
  *
  * @brief Constructor.
  *
  *****************************************************************************/

  MsgBusRepository()  : operational( true ) {}

  /*!***************************************************************************
  *
  * @brief Destructor.
  *
  *****************************************************************************/
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

  /*!***************************************************************************
  *
  * @brief Subscribe.
  *        Add new Subscriber to the repository.
  *
  *****************************************************************************/

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

  /*!***************************************************************************
  *
  * @brief UnSubscribe.
  *        Remove subscriber from repository.
  *
  *****************************************************************************/

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

  /*!***************************************************************************
  *
  * @brief Publish.
  *        Publish message for all subscribers for MSG_TYPE.
  *
  *****************************************************************************/

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



#endif /* MSGBUSREPOSITORY_H_ */
