#pragma once

#include <cassert>
#include <new>
#include <memory>
#include <stdexcept>

class TestVector;
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{
	// DECLARATION

	/*****************************************
	* VECTOR
	* Just like the std :: vector <T> class
	****************************************/
	template <typename T>
	class vector
	{
		friend class ::TestVector;
		friend class ::TestStack;
		friend class ::TestPQueue;
		friend class ::TestHash;
	public:

		class iterator;
		// CONSTRUCT
		vector();
		vector(size_t numElements);
		vector(size_t numElements, const T& t);
		vector(const std::initializer_list<T>& l);
		vector(const vector& rhs);
		vector(vector&& rhs);
		~vector();

		// ASSIGN
		void swap(vector& rhs);
		vector& operator = (const vector& rhs);
		vector& operator = (vector&& rhs);

		// ITERATOR
		iterator begin();
		iterator end();

		// ACCESS
		T& operator [] (size_t index);
		const T& operator [] (size_t index) const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;

		// INSERT
		void push_back(const T& t);
		void push_back(T&& t);
		void reserve(size_t newCapacity);
		void resize(size_t newElements);
		void resize(size_t newElements, const T& t);

		// REMOVE
		void clear();
		void pop_back();
		void shrink_to_fit();

		// STATUS
		size_t size() const;
		size_t capacity() const;
		bool empty() const;

	private:

		T* data;					// user data, a dynamically-allocated array
		size_t  numCapacity;		// the capacity of the array
		size_t  numElements;		// the number of items currently used
	};

	/**************************************************
	 * VECTOR ITERATOR
	 * An iterator through vector.
	 *************************************************/
	template <typename T>
	class vector<T>::iterator
	{
		friend class ::TestVector;
		friend class ::TestStack;
		friend class ::TestPQueue;
		friend class ::TestHash;

	private:
		T* p;

	public:
		iterator();
		iterator(T* p);
		iterator(const iterator& rhs);
		iterator(size_t index, vector<T>& v);
		iterator& operator=(const iterator& rhs);
		bool operator!=(const iterator& rhs) const;
		bool operator==(const iterator& rhs) const;
		T& operator*();
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
	};

	// DEFINITIONS

	// CONSTRUCT

	/*****************************************
	 * VECTOR :: DEFAULT constructors
	 * Default constructor: set the number of elements,
	 * construct each element, and copy the values over
	 ****************************************/
	template <typename T>
	vector <T> ::vector() : data(nullptr), numCapacity(0), numElements(0)
	{}

	/*****************************************
	* VECTOR :: NON-DEFAULT constructors
	* non-default constructor: set the number of elements,
	* construct each element, and copy the values over
	****************************************/
	template <typename T>
	vector <T> ::vector(size_t num) : data(nullptr), numElements(num), numCapacity(num)
	{
		if (num > size_t(0))
		{
			data = new T[num];
			for (size_t i = size_t(0); i < num; i++)
				data[i] = T();
		}
	}

	/*****************************************
	* VECTOR :: NON-DEFAULT constructors
	* non-default constructor: set the number of elements,
	* construct each element, and copy the values over
	****************************************/
	template <typename T>
	vector <T> ::vector(size_t num, const T& t) : numElements(num), numCapacity(num)
	{
		data = new T[num];
		for (size_t i = size_t(0); i < numElements; i++)
				data[i] = t;
	}




	/*****************************************
	* VECTOR :: INITIALIZATION LIST constructors
	* Create a vector with an initialization list.
	****************************************/
	template <typename T>
	vector <T> ::vector(const std::initializer_list<T>& l) : data(nullptr), numCapacity(l.size()), numElements(l.size())
	{
		data = new T[l.size()];
		size_t i = size_t(0);
		for (auto& item : l)
			data[i++] = item;
	}


	/*****************************************
	* VECTOR :: COPY CONSTRUCTOR
	* Allocate the space for numElements and
	* call the copy constructor on each element
	****************************************/
	template <typename T>
	vector <T> ::vector(const vector& rhs) : data(nullptr), numElements(rhs.numElements), numCapacity(rhs.numElements)
	{
		if (!rhs.empty())
		{
			data = new T[numCapacity];
			for (size_t i = 0; i < numElements; i++)
				data[i] = rhs.data[i];
		}
	}

	/**************************************
	* VECTOR :: MOVE CONSTRUCTOR
	* Steal the values from the RHS and set
	* it to zero.
	**************************************/
	template <typename T>
	vector <T> ::vector(vector&& rhs) : data(rhs.data), numElements(rhs.numElements), numCapacity(rhs.numCapacity)
	{
		rhs.data = nullptr;
		rhs.numElements = 0;
		rhs.numCapacity = 0;
	}


	/*****************************************
	* VECTOR :: DESTRUCTOR
	* Call the destructor for each element from
	* 0..numElements and then free the memory
	****************************************/
	template <typename T>
	vector <T> :: ~vector()
	{
		if (numCapacity > 0)
		{
			assert(nullptr != data);
			delete[] data;
		}

		/*for (size_t i = numElements; i > 0; --i) {
			data[i - 1].~T();
		}
		delete[] data;*/

	}

	// ASSIGN

	/***************************************
	 * VECTOR :: SWAP
	 * This method swaps the contents of the
	 * current vector with another vector
	 * passed as a parameter.
	 *     INPUT  : vector to be swapped with
	 *     OUTPUT :
	 **************************************/
	template <typename T>
	void vector <T> ::swap(vector& rhs)
	{
		std::swap(data, rhs.data);
		std::swap(numElements, rhs.numElements);
		std::swap(numCapacity, rhs.numCapacity);
	}


	/***************************************
	 * VECTOR :: ASSIGNMENT
	 * This operator will copy the contents of the
	 * rhs onto *this, growing the buffer as needed
	 *     INPUT  : rhs the vector to copy from
	 *     OUTPUT : *this
	 **************************************/
	template <typename T>
	vector<T>& vector<T>::operator=(const vector& rhs) {
		if (this != &rhs)
		{
			resize(rhs.numElements);
			for (size_t i = 0; i < numElements; i++) {
				data[i] = rhs.data[i];
			}
		}
		return *this;
	}


	template <typename T>
	vector<T>& vector<T>::operator=(vector&& rhs) {
		if (this != &rhs)
		{
			delete[] data;
			data = rhs.data;
			numElements = rhs.numElements;
			numCapacity = rhs.numCapacity;
			rhs.data = nullptr;
			rhs.numElements = 0;
			rhs.numCapacity = 0;
		}
		return *this;
	}


	// ITERATOR

	template <typename T>
	vector<T>::iterator::iterator() : p(nullptr) {}

	template <typename T>
	vector<T>::iterator::iterator(T* p) : p(p) {}

	template <typename T>
	vector<T>::iterator::iterator(const iterator& rhs) {*this = rhs; }

	template <typename T>
	vector<T>::iterator::iterator(size_t index, vector<T>& v) : p(&v.data[index]) { /* p = v.data + index*/ }

	template <typename T>
	typename vector<T>::iterator& vector<T>::iterator::operator=(const iterator& rhs) {
		if (this != &rhs)
			p = rhs.p;
		return *this;
	}

	template <typename T>
	bool vector<T>::iterator::operator!=(const iterator& rhs) const {
		return p != rhs.p;
	}

	template <typename T>
	bool vector<T>::iterator::operator==(const iterator& rhs) const {
		return p == rhs.p;
	}

	template <typename T>
	T& vector<T>::iterator::operator*() {
		return *p;
	}

	template <typename T>
	typename vector<T>::iterator& vector<T>::iterator::operator++() {
		++p;
		return *this;
	}

	template <typename T>
	typename vector<T>::iterator vector<T>::iterator::operator++(int) {
		iterator temp(*this);
		++p;
		return temp;
	}

	template <typename T>
	typename vector<T>::iterator& vector<T>::iterator::operator--() {
		--p;
		return *this;
	}

	template <typename T>
	typename vector<T>::iterator vector<T>::iterator::operator--(int) {
		iterator temp(*this);
		--p;
		return temp;
	}

	template <typename T>
	typename vector<T>::iterator vector<T>::begin() {
		return iterator(data);
	}

	template <typename T>
	typename vector<T>::iterator vector<T>::end() {
		return iterator(data + numElements);
	}

	// ACCESS

	/*****************************************
	 * VECTOR :: SUBSCRIPT
	 * Read-Write access
	 ****************************************/
	template <typename T>
	T& vector<T>::operator[](size_t index) {
		if (index >= numElements) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	/******************************************
	 * VECTOR :: SUBSCRIPT
	 * Read-Write access
	 *****************************************/
	template <typename T>
	const T& vector<T>::operator[](size_t index) const {
		if (index >= numElements)
		{
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	/*****************************************
	 * VECTOR :: FRONT
	 * Read-Write access
	 ****************************************/
	template <typename T>
	T& vector <T> ::front()
	{
		if (numElements == 0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return data[0];
	}

	/******************************************
	 * VECTOR :: FRONT
	 * Read-Write access
	 *****************************************/
	template <typename T>
	const T& vector <T> ::front() const
	{
		if (numElements == 0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return data[0];
	}

	/*****************************************
	 * VECTOR :: BACK
	 * Read-Write access
	 ****************************************/
	template <typename T>
	T& vector <T> ::back()
	{
		if (numElements == 0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return data[numElements - 1];
	}

	/******************************************
	 * VECTOR ::
	 * Read-Write access
	 *****************************************/
	template <typename T>
	const T& vector <T> ::back() const
	{
		if (numElements == 0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return data[numElements - 1];
	}

	// INSERT

	/***************************************
	 * VECTOR :: PUSH BACK
	 * This method will add the element 't' to the
	 * end of the current buffer.  It will also grow
	 * the buffer as needed to accomodate the new element
	 *     INPUT  : 't' the new element to be added
	 *     OUTPUT : *this
	 **************************************/
	template <typename T>
	void vector<T>::push_back(const T& t) {
		if (numElements == numCapacity)
		{
			size_t newCapacity = (numCapacity == 0) ? 1 : numCapacity * 2;
			reserve(newCapacity);
		}

		data[numElements] = t;
		++numElements;
	}


	template <typename T>
	void vector<T>::push_back(T&& t) {
		if (numElements == numCapacity)
		{
			size_t newCapacity = (numCapacity == 0) ? 1 : numCapacity * 2;
			reserve(newCapacity);
		}

		data[numElements] = std::move(t);
		++numElements;
	}

	/***************************************
	 * VECTOR :: RESIZE
	 * This method will adjust the size to newElements.
	 * This will either grow or shrink newElements.
	 *     INPUT  : newCapacity the size of the new buffer
	 *     OUTPUT :
	 **************************************/
	template <typename T>
	void vector <T> ::resize(size_t newElements)
	{
		assert(newElements >= 0);


		if (newElements > numElements)
		{
			if (newElements > numCapacity)
				reserve(newElements);

			for (size_t i = numElements; i < newElements; i++)
				data[i] = T();
		}

		numElements = newElements;


		/*if (newElements < numElements)
		{
			for (size_t i = newElements; i < numElements; i++)
			{
				data[i].~T();
			}
			numElements = newElements;
		}
		else if (newElements > numCapacity)
		{
			reserve(newElements);
			for (size_t i = numElements; i < newElements; i++)
			{
				new (&data[i]) T();
			}
			numElements = newElements;
		}
		else {

			for (size_t i = numElements; i < newElements; i++)
			{
				new (&data[i]) T();
			}
			numElements = newElements;
		}*/
	}


	template <typename T>
	void vector<T>::resize(size_t newElements, const T& t)
	{
		assert(newElements >= 0);


		if (newElements > numElements)
		{
			if (newElements > numCapacity)
				reserve(newElements);

			for (size_t i = numElements; i < newElements; i++)
				data[i] = t;
		}

		numElements = newElements;

		/*if (newElements < numElements) {
			for (size_t i = newElements; i < numElements; i++) {
				data[i].~T();
			}
			numElements = newElements;
		}
		else if (newElements > numCapacity) {

			reserve(newElements);
			for (size_t i = numElements; i < newElements; i++) {
				new (&data[i]) T(t);
			}
			numElements = newElements;
		}
		else {
			for (size_t i = numElements; i < newElements; i++) {
				new (&data[i]) T(t);
			}
			numElements = newElements;
		}*/
	}



	/***************************************
	 * VECTOR :: RESERVE
	 * This method will grow the current buffer
	 * to newCapacity.  It will also copy all
	 * the data from the old buffer into the new
	 *     INPUT  : newCapacity the size of the new buffer
	 *     OUTPUT :
	 **************************************/
	template <typename T>
	void vector<T>::reserve(size_t newCapacity) {
		if (newCapacity <= numCapacity)
			return;

		T* newData = new T[newCapacity];
		for (size_t i = 0; i < numElements; i++) {
			newData[i] = std::move(data[i]);
		}
		delete[] data;
		data = newData;
		numCapacity = newCapacity;
	}

	// REMOVE

	/***************************************
	 * VECTOR :: SHRINK TO FIT
	 * Get rid of any extra capacity
	 *     INPUT  :
	 *     OUTPUT :
	 **************************************/
	template <typename T>
	void vector<T>::shrink_to_fit() {
		/*if (numElements == numCapacity) {
			return;
		}
		T* newData = new T[numElements];
		for (size_t i = 0; i < numElements; i++) {
			newData[i] = std::move(data[i]);
		}

		delete[] data;
		data = newData;
		numCapacity = numElements;*/

		if (numCapacity == numElements)
			return;

		T* newData;
		if (numElements != 0)
		{
			newData = new T[numElements];
			for (size_t i = 0; i < numElements; i++)
				newData[i] = data[i];
		}
		else
			newData = nullptr;


		if (nullptr != data)
		{
			delete[] data;
		}

		data = newData;
		numCapacity = numElements;
	}


	// Definition of clear() function
	template <typename T>
	void vector<T>::clear() {
		/*delete[] data;
		data = nullptr;
		numCapacity = 0;*/
		numElements = 0;
	}

	// Definition of pop_back() function
	template <typename T>
	void vector<T>::pop_back() {
		/*if (numElements > 0) {
			data[numElements - 1].~T();
			--numElements;
		}*/

		if (numElements)
			--numElements;
	}


	// STATUS

	// Definition of size() function
	template <typename T>
	size_t vector<T>::size() const {
		return numElements;
	}

	// Definition of capacity() function
	template <typename T>
	size_t vector<T>::capacity() const {
		return numCapacity;
	}

	// Definition of empty() function
	template <typename T>
	bool vector<T>::empty() const {
		return numElements == 0;
	}

} // namespace custom