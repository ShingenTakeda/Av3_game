/*!*****************************************************************************
* @file   MsgTypeContainer.h
*
* @brief  Holds all callable targets of the specific MSG_TYPE.
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef MSGTYPECONTAINER_H_
#define MSGTYPECONTAINER_H_

#include <map>

#include "MessageBusDefs.h"
#include "SubscriberHandle.h"

/*!*****************************************************************************
* @class MsgTypeContainerBase
*
* @brief Non template base of MsgTypeContainer class
*        Required for omitting template parameter dependency
*        in MsgTypeContainer class
*
*******************************************************************************/

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

/*!*****************************************************************************
* @class MsgTypeContainer
*
* @brief Holds all callable targets of the specific MSG_TYPE
*
*******************************************************************************/

template < typename MSG_TYPE >
class MsgTypeContainer : public MsgTypeContainerBase
{
public:

  /*!***************************************************************************
  *
  * @brief Add.
  *        Add new callable target.
  *
  *****************************************************************************/

  void Add( SubscriberHandle handle, Subscriber< MSG_TYPE > subscriber  )
  {
    containerMap.insert( MsgBusContainerMapPair( handle, subscriber ) );
  }

  /*!***************************************************************************
  *
  * @brief Remove.
  *        Remove callable target.
  *
  *****************************************************************************/

  void Remove( SubscriberHandle handle )
  {
    containerMap.erase( handle );
  }

  /*!***************************************************************************
  *
  * @brief Empty.
  *        Check if container is empty.
  *
  *****************************************************************************/

  bool Empty()
  {
    return containerMap.empty();
  }

  /*!***************************************************************************
  *
  * @brief Publish.
  *        Publish message to all targets in conatiner.
  *
  *****************************************************************************/
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

#endif /* MSGTYPECONTAINER_H_ */
