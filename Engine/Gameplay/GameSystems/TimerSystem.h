#pragma once

#include "Engine/Gameplay/GameSystem.h"

#include <queue> // For priority queue

// Major TODO:
// This system can trigger timers on objects after they are deleted!
// It's memory corruption.
// We could require each object to clean up its timers...
// But that would require a way to delete from the priority queue.
// So we need a better approach.
// We may need "Weak Ref Safety Delegates".

class TimerSystem : public GameSystem
{
public:
	MACRO_DeclareTypeInfo(TimerSystem)

	TimerSystem()
	{
		std::cout << "Created timer system." << std::endl;
	}

	template<typename ReceiverType>
	void StartTimer(float seconds, ReceiverType* receiver, void (ReceiverType::*memberFunction)(void))
	{
		CallbackEvent<void> tempEvent;
		tempEvent.BindDelegate(receiver, memberFunction);
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
