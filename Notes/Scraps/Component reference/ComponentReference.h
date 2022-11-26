#pragma once

#include "Engine/Component.h"
#include "Engine/GameObject.h"

template<typename ComponentType>
class ComponentReference
{
public:
	ComponentReference() = default;

	explicit ComponentReference(ComponentType* componentPtr)
		: m_Ptr(componentPtr)
	{
		RegisterDelegate();
	}

	ComponentReference(const ComponentReference& other)
		: ComponentReference(other.m_Ptr) {}

	ComponentReference& operator=(const ComponentReference& other)
	{
		if (m_Ptr != other.m_Ptr)
		{
			UnregisterDelegate();
			m_Ptr = other.m_Ptr;
			RegisterDelegate();
		}
		return *this;
	}

	//ComponentReference& operator=(ComponentReference&& other) = delete;
	//ComponentReference(ComponentReference&& other) = delete;

	~ComponentReference()
	{
		UnregisterDelegate();
	}

	bool IsValid() const { return m_Ptr != nullptr; }
	ComponentType* Get() { return m_Ptr; }
	const ComponentType* Get() const { return m_Ptr; }

	ComponentType& operator*() { return *Get(); }
	const ComponentType& operator*() const { return *Get(); }
	ComponentType* operator->() { return Get(); }
	const ComponentType* operator->() const { return Get(); }
	operator bool () const { return IsValid(); }

	bool operator==(const ComponentReference& other) const { return m_Ptr == other.m_Ptr; }
	bool operator!=(const ComponentReference& other) const { return !operator==(other); }

	bool operator==(const ComponentType* other) const { return m_Ptr == other; }

private:
	void RegisterDelegate()
	{
		if (m_Ptr != nullptr)
		{
			assert(m_Ptr->GetOwner());
			m_Ptr->GetOwner()->EventDestroy.AddDelegate(this, &ComponentReference::OnObjectDestroyed);
		}
	}

	void UnregisterDelegate()
	{
		if (m_Ptr != nullptr)
		{
			m_Ptr->GetOwner()->EventDestroy.RemoveDelegatesForReceiver(this);
		}
	}

	void OnObjectDestroyed(GameObject* object)
	{
		m_Ptr = nullptr;
	}

	ComponentType* m_Ptr = nullptr;
};
