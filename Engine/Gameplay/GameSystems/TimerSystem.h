#pragma once

#include "Engine/Gameplay/GameSystem.h"

#include <queue> // For priority queue

// This system can be used to request execution of a function at a future time.
// A priority queue is used to efficiently check only the next timer that will trigger.

class TimerSystem : public GameSystem
{
public:
	MACRO_DeclareTypeInfo(TimerSystem)

	// Set a timer to execute a member function on an object.
	// The object must be provided as a weak reference to avoid a callback on a deleted object.
	template<typename ReceiverType>
	void StartTimer(float seconds, WeakRef<ReceiverType> receiver, void (ReceiverType::*memberFunction)(void))
	{
		CallbackEvent<void> tempEvent;
		tempEvent.BindWeakRef(std::move(receiver), memberFunction);
		StartTimerInternal(seconds, std::move(tempEvent));
	}

	// Set a timer to execute a free function.
	void StartTimer(float seconds, void (*freeFunction)(void))
	{
		CallbackEvent<void> tempEvent;
		tempEvent.BindFreeFunction(freeFunction);
		StartTimerInternal(seconds, std::move(tempEvent));
	}

	virtual void Tick(float deltaTime) override;

private:
	void StartTimerInternal(float seconds, CallbackEvent<void>&& timerEvent);

	struct Timer
	{
		Timer(double triggerTime, CallbackEvent<void>&& timerEvent)
			: m_TriggerTime(triggerTime)
			, m_TimerEvent(std::move(timerEvent))
		{}

		double m_TriggerTime;
		CallbackEvent<void> m_TimerEvent;

		bool operator>(const Timer& other) const { return m_TriggerTime > other.m_TriggerTime; }
	};

	std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer>> m_TimerPriorityQueue;
};
