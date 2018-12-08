#ifndef SGL_FUTURE_H
#define SGL_FUTURE_H

#include "core_types.h"
#include "templates/function.h"

/**
 * @class GenericFutureState async/future.h
 * @brief Agnostic-type future state (container for async values)
 */
class GenericFutureState
{
protected:
	/// @brief Callback function, called when future is complete
	Function<void()> callback;
};

#endif