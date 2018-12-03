#ifndef SDL_ASYNC_H
#define SDL_ASYNC_H

#include "core_types.h"

/// @brief Available async execution methods
enum class AsyncExecutionMethod
{
	/// Short running tasks (e.g. render commands)
	TaskGraph,

	/// Long running tasks, executed in separate threads
	Thread,

	/// Parallel tasks, executed in a global thread pool
	ThreadPool
};



#endif