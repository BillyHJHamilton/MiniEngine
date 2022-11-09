#pragma once

#include "Core.h"

struct Resource
{
	int x = 1;
};

class ResourceControlBlock
{
public:
	bool IsAlive()
	{
		return m_Alive;
	}
	void NotifyDead()
	{
		m_Alive = false;
		CheckDelete();
	}
	void Increment()
	{
		++m_SoftRefCount;
	}
	void Decrement()
	{
		--m_SoftRefCount;
		CheckDelete();
	}
	void CheckDelete()
	{
		if (!m_Alive && m_SoftRefCount == 0)
		{
			delete this;
		}
	}
private:
	bool m_Alive = true;
	int m_SoftRefCount = 0;
};

class ResourceOwner
{
public:
	ResourceOwner()
		: m_OwnedPtr(nullptr)
		, m_ControlBlock(nullptr)
	{}

	explicit ResourceOwner(Resource* newResource)
		: m_OwnedPtr(newResource)
		, m_ControlBlock(new ResourceControlBlock)
	{}

	~ResourceOwner()
	{
		if (IsValid())
		{
			m_ControlBlock->NotifyDead();
			delete m_OwnedPtr;
		}
	}

	ResourceOwner(ResourceOwner& other) = delete;
	ResourceOwner& operator=(ResourceOwner& other) = delete;

	ResourceOwner(ResourceOwner&& other) noexcept
		: ResourceOwner()
	{
		Swap(other);
	}
	ResourceOwner& operator=(ResourceOwner&& other) noexcept
	{
		Swap(other);
		return *this;
	}

	void Swap(ResourceOwner& other)
	{
		std::swap(m_OwnedPtr, other.m_OwnedPtr);
		std::swap(m_ControlBlock, other.m_ControlBlock);
	}

	bool operator==(const ResourceOwner& other) { return m_OwnedPtr == other.m_OwnedPtr; }
	bool operator!=(const ResourceOwner& other) { return !operator==(other); }
	bool operator==(const Resource* other) const { return m_OwnedPtr == other; }

	const Resource& operator*() const { return *m_OwnedPtr; }
	Resource& operator*() { return *m_OwnedPtr; }
	const Resource* operator->() const { return m_OwnedPtr; }
	Resource* operator->() { return m_OwnedPtr; }

	Resource* Get() { return m_OwnedPtr; }
	const Resource* Get() const { return m_OwnedPtr; }

	operator bool() const { return IsValid(); }
	bool IsValid() const { return m_OwnedPtr != nullptr; }

private:
	Resource* m_OwnedPtr;
	ResourceControlBlock* m_ControlBlock;

	friend class ResourceReference;
};

class ResourceReference
{
public:
	ResourceReference() = default;

	explicit ResourceReference(const ResourceOwner& owner)
	{
		m_Ptr = owner.m_OwnedPtr;
		m_ControlBlock = owner.m_ControlBlock;
		if (m_ControlBlock)
		{
			m_ControlBlock->Increment();
		}
	}

	// Need to think about this one a little more.
	// I think it's what we'll want for our "component references"
	//explicit ResourceReference(Resource* subObject, const ResourceOwner& owner)
	//{
	//	m_Ptr = subObject;
	//	m_ControlBlock = owner.m_ControlBlock;
	//	m_ControlBlock->Increment();
	//}

	~ResourceReference()
	{
		if (m_ControlBlock)
		{
			m_ControlBlock->Decrement();
		}
	}

	ResourceReference(const ResourceReference& other)
	{
		m_Ptr = other.m_Ptr;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->Increment();
	}
	ResourceReference& operator=(ResourceReference other)
	{
		Swap(other);
		return *this;
	}

	ResourceReference& operator=(const ResourceOwner &owner)
	{
		*this = ResourceReference(owner);
		return *this;
	}

	ResourceReference(ResourceReference&& other) noexcept
		: ResourceReference()
	{
		Swap(other);
	}

	void Swap(ResourceReference& other)
	{
		std::swap(m_Ptr, other.m_Ptr);
		std::swap(m_ControlBlock, other.m_ControlBlock);
	}

	bool operator==(const ResourceReference& other) { return m_Ptr == other.m_Ptr; }
	bool operator!=(const ResourceReference& other) { return !operator==(other); }

	bool operator==(const Resource* other) const { return m_Ptr == other; }

	const Resource& operator*() const { return *Get(); }
	Resource& operator*() { return *Get(); }
	const Resource* operator->() const { return Get(); }
	Resource* operator->() { return Get(); }

	Resource* Get() { return IsValid() ? m_Ptr : nullptr; }
	const Resource* Get() const { return IsValid() ? m_Ptr : nullptr; }

	operator bool() const { return IsValid(); }

	bool IsValid() const
	{
		return m_Ptr != nullptr
			&& m_ControlBlock != nullptr
			&& m_ControlBlock->IsAlive();
	}

private:
	Resource* m_Ptr = nullptr;
	ResourceControlBlock* m_ControlBlock = nullptr;
};

#if UNIT_TESTS
void TestReferences();
#endif
