#pragma once

#include "CoreHeader.h"

// Delegate broadcaster

// Inspired by simoncoenen.com/blog/programming/CPP_Delegates

template<typename ReturnType, typename... ArgumentList>
class IDelegate
{
public:
	virtual ReturnType Execute(ArgumentList... Arguments) = 0;
	virtual const void* GetBoundObjectPointer() const = 0;
};

template<typename ReceiverType, typename ReturnType, typename... ArgumentList>
class Delegate : public IDelegate<ReturnType, ArgumentList...>
{
public:
	using MemberFunctionType = ReturnType(ReceiverType::*)(ArgumentList...);

	Delegate(ReceiverType* NewReceiver, MemberFunctionType MemberFunctionToBind) :
		m_Receiver(NewReceiver),
		m_BoundFunction(MemberFunctionToBind)
	{}

	virtual ReturnType Execute(ArgumentList... Arguments) override
	{
		return (m_Receiver->*m_BoundFunction)(std::forward<ArgumentList>(Arguments)...);
	}

	virtual const void* GetBoundObjectPointer() const override
	{
		return static_cast<void*>(m_Receiver);
	}

private:
	ReceiverType* m_Receiver;
	MemberFunctionType m_BoundFunction;
};

// An event that binds to a single delegate.
// The delegate can be executed and its return value received.
template<typename ReturnType, typename... ArgumentList>
class CallbackEvent
{
public:
	template<typename ReceiverType>
	void BindDelegate(ReceiverType* Receiver,
		typename Delegate<ReceiverType,ReturnType,ArgumentList...>::MemberFunctionType MemberFunctionToBind)
	{
		m_Delegate = std::make_unique<Delegate<ReceiverType,ReturnType,ArgumentList...>>(Receiver, MemberFunctionToBind);
	}

	void Clear()
	{
		m_Delegate.reset();
	}

	ReturnType Execute(ArgumentList... Arguments)
	{
		return m_Delegate->Execute(Arguments...);
	}

private:
	std::unique_ptr<IDelegate<ReturnType,ArgumentList...>> m_Delegate;
};

// A multicast event.  The return type of the event must be void.
template<typename... ArgumentList>
class MulticastEvent
{
public:
	using ReturnType = void;

	template<typename ReceiverType>
	void AddDelegate(ReceiverType* receiver,
		typename Delegate<ReceiverType,ReturnType,ArgumentList...>::MemberFunctionType MemberFunctionToBind)
	{
		m_DelegateList.push_back(
			std::make_unique<Delegate<ReceiverType,ReturnType,ArgumentList...>>(receiver, MemberFunctionToBind)
		);
	}

	template<typename ReceiverType>
	void RemoveDelegatesForReceiver(const ReceiverType* receiver)
	{
		const void* const GenericObjectPointer = static_cast<const void*>(receiver);
		auto removeItr = std::remove_if(m_DelegateList.begin(), m_DelegateList.end(),
			[GenericObjectPointer](const std::unique_ptr<IDelegate<ReturnType, ArgumentList...>>& NextDelegate)
			{
				return NextDelegate->GetBoundObjectPointer() == GenericObjectPointer;
			}
		);
		m_DelegateList.erase(removeItr);
	}

	void Clear()
	{
		m_DelegateList.clear();
	}

	void Broadcast(ArgumentList... arguments)
	{
		for (std::unique_ptr<IDelegate<ReturnType, ArgumentList...>>& NextDelegate : m_DelegateList)
		{
			NextDelegate->Execute(std::forward<ArgumentList>(arguments)...);
		}
	}

private:
	std::vector<std::unique_ptr<IDelegate<ReturnType,ArgumentList...>>> m_DelegateList;
};

// TO DO: To allow binding free functions, etc., we could have the Bind function
// return an integer handle.  Each delegate should contain a handle.  Then we can
// have a function that erases a delegate by its handle.

// TO DO: We could make a delegate that uses a weak pointer to an object.
// Then it can check the weak pointer before dereferencing the object.
// That would be if we convert our engine to use shared_ptr/weak_ptr instead
// of unique_ptr.  Which we should probably do, since weak_ptrs are useful,
// and shared_ptr is no worse than unique_ptr overall for managing lifetime.

// Theoretically, I think we could make the delegate types declared inside the
// templated event class.  But then we couldn't reuse them between the CallbackEvent
// and the MulticastEvent.  There is no particular reason to expose them, but
// also probably no particular reason to hide them.

#ifdef UNIT_TESTS
void TestEvents();
#endif
