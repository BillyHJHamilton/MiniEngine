#pragma once

#include "CoreHeader.h"

namespace CoreUtility
{
	// Utility functions for removing items from std::vector without maintaining order.

	template<typename VectorItemType>
	void RemoveSwap(std::vector<VectorItemType>& vector, int indexToRemove)
	{
		assert(indexToRemove < vector.size());
		vector[indexToRemove] = std::move(vector.back());
		vector.pop_back();
	}

	template<typename VectorItemType>
	void RemoveSwap(std::vector<VectorItemType>& vector, typename std::vector<VectorItemType>::iterator& itrToRemove)
	{
		assert(itrToRemove != vector.cend());
		auto backItr = vector.end() - 1;
		*itrToRemove = std::move(*backItr);
		vector.pop_back();
	}

	template<typename VectorItemType, typename ValueType>
	void RemoveSwapFirstMatchingItem(std::vector<VectorItemType>& vector, ValueType& value)
	{
		auto itr = std::find(vector.begin(), vector.end(), value);
		if (itr != vector.end())
		{
			RemoveSwap(vector, itr);
		}
	}

	template<typename VectorItemType, typename ValueType>
	void RemoveSwapAllMatchingItems(std::vector<VectorItemType>& vector, ValueType& value)
	{
		for (int i = 0; i < vector.size(); ++i)
		{
			if (vector[i] == value)
			{
				RemoveSwap(vector, i);
				--i;
			}
		}
	}

	template<typename VectorItemType>
	void RemoveSwapAllNullItems(std::vector<VectorItemType>& vector)
	{
		for (int i = 0; i < vector.size(); ++i)
		{
			if (vector[i] == nullptr)
			{
				RemoveSwap(vector, i);
				--i;
			}
		}
	}
	
	// Usable only if an IsValid function has been defined for the item type.
	template<typename VectorItemType>
	void RemoveSwapAllInvalidItems(std::vector<VectorItemType>& vector)
	{
		for (int i = 0; i < vector.size(); ++i)
		{
			const VectorItemType& item = vector[i];
			if (!IsValid(item))
			{
				RemoveSwap(vector, i);
				--i;
			}
		}
	}
}
