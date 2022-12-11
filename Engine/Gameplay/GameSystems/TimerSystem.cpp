#include "TimerSystem.h"
#include "Engine/App/GameApp.h"

TypeInfo const TimerSystem::s_TypeInfo = TypeInfo("TimerSystem", {"GameSystem"});

void TimerSystem::StartTimerInternal(float seconds, CallbackEvent<void>&& timerEvent)
{
	m_TimerPriorityQueue.emplace(GameApp::Get().GetClockTime() + seconds, std::move(timerEvent));
}

void TimerSystem::Tick(float deltaTime)
{
	// TODO: We should really use a total world time variable, not game clock time.
	// That would allow for possible time dilation effects.
	double currentTime = GameApp::Get().GetClockTime();

	while (!m_TimerPriorityQueue.empty()
		&& m_TimerPriorityQueue.top().m_TriggerTime <= currentTime)
	{
		// C++ standard library failed to provide any non-const access to the top element of a
		// priority queue if the element type is not copyable.  Thus, a const_cast is required.
		Timer& topTimer = const_cast<Timer&>(m_TimerPriorityQueue.top());
		topTimer.m_TimerEvent.Execute();
		m_TimerPriorityQueue.pop();
	}
}

// Note: A limitation of the current design is that once a timer is scheduled,
// it cannot be cancelled.  It is as unalterable as the law of the Medes and Persians.
// This is another irritating limitation of the std::priority_queue.
