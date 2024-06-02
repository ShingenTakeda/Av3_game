/*!*****************************************************************************
* @file   MessageBusDefs.h
*
* @brief  Types definition for Message Bus
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef MESSAGEBUSDEFS_H_
#define MESSAGEBUSDEFS_H_

#include <functional>

/*!*****************************************************************************
* @typedef Subscriber
*
* @brief Subscriber may be any callable target.
*        That may be function, lambda expression, functor or bind expression.
*
* @tparam  MSG_TYPE
*
*******************************************************************************/

template < typename MSG_TYPE > using Subscriber = std::function<void(MSG_TYPE)>;

#endif /* MESSAGEBUSDEFS_H_ */
