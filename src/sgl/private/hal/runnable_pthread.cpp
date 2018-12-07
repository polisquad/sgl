#include "hal/runnable_pthread.h"
#include "hal/runnable.h"

void RunnablePThread::kill(bool bShouldWait)
{
	ASSERT(id, "thread not created?");
	
	// Gently stop runnable
	if (runnable) runnable->stop();

	if (bShouldWait & bStarted)
	{
		pthread_join(id, nullptr);
		bStarted = false;
	}
}

uint32 RunnablePThread::run()
{
	uint32 status = -1;
	if (!runnable) return status;

	if (runnable->init())
	{
		status = runnable->run();
		// Clean up
		runnable->exit();
	}

	return status;
}