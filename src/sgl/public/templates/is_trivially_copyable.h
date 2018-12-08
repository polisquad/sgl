#ifndef SGL_IS_TRIVIALLY_COPYABLE_H
#define SGL_IS_TRIVIALLY_COPYABLE_H

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

#endif