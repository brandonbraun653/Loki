#include <Chimera/threading.hpp>
#include <boost/container/static_vector.hpp>

#ifdef CHIMERA_FREERTOS

namespace Chimera
{
	namespace Threading
	{
		TaskHandle_t INIT_THREAD;
		bool setupCallbacksEnabled = true;
		static boost::container::static_vector<Thread_t, maxThreads> registeredThreads;

		/* Private Function:
		*	Implements a simple timeout while waiting for a newly created thread to complete
		*	its initialization sequence and signal back to the init thread. */
		BaseType_t threadInitTimeout(TaskHandle_t* threadHandle)
		{
			volatile BaseType_t error = pdPASS;
			TickType_t lastTimeWoken = xTaskGetTickCount();
			uint32_t timeoutCounter = 0;

			while (!ulTaskNotifyTake(pdTRUE, 0))
			{
				vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(threadInitCheckDelay_ms));
				timeoutCounter += threadInitCheckDelay_ms;

				if (timeoutCounter > maxThreadInitTimeout_ms)
				{
					error = pdFAIL;
					break;
				}
			}

			return error;
		}

		/* Private Function: Initializes all the threads registered before startScheduler() was called. */
		void initThreads(void* arguments)
		{
			volatile BaseType_t error = pdPASS;
			Thread_t thread;

			/* Create all the threads */
			for (int i = 0; i < registeredThreads.size(); i++)
			{
				thread = registeredThreads[i];
				error = xTaskCreate(thread.func, thread.name, thread.stackDepth, thread.funcParams, thread.priority, thread.handle);

				if (error == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
				{
					/* If you hit this point, one of the above tasks tried to allocate more heap space than was available. */
					volatile size_t bytesRemaining = xPortGetFreeHeapSize();
					for (;;);
				}

				/* If using initialization callbacks, wait for setup to be complete. Otherwise do nothing. Tasks are running. */
				if (setupCallbacksEnabled)
				{
					if (error == pdPASS && threadInitTimeout(thread.handle) == pdPASS)
						registeredThreads[i].handle = thread.handle;
					else
					{
						/* If you get stuck here, it's because you did not call back to this thread after initialization code was
						* completed. Call "signalThreadSetupComplete()" after setup code and just before the infinite loop. */
						vTaskSuspendAll();
						while (1);
					}
				}
			}

			/* Resume threads in the order which they were registered */
			if (setupCallbacksEnabled)
			{
				for (int i = 0; i < registeredThreads.size(); i++)
					vTaskResume(registeredThreads[i].handle);
			}

			/* Cleanly exit this thread */
			INIT_THREAD = nullptr;
			vTaskDelete(NULL);
		}

		void startScheduler(bool useSetupCallbacks)
		{
			setupCallbacksEnabled = useSetupCallbacks;
			xTaskCreate(initThreads, "thor_init", 500, NULL, 1, &INIT_THREAD);
			vTaskStartScheduler();
		}

		BaseType_t addThread(TaskFunction_t threadFunc, const char* threadName, const uint16_t stackDepth, void* const threadFuncParams,
			UBaseType_t threadPriority, TaskHandle_t* const threadHandle)
		{
			volatile BaseType_t error = pdPASS;

			if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
				error = xTaskCreate(threadFunc, threadName, stackDepth, threadFuncParams, threadPriority, threadHandle);
			else
				registeredThreads.push_back({ threadFunc, threadName, stackDepth, threadFuncParams, threadPriority, threadHandle });

			return error;
		}

		BaseType_t addThread(Thread_t& thread)
		{
			return addThread(thread.func, thread.name, thread.stackDepth, thread.funcParams, thread.priority, thread.handle);
		}

		void deleteThread(TaskHandle_t task)
		{
			vTaskDelete(task);
		}

		BaseType_t signalThreadSetupComplete()
		{
			if (setupCallbacksEnabled)
				return sendMessageAndWait(INIT_THREAD, 1u);
			else
				return pdPASS;
		}

		BaseType_t sendMessageAndWait(TaskHandle_t task, const uint32_t msg)
		{
			if (task)
			{
				xTaskNotify(task, msg, eSetValueWithOverwrite);
				vTaskSuspend(task);
				taskYIELD();
				return pdPASS;
			}
			else
				return pdFAIL;
		}

		BaseType_t sendMessage(TaskHandle_t task, const uint32_t msg)
		{
			if (task)
				return xTaskNotify(task, msg, eSetValueWithOverwrite);
			else
				return pdFAIL;
		}
	}
}

#endif 