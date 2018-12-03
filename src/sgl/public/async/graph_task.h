#ifndef SDL_GRAPH_TASK_H
#define SDL_GRAPH_TASK_H

namespace Threads
{
	/// @brief Unscoped enum with thread types
	enum Type
	{
		VOID_THREAD = -1,
		ANY_THREAD = 0,
		RHI_THREAD,
		GAME_THREAD
	};


}

/**
 * @class BaseGraphTask async/graph_task.h
 * @brief Base class for all task graphs
 */
class BaseGraphTask
{

};

/**
 * @class GraphTask async/graph_task.h
 * @brief Embeds a user defined task
 */
template<typename Task>
class GraphTask : public BaseGraphTask
{
public:
	/// @brief Helper to construct and execute the task
	class TaskConstructor
	{
		friend class GraphTask;

	private:
		/// @brief GraphTask owner
		GraphTask * owner;

		/// @brief Current thread in which to run task
		Threads::Type currentThread;

	private:
		/// @brief Default-constructor
		TaskConstructor(GraphTask * _owner, Threads::Type _currentThread) :
			owner(_owner),
			currentThread(_currentThread) {}

		TaskConstructor(const TaskConstructor&) = delete;
		void operator=(const TaskConstructor&) = delete;
	};

public:
	/**
	 * @brief Factory constructor method
	 */
	static 
};

#endif