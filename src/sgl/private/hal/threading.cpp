#include "hal/threading.h"

/// @todo Remove
#include "hal/runnable_pthread.h"

/////////////////////////////////////////////////
// Globals                                     //
/////////////////////////////////////////////////

/// @brief Global audio thread ... but we don't have audio LOL
RunnableThread * gAudioThread = nullptr;

/// @brief Global rendering thread
RunnableThread * gRenderingThread = nullptr;

/// @brief Return true if current thread is rendering thread
FORCE_INLINE bool isInRenderingThread()
{
	return gRenderingThread && PlatformTLS::getCurrentThreadId() == gRenderingThread->getThreadId();
}

/////////////////////////////////////////////////
// RunnableThread implementation               //
/////////////////////////////////////////////////

RunnableThread::RunnableThread() :
	runnable(nullptr),
	id(0) {}

RunnableThread::~RunnableThread()
{
	/// @todo Remove from thread manager ... when there is one!
}

RunnableThread * RunnableThread::create(Runnable * _runnable, const ansichar * _name, uint32 stackSize)
{
	// We assume we support multithreading, it's 2018 for fuck sake!

	/// @todo Replace with platform specific code
	RunnableThread * thread = new RunnablePThread;
	if (thread)
	{
		if (UNLIKELY(!thread->create_internal(_runnable, _name, stackSize)))
		{
			delete thread;
			thread = nullptr;
		}
	}

	return thread;
}