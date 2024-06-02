/*!*****************************************************************************
* @file   MessageBus.h
*
* @brief  Message Bus API
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef MESSAGEBUS_H_
#define MESSAGEBUS_H_

#include <future>

#include "MessageBusDefs.h"
#include "MsgBusRepository.h"

/*!*****************************************************************************
* @class MsgBus
*
* @brief Message Bus API.
*
*        Message Bus provides generic message distribution functionality.
*
*        Message Bus class has only static methods and static data members.
*        No instantiation is required for using Message Bus.
*
*******************************************************************************/

template < int BUS_ID = 0 > class MsgBus
{
public:

  /*!***************************************************************************
  * @brief   Subscribe for receiving messages of the specific Message Type.
  *
  * @tparam  MSG_TYPE     Type for which new subscriber will be added.
  *
  * @param   subscriber   Callable target.
  *
  * @return  Handle associated with a registered subscriber. Use IsValidHandle()
  *          for operation success checking.
  *
  *****************************************************************************/

  template < typename MSG_TYPE >
  static SubscriberHandle Subscribe( Subscriber< MSG_TYPE > subscriber );

  /*!***************************************************************************
  * @brief   UnSubscribe from receiving messages of the specific Message Type.
  *
  * @param   handle      Subscriber handle.
  *
  *****************************************************************************/

  static void UnSubscribe( SubscriberHandle& handle );

  /*!***************************************************************************
  * @brief   Publish message by blocking call. The method will return only
  *          when all subscribers  will receive published message.
  *
  * @tparam  MSG_TYPE    Message type - optional, will be deducted by compiler.
  *
  * @param   msg         Message to be published.
  *
  *****************************************************************************/

  template < typename MSG_TYPE >
  static void PublishBlocking( const MSG_TYPE& msg );

  /*!***************************************************************************
  * @brief   Publish message by asynchronous call. The method will return
  *          immediately, the message will be delivered asynchronously.
  *
  * @tparam  MSG_TYPE    Message type - optional, will be deducted by compiler.
  *
  * @param   msg         Message to be published.
  *
  *****************************************************************************/

  template < typename MSG_TYPE >
  static void PublishAsync( const MSG_TYPE& msg );

  /*!***************************************************************************
  * @brief   Check Subscriber handle validity.
  *
  * @param   handle      Subscriber handle.
  *
  * @return  true - valid handle, false else.
  *
  *****************************************************************************/

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

////////////////////////////////////////////////////////////////////////////////

template < int BUS_ID >
template < typename MSG_TYPE >
SubscriberHandle MsgBus< BUS_ID >::Subscribe( Subscriber< MSG_TYPE > subscriber)
{
  return msgBusRepository.Subscribe< MSG_TYPE >( subscriber );
}

////////////////////////////////////////////////////////////////////////////////

template < int BUS_ID >
void MsgBus< BUS_ID >::UnSubscribe( SubscriberHandle& handle )
{
  msgBusRepository.UnSubscribe( handle );
}

////////////////////////////////////////////////////////////////////////////////

template < int BUS_ID >
template < typename MSG_TYPE >
void MsgBus< BUS_ID >::PublishBlocking( const MSG_TYPE& msg )
{
  msgBusRepository.Publish( msg );
}

////////////////////////////////////////////////////////////////////////////////

template < int BUS_ID >
template < typename MSG_TYPE >
void MsgBus< BUS_ID >::PublishAsync( const MSG_TYPE& msg )
{
  std::async( std::launch::async,
              MsgBus< BUS_ID >::PublishBlocking< MSG_TYPE >,
              msg
             );
}

////////////////////////////////////////////////////////////////////////////////

template < int BUS_ID >
bool MsgBus< BUS_ID >::IsValidHandle( SubscriberHandle& handle )
{
  return handle.IsValid();
}

////////////////////////////////////////////////////////////////////////////////

template < int MSG_BUS_NUM >
MsgBusRepository MsgBus< MSG_BUS_NUM >::msgBusRepository;

#endif /* MESSAGEBUS_H_ */
