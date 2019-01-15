#pragma once

#include "core_types.h"
#include <type_traits>

/**
 * @brief Set the const member to @c true if type is
 * trivially copyable
 * 
 * Just a typedef for now
 */
template<typename T>
using IsTriviallyCopyable = std::is_trivially_copyable<T>;

#define IsTriviallyCopyableV(T) IsTriviallyCopyable<T>::value

