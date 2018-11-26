#ifndef SGL_LOGGER_H
#define SGL_LOGGER_H

#include "system.h"
#include "event.h"

/**
 * @class Logger core/logger.h
 * @brief Logs system events and user logs
 */
class Logger : public System
{
public:
	/// @copydoc System::handle()
	FORCE_INLINE virtual void handle(Event * e);
};

void Logger::handle(Event * e)
{
	// Log to stdout for now
	fprintf(stdout, "(%llu): unkown event;\n", e->getSender()->getName());
}

#endif