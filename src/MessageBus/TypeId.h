#pragma once
#include <cstdint>

using TypeId = uintptr_t;
template < typename T >
static TypeId GetTypeId()
{
  static uint32_t placeHolder;
  return (reinterpret_cast<TypeId>(&placeHolder));
}

