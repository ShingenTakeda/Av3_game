#pragma once
#include <functional>

template < typename MSG_TYPE > using Subscriber = std::function<void(MSG_TYPE)>;
