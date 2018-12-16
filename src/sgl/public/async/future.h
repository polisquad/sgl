#ifndef SGL_FUTURE_H
#define SGL_FUTURE_H

#include "core_types.h"
#include "hal/event.h"
#include "hal/critical_section.h"
#include "templates/atomic.h"
#include "templates/function.h"

/**
 * @class GenericFutureState async/future.h
 * @brief Agnostic-type future state (container for async values)
 */
class GenericFutureState
{
public:
	/// @brief Default constructor
	FORCE_INLINE GenericFutureState() :
		completionEvent(nullptr),
		bComplete(false) {}

	/// @brief Destructor
	FORCE_INLINE ~GenericFutureState()
	{
		/// @todo Release completion event
		completionEvent = nullptr;
	}

	/// @brief Returns completeness flag
	FORCE_INLINE bool isComplete() const { return bComplete; }

	/**
	 * @brief Blocks calling thread until result is available
	 * 
	 * @param [in]	the max amount of time to wait (ms)
	 * 
	 * @return @c false if timed out, @c true otherwise (result available)
	 */
	FORCE_INLINE bool wait(uint32 waitTime = 0xffffffff) const { return completionEvent->wait(waitTime); }

	/// @brief Mark as complete and signal waiting threads
	void complete()
	{
		completionEvent->trigger();
		if (callback) callback();
	}

protected:
	/// @brief Controls access to this future
	CriticalSection mutex;

	/// @brief Callback function, called when future is complete
	Function<void()> callback;

	/// @brief Event triggered when the result is available
	Event * completionEvent;

	/// @brief Indicates if the result is available
	Atomic<bool> bComplete;
};

#endif