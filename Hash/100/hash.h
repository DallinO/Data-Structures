/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil


class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
	/************************************************
	 * UNORDERED SET
	 * A set implemented as a hash
	 ************************************************/
	template <typename T>
	class unordered_set
	{
		friend class ::TestHash;   // give unit tests access to the privates
		friend void swap<>(unordered_set<T>& lhs, unordered_set<T>& rhs);
	public:
		//
		// Construct
		//
		unordered_set() : buckets(), numElements(0)
		{
			// Do nothing
		}
		unordered_set(unordered_set& rhs) : buckets(), numElements(rhs.numElements)
		{
			// Copy each bucket
			for (size_t i = 0; i < 10; ++i)
				buckets[i] = rhs.buckets[i];
		}
		unordered_set(unordered_set&& rhs) : buckets(), numElements(std::move(rhs.numElements))
		{
			// Copy each bucket
			for (size_t i = 0; i < 10; ++i)
				buckets[i] = std::move(rhs.buckets[i]);
		}
		template <class Iterator>
		unordered_set(Iterator first, Iterator last) : buckets(), numElements(0)
		{
			// Iterate over each item and insert them
			Iterator current = Iterator(first);
			while (current != last)
			{
				insert(*current);
				++current;
			}
		}


		//
		// Assign
		//
		unordered_set& operator=(unordered_set& rhs)
		{
			if (this != &rhs)
			{
				// Clear the current contents
				clear();

				// Copy elements
				numElements = rhs.numElements;
				for (size_t i = 0; i < 10; ++i) {
					buckets[i] = rhs.buckets[i];
				}
			}
			return *this;
		}
		unordered_set& operator=(unordered_set&& rhs)
		{
			if (this != &rhs)
			{
				clear();
				numElements = rhs.numElements;
				for (size_t i = 0; i < 10; ++i) {
					buckets[i] = std::move(rhs.buckets[i]);
				}
				rhs.numElements = 0;
			}
			return *this;
		}
		unordered_set& operator=(const std::initializer_list<T>& il)
		{
			clear();
			for (const auto& item : il) {
				insert(item);
			}
			return *this;
		}
		void swap(unordered_set& rhs)
		{
			std::swap(numElements, rhs.numElements);
			for (size_t i = 0; i < 10; ++i) {
				buckets[i].swap(rhs.buckets[i]);
			}
		}

		// 
		// Iterator
		//
		class iterator;
		class local_iterator;
		iterator begin()
		{
			// Find the first non-empty bucket
			for (size_t i = 0; i < 10; ++i) {
				if (!buckets[i].empty()) {
					// Create and return an iterator pointing to the beginning of the first non-empty bucket
					return iterator(&buckets[i], &buckets[i] + 1, buckets[i].begin());
				}
			}
			// If no non-empty buckets were found, return end()
			return end();
		}
		iterator end()
		{
			// Return an iterator that represents the end of the unordered_set
		 // Use nullptr for buckets and end of list iterator for list
			return iterator(nullptr, nullptr, typename custom::list<T>::iterator());
		}
		local_iterator begin(size_t iBucket)
		{
			if (iBucket >= 10) {
				throw std::out_of_range("Bucket index out of range");
			}
			return local_iterator(buckets[iBucket].begin());
		}
		local_iterator end(size_t iBucket)
		{
			if (iBucket >= 10) {
				throw std::out_of_range("Bucket index out of range");
			}
			return local_iterator(buckets[iBucket].end());
		}

		//
		// Access
		//
		size_t bucket(const T& t)
		{
			// Hash the element and return the appropriate bucket index
			std::hash<T> hashFn;
			return hashFn(t) % 10;
		}

		iterator find(const T& t)
		{
			size_t bucketIndex = bucket(t);
			auto it = buckets[bucketIndex].begin();
			auto endIt = buckets[bucketIndex].end();

			while (it != endIt) {
				if (*it == t) {
					return iterator(&buckets[bucketIndex], &buckets[bucketIndex] + 1, it);
				}
				++it;
			}

			return end();
		}

		//   
		// Insert
		//
		custom::pair<iterator, bool> insert(const T& t)
		{
			size_t bucketIndex = bucket(t);
			auto& bucketList = buckets[bucketIndex];

			// Check if the element already exists
			for (auto it = bucketList.begin(); it != bucketList.end(); ++it)
			{
				if (*it == t)
				{
					return { iterator(&buckets[bucketIndex], &buckets[bucketIndex] + 1, it), false };
				}
			}

			// Insert the new element
			bucketList.push_back(t);
			++numElements;

			// Return the iterator to the newly inserted element
			auto it = bucketList.end();
			--it;  // Move to the last element which is the newly inserted one
			return { iterator(&buckets[bucketIndex], &buckets[bucketIndex] + 1, it), true };
		}

		void insert(const std::initializer_list<T>& il)
		{
			for (const auto& item : il) {
				insert(item);
			}
		}


		// 
		// Remove
		//
		void clear() noexcept
		{
			for (auto& bucket : buckets) {
				bucket.clear();
			}
			numElements = 0;
		}
		iterator erase(const T& t)
		{
			size_t bucketIndex = bucket(t);
			auto& bucketList = buckets[bucketIndex];

			auto it = bucketList.begin();
			auto endIt = bucketList.end();

			while (it != endIt) {
				if (*it == t) {
					it = bucketList.erase(it);
					--numElements;
					return iterator(&buckets[bucketIndex], &buckets[bucketIndex] + 1, it);
				}
				++it;
			}

			return end();
		}

		//
		// Status
		//
		size_t size() const
		{
			return numElements;
		}
		bool empty() const
		{
			return numElements == 0;
		}
		size_t bucket_count() const
		{
			return buckets->size();
		}
		size_t bucket_size(size_t i) const
		{
			if (i >= 10) {
				throw std::out_of_range("Bucket index out of range");
			}
			return buckets[i].size();
		}

	private:

		custom::list<T> buckets[10];   // exactly 10 buckets
		int numElements;                // number of elements in the Hash
	};


	/************************************************
	 * UNORDERED SET ITERATOR
	 * Iterator for an unordered set
	 ************************************************/
	template <typename T>
	class unordered_set <T> ::iterator
	{
		friend class ::TestHash;   // give unit tests access to the privates
		template <class TT>
		friend class custom::unordered_set;
	public:
		// 
		// Construct
		//
		iterator() : pBucket(nullptr), pBucketEnd(nullptr), itList() {}
		iterator(
			typename custom::list<T>* pBucket,
			typename custom::list<T>* pBucketEnd,
			typename custom::list<T>::iterator itList)
			: pBucket(pBucket), pBucketEnd(pBucketEnd), itList(itList)
		{
			// Move to the next valid element if current element is end of the bucket
			if (itList == pBucket->end())
			{
				++(*this);
			}
		}
		iterator(const iterator& rhs)
			: pBucket(rhs.pBucket), pBucketEnd(rhs.pBucketEnd), itList(rhs.itList) {}

		//
		// Assign
		//
		iterator& operator = (const iterator& rhs)
		{
			if (this != &rhs)
			{
				pBucket = rhs.pBucket;
				pBucketEnd = rhs.pBucketEnd;
				itList = rhs.itList;
			}
			return *this;
		}

		//
		// Compare
		//
		bool operator != (const iterator& rhs) const
		{
			return !(*this == rhs);
		}
		bool operator == (const iterator& rhs) const
		{
			return pBucket == rhs.pBucket && itList == rhs.itList;
		}

		// 
		// Access
		//
		T& operator * ()
		{
			return *itList;
		}

		//
		// Arithmetic
		//
		iterator& operator ++ ()
		{
			// Move to the next item in the current bucket
			++itList;

			// If end of current bucket is reached, move to the next non-empty bucket
			while (itList == pBucket->end())
			{
				++pBucket;
				if (pBucket == pBucketEnd)
				{
					// Reached end of all buckets
					return *this;
				}
				itList = pBucket->begin();
			}
			return *this;
		}

		iterator operator ++ (int postfix)
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

	private:
		custom::list<T>* pBucket;
		custom::list<T>* pBucketEnd;
		typename list<T>::iterator itList;
	};


	/************************************************
	 * UNORDERED SET LOCAL ITERATOR
	 * Iterator for a single bucket in an unordered set
	 ************************************************/
	template <typename T>
	class unordered_set <T> ::local_iterator
	{
		friend class ::TestHash;   // give unit tests access to the privates

		template <class TT>
		friend class custom::unordered_set;
	public:
		// 
		// Construct
		//
		local_iterator() : itList() {}

		local_iterator(const typename custom::list<T>::iterator& itList)
			: itList(itList) {}

		local_iterator(const local_iterator& rhs)
			: itList(rhs.itList) {}

		//
		// Assign
		//
		local_iterator& operator=(const local_iterator& rhs)
		{
			if (this != &rhs)
			{
				itList = rhs.itList;
			}
			return *this;
		}


		// 
		// Compare
		//
		bool operator==(const local_iterator& rhs) const
		{
			return itList == rhs.itList;
		}

		bool operator!=(const local_iterator& rhs) const
		{
			return !(*this == rhs);
		}

		// 
		// Access
		//
		T& operator*()
		{
			return *itList;
		}

		// 
		// Arithmetic
		//
		local_iterator& operator++()
		{
			++itList;
			return *this;
		}

		local_iterator operator++(int)
		{
			local_iterator temp = *this;
			++(*this);
			return temp;
		}

	private:
		typename list<T>::iterator itList;
	};

	/*****************************************
	 * SWAP
	 * Stand-alone unordered set swap
	 ****************************************/
	template <typename T>
	void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
	{
		// Swap the buckets between lhs and rhs
		using std::swap;
		swap(lhs.buckets, rhs.buckets);
		swap(lhs.numElements, rhs.numElements);
	}
}