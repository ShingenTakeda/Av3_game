/*!*****************************************************************************
* @file   TypeId.h
*
* @brief  IRun Time unique id per type generator.
*
* @author Evgeny Zavalkovsky
*
* @date   February 2014
*******************************************************************************/

#ifndef TYPEID_H_
#define TYPEID_H_

#include <cstdint>

/*!*****************************************************************************
* @typedef TypeId
*
* @brief	 Unique Id type
*
*******************************************************************************/

using TypeId = uintptr_t;

/*!*****************************************************************************
* @brief	 generate unique id for type T
*
* @tparam	 T 	Type for which unique id will be generated
*
*
*******************************************************************************/

template < typename T >
static TypeId GetTypeId()
{
  static uint32_t placeHolder;
  return (reinterpret_cast<TypeId>(&placeHolder));
}

#endif /* TYPEID_H_ */
