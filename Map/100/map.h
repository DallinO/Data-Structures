/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

	/*****************************************************************
	 * MAP
	 * Create a Map, similar to a Binary Search Tree
	 *****************************************************************/
	template <class K, class V>
	class map
	{
		friend ::TestMap; // give unit tests access to the privates
		//template <class KK, class VV>
		//friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
	public:
		using Pairs = custom::pair<K, V>;
		using value_type = custom::pair<K, V>;

		// 
		// Construct
		//
		map() : bst()
		{
		}

		map(const map& rhs) : bst(rhs.bst)
		{
		}

		map(map&& rhs) noexcept : bst(std::move(rhs.bst))
		{
		}

		template <class Iterator>
		map(Iterator first, Iterator last)
		{
			while (first != last)
			{
				bst.insert(*first);
				++first;
			}
		}

		map(const std::initializer_list<value_type>& il) : bst()
		{
			for (auto it = il.begin(); it != il.end(); ++it)
			{
				bst.insert(*it);
			}
		}

		~map()
		{
			clear();
		}

		// 
		   // Assign
		   //
		map& operator = (const map& rhs)
		{
			if (this != &rhs)
			{
				bst = rhs.bst;
			}
			return *this;
		}

		map& operator = (map&& rhs) noexcept
		{
			if (this != &rhs)
			{
				bst = std::move(rhs.bst);
			}
			return *this;
		}

		map& operator = (const std::initializer_list<value_type>& il)
		{
			clear();
			for (auto it = il.begin(); it != il.end(); ++it)
			{
				bst.insert(*it);
			}
			return *this;
		}

		// 
		// Iterator
		//
		class iterator;

		iterator begin()
		{
			return iterator(bst.begin());
		}

		iterator end()
		{
			return iterator(bst.end());
		}


		// 
		// Access
		//
		const V& operator[](const K& k) const
		{
			typename BST<value_type>::iterator it = bst.find(custom::make_pair(k, V()));
			if (it == bst.end())
			{
				throw std::out_of_range("Key not found");
			}
			return it->second;
		}

		V& operator[](const K& k)
		{
			// Create default pair for lookup
			Pairs pair = k, Value();
			typename BST<Pairs>::iterator it = bst.find(pair);

			// If node exists, return the value
			if (it.pNode)
				return it.pNode->data.second;

			// Add the node
			bst.insert(pair);

			// Re-find the node and return the value
			return bst.find(pair).pNode->data.second;
		}

		const V& at(const K& k) const
		{
			Pairs pair = k, Value();
			typename BST<Pairs>::iterator it = bst.find(pair);

			// If node exists, return the value
			if (it.pNode)
				return it.pNode->data.second;

			// Node doesn't exist
			throw std::out_of_range("invalid map<K, T> key");
		}

		V& at(const K& k)
		{
			// Create default pair for lookup
			Pairs pair = k, Value();
			typename BST<Pairs>::iterator it = bst.find(pair);

			// If node exists, return the value
			if (it.pNode)
				return it.pNode->data.second;

			// Node doesn't exist
			throw std::out_of_range("invalid map<K, T> key");
		}

		iterator find(const K& k)
		{
			return iterator(bst.find(custom::make_pair(k, V())));
		}

		// 
		// Insert
		//
		custom::pair<typename map::iterator, bool> insert(value_type&& rhs)
		{
			auto result = bst.insert(std::move(rhs));
			return custom::make_pair(iterator(result.first), result.second);
		}

		custom::pair<typename map::iterator, bool> insert(const value_type& rhs)
		{
			auto result = bst.insert(rhs);
			return custom::make_pair(iterator(result.first), result.second);
		}

		template <class Iterator>
		void insert(Iterator first, Iterator last)
		{
			while (first != last)
			{
				bst.insert(*first);
				++first;
			}
		}

		void insert(const std::initializer_list<value_type>& il)
		{
			for (auto it = il.begin(); it != il.end(); ++it)
			{
				bst.insert(*it);
			}
		}

		// 
		// Remove
		//
		void clear() noexcept
		{
			bst.clear();
		}

		size_t erase(const K& k)
		{
			auto it = bst.find(custom::make_pair(k, V()));
			if (it == bst.end())
			{
				return 0;
			}
			bst.erase(it);
			return 1;
		}

		iterator erase(iterator it)
		{
			return iterator(bst.erase(it.it));
		}

		iterator erase(iterator first, iterator last)
		{
			while (first != last)
			{
				first = erase(first);
			}
			return last;
		}

		//
		// Status
		//
		bool empty() const noexcept { return bst.empty(); }
		size_t size() const noexcept { return bst.size(); }


	private:

		// the students DO NOT need to use a nested class
		BST < custom::pair<K, V >> bst;
	};


	/**********************************************************
	 * MAP ITERATOR
	 * Forward and reverse iterator through a Map, just call
	 * through to BSTIterator
	 *********************************************************/
	template <typename K, typename V>
	class map <K, V> ::iterator
	{
		friend class ::TestMap; // give unit tests access to the privates
		template <class KK, class VV>
		friend class custom::map;
	public:
		//
		// Construct
		//
		iterator() : it() {}

		iterator(const typename BST<custom::pair<K, V>>::iterator& rhs) : it(rhs) {}

		iterator(const iterator& rhs) : it(rhs.it) {}

		//
		// Assign
		//
		iterator& operator=(const iterator& rhs)
		{
			if (this != &rhs)
			{
				it = rhs.it;
			}
			return *this;
		}

		//
		// Compare
		//
		bool operator==(const iterator& rhs) const { return it == rhs.it; }
		bool operator!=(const iterator& rhs) const { return it != rhs.it; }

		// 
		// Access
		//
		const custom::pair<K, V>& operator*() const
		{
			return *it;
		}

		//
		// Increment
		//
		iterator& operator++()
		{
			++it;
			return *this;
		}

		iterator operator++(int postfix)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		iterator& operator--()
		{
			--it;
			return *this;
		}

		iterator operator--(int postfix)
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}

	private:
		typename BST<custom::pair<K, V>>::iterator it;
	};

}; //  namespace custom
