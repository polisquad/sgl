#ifndef SGL_ENABLE_IF_H
#define SGL_ENABLE_IF_H

#include "core_types.h"

/**
 * Sets Type to specified type if boolean condition is true
 */
template<bool Cond, typename T = void>
struct EnableIf {};

template<typename T>
struct EnableIf<true, T> { using Type = T; };

/// Quick macro
#define EnableIfT(Cond, T) typename EnableIf<Cond, T>::Type

#endif