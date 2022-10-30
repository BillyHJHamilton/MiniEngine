#pragma once

#include "Engine/Component.h"
#include "Engine/GameObject.h"

template<typename ComponentType>
class ComponentReference
{
public:
	explicit ComponentReference(ComponentType* componentPtr)
		: m_Ptr(componentPtr)
		, m_Valid(true)
	{
		assert(componentPtr != nullptr && componentPtr->GetOwner() != nullptr);
		componentPtr->GetOwner()->EventDestroy.AddDelegate(this, &ComponentReference::OnObjectDestroyed);
	}

	bool IsValid() const { return m_Valid && m_Ptr; }
	ComponentType* Get() { return m_Valid ? m_Ptr : nullptr; }
	const ComponentType* Get() const { return m_Valid ? m_Ptr : nullptr; }

	ComponentType& operator*() { return *Get(); }
	const ComponentType& operator*() const { return *Get(); }
	ComponentType* operator->() { return Get(); }
	const ComponentType* operator->() const { return Get(); }
	operator bool () const { return IsValid(); }

	bool operator==(const ComponentReference& other) const { return m_Ptr == other.m_Ptr; }
	bool operator!=(const ComponentReference& other) const { return !operator==(other); }

	bool operator==(const ComponentType* other) const { return m_Ptr == other; }

private:
	void OnObjectDestroyed(GameObject* object)
	{
		m_Valid = false;
	}

	bool m_Valid;
	ComponentType* m_Ptr;
};
