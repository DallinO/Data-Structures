/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

	/**************************************************
	 * LIST
	 * Just like std::list
	 **************************************************/
	template <typename T>
	class list
	{
		friend class ::TestList; // give unit tests access to the privates
		friend class ::TestHash;
	public:

		// Default constructor
		list() : numElements(0), pHead(nullptr), pTail(nullptr) {}

		// Copy constructor
		list(const list<T>& rhs) : numElements(0), pHead(nullptr), pTail(nullptr) {
			for (Node* p = rhs.pHead; p; p = p->pNext) {
				push_back(p->data);
			}
		}

		// Move constructor
		list(list<T>&& rhs) : numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail) {
			rhs.numElements = 0;
			rhs.pHead = nullptr;
			rhs.pTail = nullptr;
		}

		// Constructor with number of elements and a value
		list(size_t num, const T& t) : numElements(0), pHead(nullptr), pTail(nullptr) {
			for (size_t i = 0; i < num; ++i) {
				push_back(t);
			}
		}

		// Constructor with number of elements (default value-initialized)
		list(size_t num) : numElements(0), pHead(nullptr), pTail(nullptr) {
			for (size_t i = 0; i < num; ++i) {
				push_back(T());
			}
		}

		// Initializer list constructor
		list(const std::initializer_list<T>& il) : numElements(0), pHead(nullptr), pTail(nullptr) {
			for (const T& item : il) {
				push_back(item);
			}
		}

		// Range constructor
		template <class Iterator>
		list(Iterator first, Iterator last) : numElements(0), pHead(nullptr), pTail(nullptr) {
			for (auto it = first; it != last; ++it) {
				push_back(*it);
			}
		}

		// Destructor
		~list() {
			clear();
		}

		// Copy assignment operator
		list<T>& operator=(const list<T>& rhs) {
			if (this != &rhs) {
				list temp(rhs);
				swap(temp);
			}
			return *this;
		}

		// Move assignment operator
		list<T>& operator=(list<T>&& rhs) {
			if (this != &rhs) {
				clear();
				numElements = rhs.numElements;
				pHead = rhs.pHead;
				pTail = rhs.pTail;
				rhs.numElements = 0;
				rhs.pHead = nullptr;
				rhs.pTail = nullptr;
			}
			return *this;
		}

		// Initializer list assignment operator
		list<T>& operator=(const std::initializer_list<T>& il) {
			clear();
			for (const T& item : il) {
				push_back(item);
			}
			return *this;
		}

		// Swap function
		void swap(list<T>& rhs) {
			std::swap(numElements, rhs.numElements);
			std::swap(pHead, rhs.pHead);
			std::swap(pTail, rhs.pTail);
		}

		void swap(list<T>& lhs, list<T>& rhs) {
			using std::swap;
			swap(lhs.numElements, rhs.numElements);
			swap(lhs.pHead, rhs.pHead);
			swap(lhs.pTail, rhs.pTail);
		}


		// Iterator methods
		class  iterator;
		iterator begin() { return iterator(pHead); }
		iterator rbegin() { return iterator(pTail); }
		iterator end() { return iterator(nullptr); }


		// Accessors
		T& front() {
			if (pHead) {
				return pHead->data;
			}
			throw "ERROR: unable to access data from an empty list";
		}

		T& back() {
			if (pTail) {
				return pTail->data;
			}
			throw "ERROR: unable to access data from an empty list";
		}

		// Modifiers
		void push_front(const T& data) {
			Node* newNode = new Node(data);
			newNode->pNext = pHead;
			if (pHead) {
				pHead->pPrev = newNode;
			}
			pHead = newNode;
			if (!pTail) {
				pTail = pHead;
			}
			++numElements;
		}

		void push_front(T&& data) {
			Node* newNode = new Node(std::move(data));
			newNode->pNext = pHead;
			if (pHead) {
				pHead->pPrev = newNode;
			}
			pHead = newNode;
			if (!pTail) {
				pTail = pHead;
			}
			++numElements;
		}

		void push_back(const T& data) {
			Node* newNode = new Node(data);
			newNode->pPrev = pTail;
			if (pTail) {
				pTail->pNext = newNode;
			}
			pTail = newNode;
			if (!pHead) {
				pHead = pTail;
			}
			++numElements;
		}

		void push_back(T&& data) {
			Node* newNode = new Node(std::move(data));
			newNode->pPrev = pTail;
			if (pTail) {
				pTail->pNext = newNode;
			}
			pTail = newNode;
			if (!pHead) {
				pHead = pTail;
			}
			++numElements;
		}

		iterator insert(iterator it, const T& data) {
			if (it.p == nullptr) {
				push_back(data);
				return iterator(pTail);
			}
			else if (it.p == pHead) {
				push_front(data);
				return iterator(pHead);
			}
			else {
				Node* newNode = new Node(data);
				newNode->pNext = it.p;
				newNode->pPrev = it.p->pPrev;
				it.p->pPrev->pNext = newNode;
				it.p->pPrev = newNode;
				++numElements;
				return iterator(newNode);
			}
		}

		iterator insert(iterator it, T&& data) {
			if (it.p == nullptr) {
				push_back(std::move(data));
				return iterator(pTail);
			}
			else if (it.p == pHead) {
				push_front(std::move(data));
				return iterator(pHead);
			}
			else {
				Node* newNode = new Node(std::move(data));
				newNode->pNext = it.p;
				newNode->pPrev = it.p->pPrev;
				it.p->pPrev->pNext = newNode;
				it.p->pPrev = newNode;
				++numElements;
				return iterator(newNode);
			}
		}

		void pop_front() {
			if (pHead) {
				Node* temp = pHead;
				pHead = pHead->pNext;
				if (pHead) {
					pHead->pPrev = nullptr;
				}
				else {
					pTail = nullptr;
				}
				delete temp;
				--numElements;
			}
		}

		void pop_back() {
			if (pTail) {
				Node* temp = pTail;
				pTail = pTail->pPrev;
				if (pTail) {
					pTail->pNext = nullptr;
				}
				else {
					pHead = nullptr;
				}
				delete temp;
				--numElements;
			}
		}

		void clear() {
			while (pHead) {
				pop_front();
			}
		}

		iterator erase(const iterator& it) {
			if (it.p == nullptr) {
				return end();
			}

			Node* pNode = it.p;
			iterator nextIterator(pNode->pNext);

			// If pNode is the head node
			if (pNode == pHead) {
				pHead = pNode->pNext;
				if (pHead) {
					pHead->pPrev = nullptr;
				}
			}
			else {
				pNode->pPrev->pNext = pNode->pNext;
			}

			// If pNode is the tail node
			if (pNode == pTail) {
				pTail = pNode->pPrev;
				if (pTail) {
					pTail->pNext = nullptr;
				}
			}
			else {
				pNode->pNext->pPrev = pNode->pPrev;
			}

			delete pNode;
			--numElements;

			return nextIterator;
		}

		// Capacity
		bool empty() const { return numElements == 0; }

		size_t size() const { return numElements; }



	private:
		// nested linked list class
		class Node;

		size_t numElements;
		Node* pHead;
		Node* pTail;
	};

	/*************************************************
	 * NODE
	 * the node class.  Since we do not validate any
	 * of the setters, there is no point in making them
	 * private.  This is the case because only the
	 * List class can make validation decisions
	 *************************************************/
	template <typename T>
	class list <T> ::Node
	{
	public:

		// MEMBER VARIABLES
		T data;
		Node* pNext;
		Node* pPrev;

		// CONSTRUCTORS
		Node() : pNext(nullptr), pPrev(nullptr) {}
		Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
		Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}
	};

	/*************************************************
	 * LIST ITERATOR
	 * Iterate through a List, non-constant version
	 ************************************************/
	template <typename T>
	class list <T> ::iterator
	{
		friend class ::TestList; // give unit tests access to the privates
		friend class ::TestHash;
		template <typename TT>
		friend class custom::list;
	public:
		// CONSTRUCTORS
		iterator() : p(nullptr) {}
		iterator(Node* p) : p(p) {}
		iterator(const iterator& rhs) : p(rhs.p) {}

		// OPERATORS
		//
		// Assignment operator
		iterator& operator=(const iterator& rhs) {
			if (this != &rhs) {
				p = rhs.p;
			}
			return *this;
		}

		// Equals, not equals operator
		bool operator == (const iterator& rhs) const { return true; }
		bool operator != (const iterator& rhs) const { return true; }

		T& operator * ()
		{
			return p->data;
		}

		// Dereference operator, fetch a node
		T* operator->()
		{
			return &(p->data);
		}

		// Postfix increment
		iterator operator++(int)
		{
			iterator temp = *this;
			if (p) {
				p = p->pNext;
			}
			return temp;
		}

		// Prefix increment
		iterator& operator++()
		{
			if (p) {
				p = p->pNext;
			}
			return *this;
		}

		// Postfix decrement
		iterator operator--(int)
		{
			iterator temp = *this;
			if (p) {
				p = p->pPrev;
			}
			return temp;
		}

		// Prefix decrement
		iterator& operator--()
		{
			if (p) {
				p = p->pPrev;
			}
			return *this;
		}

		// two friends who need to access p directly
		friend iterator list <T> ::insert(iterator it, const T& data);
		friend iterator list <T> ::insert(iterator it, T&& data);
		friend iterator list <T> ::erase(const iterator& it);

	private:

		typename list <T> ::Node* p;
	};

	
	//#endif
}; // namespace custom
