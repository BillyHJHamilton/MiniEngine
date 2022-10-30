#include "Event.h"

#ifdef UNIT_TESTS
namespace EventPrivate
{
	class Example
	{
	public:
		void DoubleValue(int ValueIn, int& ValueOut)
		{
			ValueOut = 2*ValueIn;
		}

		int ReturnDoubleValue(int ValueIn)
		{
			return 2*ValueIn;
		}
	};
}

void TestEvents()
{
	EventPrivate::Example MyExample;

	MulticastEvent<int,int&> MyExampleEvent;
	MyExampleEvent.AddDelegate(&MyExample, &EventPrivate::Example::DoubleValue);
	int Output;
	MyExampleEvent.Broadcast(5, Output);
	assert(Output == 10);
	
	/*
	Example MyExample2;
	ExampleCallback MyExampleCallback;
	MyExampleCallback.BindDelegate(&MyExample2, &Example::PrintValue);
	MyExampleCallback.Execute(6);*/

	CallbackEvent<int,int> MyExampleCallback;
	MyExampleCallback.BindDelegate(&MyExample, &EventPrivate::Example::ReturnDoubleValue);
	Output = MyExampleCallback.Execute(6);
	assert(Output == 12);
	std::cout << "Output = " << Output << std::endl;
}
#endif

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------//
// First verison which uses no templates, just an example type.
//---------------------------------------------------------------------------//

/*
class Example
{
public:
	void PrintValue(int Value)
	{
		std::cout << "Example prints " << Value << std::endl;
	}

	void PrintValueConst(int Value) const
	{
		std::cout << "Const example prints " << Value << std::endl;
	}
};

class IDelegate_VoidInt
{
public:
	virtual void Execute(int Num) = 0;
	virtual const void* GetBoundObjectPointer() const = 0;
};

class ExampleDelegate_VoidInt : public IDelegate_VoidInt
{
public:
	using ReturnType = void;
	using ReceiverType = Example;
	using ArgumentList = int;
	using MemberFunctionType = ReturnType(ReceiverType::*)(ArgumentList);

	ExampleDelegate_VoidInt(ReceiverType* NewReceiver, MemberFunctionType MemberFunctionToBind) :
		m_Receiver(NewReceiver),
		m_BoundFunction(MemberFunctionToBind)
	{}

	virtual void Execute(ArgumentList Arguments) override
	{
		return (m_Receiver->*m_BoundFunction)(Arguments);
	}

	virtual const void* GetBoundObjectPointer() const override
	{
		return static_cast<void*>(m_Receiver);
	}

private:
	ReceiverType* m_Receiver;
	MemberFunctionType m_BoundFunction;
};

class ExampleCallback
{
public:
	using ReturnType = void;
	using ReceiverType = Example;
	using ArgumentList = int;
	using MemberFunctionType = ReturnType(ReceiverType::*)(ArgumentList);

	void BindDelegate(ReceiverType* Receiver, MemberFunctionType MemberFunctionToBind)
	{
		m_Delegate = std::make_unique<ExampleDelegate_VoidInt>(Receiver, MemberFunctionToBind);
	}

	void Clear()
	{
		m_Delegate.reset();
	}

	ReturnType Execute(ArgumentList Arguments)
	{
		m_Delegate->Execute(Arguments);
	}

private:
	std::unique_ptr<IDelegate_VoidInt> m_Delegate;
};

// A multicast event.  The return type of the event must be void.
class ExampleEvent
{
public:
	using ReceiverType = Example;
	using ArgumentList = int;
	using MemberFunctionType = void(ReceiverType::*)(ArgumentList);

	void AddDelegate(ReceiverType* Receiver, MemberFunctionType MemberFunctionToBind)
	{
		m_DelegateList.push_back(
			std::make_unique<ExampleDelegate_VoidInt>(Receiver, MemberFunctionToBind)
		);
	}

	void RemoveDelegatesForReceiver(const ReceiverType* Receiver)
	{
		const void* const GenericObjectPointer = static_cast<const void*>(Receiver);
		m_DelegateList.erase(std::remove_if(m_DelegateList.begin(), m_DelegateList.end(),
			[GenericObjectPointer](const std::unique_ptr<IDelegate_VoidInt>& NextDelegate)
			{
				return NextDelegate->GetBoundObjectPointer() == GenericObjectPointer;
			}
		));
	}

	void Clear()
	{
		m_DelegateList.clear();
	}

	void Broadcast(ArgumentList Arguments)
	{
		for (std::unique_ptr<IDelegate_VoidInt>& NextDelegate : m_DelegateList)
		{
			NextDelegate->Execute(Arguments);
		}
	}

private:
	std::vector<std::unique_ptr<IDelegate_VoidInt>> m_DelegateList;
};
*/

//---------------------------------------------------------------------------//
// Second version which uses a templated receiver type, but fixed signature.
//---------------------------------------------------------------------------//

/*
template<typename ReceiverType>
class Delegate_VoidInt : public IDelegate_VoidInt
{
public:
	using ReturnType = void;
	using ArgumentList = int;
	using MemberFunctionType = ReturnType(ReceiverType::*)(ArgumentList);

	Delegate_VoidInt(ReceiverType* NewReceiver, MemberFunctionType MemberFunctionToBind) :
		m_Receiver(NewReceiver),
		m_BoundFunction(MemberFunctionToBind)
	{}

	virtual void Execute(ArgumentList Arguments) override
	{
		return (m_Receiver->*m_BoundFunction)(Arguments);
	}

	virtual const void* GetBoundObjectPointer() const override
	{
		return static_cast<void*>(m_Receiver);
	}

private:
	ReceiverType* m_Receiver;
	MemberFunctionType m_BoundFunction;
};

class Event_VoidInt
{
public:
	using ReturnType = void;
	using ArgumentList = int;

	template<typename ReceiverType>
	void AddDelegate(ReceiverType* Receiver,
		typename Delegate_VoidInt<ReceiverType>::MemberFunctionType MemberFunctionToBind)
	{
		m_DelegateList.push_back(
			std::make_unique<ExampleDelegate_VoidInt>(Receiver, MemberFunctionToBind)
		);
	}

	template<typename ReceiverType>
	void RemoveDelegatesForReceiver(const ReceiverType* Receiver)
	{
		const void* const GenericObjectPointer = static_cast<const void*>(Receiver);
		m_DelegateList.erase(std::remove_if(m_DelegateList.begin(), m_DelegateList.end(),
			[GenericObjectPointer](const std::unique_ptr<IDelegate_VoidInt>& NextDelegate)
			{
				return NextDelegate->GetBoundObjectPointer() == GenericObjectPointer;
			}
		));
	}

	void Clear()
	{
		m_DelegateList.clear();
	}

	void Broadcast(ArgumentList Arguments)
	{
		for (std::unique_ptr<IDelegate_VoidInt>& NextDelegate : m_DelegateList)
		{
			NextDelegate->Execute(Arguments);
		}
	}

private:
	std::vector<std::unique_ptr<IDelegate_VoidInt>> m_DelegateList;
};
*/

