#ifndef SGL_SHARED_PTR_H
#define SGL_SHARED_PTR_H

#include "core_types.h"
#include <memory>

/**
 * Just a typedef for now
 * Maybe I'll implement it in the future
 */
template<typename T>
using SharedPtr = std::shared_ptr<T>;

#endif