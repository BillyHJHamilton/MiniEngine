#pragma once

#include "CoreHeader.h"

// TODO: These are probably not thread-safe.
// TODO: Support const ref.
// TODO: Conversion between types of ref.

class RefControlBlock
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

// A soft reference attached to an external control block.
// The reference checks the control block to see if it is valid.
template<typename T>
class WeakRef
{
public:
	WeakRef() = default;

	explicit WeakRef(RefControlBlock* controlBlock, T* resource)
	{
		m_Ptr = resource;
		m_ControlBlock = controlBlock;
		if (m_ControlBlock)
		{
			m_ControlBlock->Increment();
		}
	}

	~WeakRef()
	{
		if (m_ControlBlock)
		{
			m_ControlBlock->Decrement();
		}
	}

	WeakRef(const WeakRef& other)
	{
		m_Ptr = other.m_Ptr;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->Increment();
	}
	WeakRef& operator=(WeakRef other)
	{
		Swap(other);
		return *this;
	}

	WeakRef(WeakRef&& other) noexcept
		: WeakRef()
	{
		Swap(other);
	}

	void Swap(WeakRef& other)
	{
		std::swap(m_Ptr, other.m_Ptr);
		std::swap(m_ControlBlock, other.m_ControlBlock);
	}

	bool operator==(const WeakRef& other)
	{
		// These operations must check validity.
		// Otherwise, we could match to a new object with the same address as an invalid object;
		return (IsValid() && (m_Ptr == other.m_Ptr))
			|| (!IsValid() && !other.IsValid());
	}
	bool operator!=(const WeakRef& other)
	{
		return (IsValid() && (m_Ptr == other.m_Ptr))
			|| (!IsValid() && !other.IsValid());
	}

	bool operator==(const T* other) const
	{
		return (IsValid() || m_Ptr == nullptr)
			&& (m_Ptr == other);
	}

	const T& operator*() const { return *Get(); }
	T& operator*() { return *Get(); }
	const T* operator->() const { return Get(); }
	T* operator->() { return Get(); }

	T* Get() { return IsValid() ? m_Ptr : nullptr; }
	const T* Get() const { return IsValid() ? m_Ptr : nullptr; }

	operator bool() const { return IsValid(); }

	bool IsValid() const
	{
		return m_Ptr != nullptr
			&& m_ControlBlock != nullptr
			&& m_ControlBlock->IsAlive();
	}

	void Reset()
	{
		if (m_ControlBlock)
		{
			m_ControlBlock->Decrement();
		}
		m_Ptr = nullptr;
		m_ControlBlock = nullptr;
	}

private:
	T* m_Ptr = nullptr;
	RefControlBlock* m_ControlBlock = nullptr;
};

// Pointer that manages the lifetime of an object and supports weak references.
// Essentially similar to a "unique_ptr" that can also issue "weak_ptr"s.
template<typename T>
class RefOwner
{
public:
	RefOwner()
		: m_OwnedPtr(nullptr)
		, m_ControlBlock(nullptr)
	{}

	explicit RefOwner(T* newResource)
		: m_OwnedPtr(newResource)
		, m_ControlBlock(new RefControlBlock)
	{}

	~RefOwner()
	{
		if (IsValid())
		{
			m_ControlBlock->NotifyDead();
			delete m_OwnedPtr;
		}
	}

	RefOwner(RefOwner& other) = delete;
	RefOwner& operator=(RefOwner& other) = delete;

	RefOwner(RefOwner&& other) noexcept
		: RefOwner()
	{
		Swap(other);
	}
	RefOwner& operator=(RefOwner&& other) noexcept
	{
		Swap(other);
		return *this;
	}

	void Swap(RefOwner& other)
	{
		std::swap(m_OwnedPtr, other.m_OwnedPtr);
		std::swap(m_ControlBlock, other.m_ControlBlock);
	}

	// Get a weak reference to the object managed by this TOwner.
	WeakRef<T> GetReference() const
	{
		return WeakRef<T>(m_ControlBlock, m_OwnedPtr);
	}
	explicit operator WeakRef<T>() const
	{
		return GetReference();
	}

	// Issue a reference that will be invalidated when the resource managed by this RefOwner is deleted.
	// By requesting a sub-reference managed by this RefOwner, you make the promise that the lifetime
	// of the referenced object is equal to the lifetime of this RefOwner's managed resource.
	// For example, you could safely issue a reference to a member of the managed resource.
	template<typename SubobjectType>
	WeakRef<SubobjectType> MakeSubReference(SubobjectType* subobject) const
	{
		return WeakRef<SubobjectType>(m_ControlBlock, subobject);
	}

	bool operator==(const RefOwner& other) { return m_OwnedPtr == other.m_OwnedPtr; }
	bool operator!=(const RefOwner& other) { return !operator==(other); }
	bool operator==(const T* other) const { return m_OwnedPtr == other; }

	const T& operator*() const { return *m_OwnedPtr; }
	T& operator*() { return *m_OwnedPtr; }
	const T* operator->() const { return m_OwnedPtr; }
	T* operator->() { return m_OwnedPtr; }

	T* Get() { return m_OwnedPtr; }
	const T* Get() const { return m_OwnedPtr; }

	operator bool() const { return IsValid(); }
	bool IsValid() const { return m_OwnedPtr != nullptr; }

private:
	T* m_OwnedPtr;
	RefControlBlock* m_ControlBlock;
};

// A reference counter without a managed object.
// Allocates a control block when created, and invalidates it when destroyed.
// You can include RefTracker as a member of an object for which you want to issue weak references.
// You can safely use RefTracker to issue references to the owning object, or to any object with the same lifetime.
// Essentially an alternative to std::weak_from_this, for objects not managed by a shared_ptr.
class RefTracker
{
public:
	RefTracker()
		: m_ControlBlock(new RefControlBlock)
	{}

	~RefTracker()
	{
		m_ControlBlock->NotifyDead();
	}

	RefTracker(RefTracker& other) = delete;
	RefTracker& operator=(RefTracker& other) = delete;

	RefTracker(RefTracker&& other) noexcept
		: RefTracker()
	{
		Swap(other);
	}
	RefTracker& operator=(RefTracker&& other) noexcept
	{
		Swap(other);
		return *this;
	}

	void Swap(RefTracker& other)
	{
		std::swap(m_ControlBlock, other.m_ControlBlock);
	}

	// Issue a reference that will be invalidated when this RefTracker is deleted.
	// By requesting a reference managed by this RefTracker, you make the promise that the lifetime
	// of the object to which it refers will be equal to the lifetime of this RefTracker.
	template<typename T>
	WeakRef<T> MakeReference(T* resource) const
	{
		return WeakRef<T>(m_ControlBlock, resource);
	}

private:
	RefControlBlock* m_ControlBlock;
};

#if UNIT_TESTS
void TestReferences();
#endif
