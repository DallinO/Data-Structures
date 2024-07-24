/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <stdexcept>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *         iaFront
 * ia =   0   1   2   3   4
 *      +---+---+---+---+---+
 *      |   | A | B | C |   |
 *      +---+---+---+---+---+
 * id =       0   1   2
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
    deque() : data(nullptr), numCapacity(0), numElements(0), iaFront(0) {}
   deque(int newCapacity);
   deque(const deque <T> & rhs);
   ~deque()
   { 
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque <T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
       return iterator(this, 0);
   }
   iterator end()
   {
       return iterator(this, numElements);
   }

   //
   // Access
   //
   T& front();
   T& back();
   const T & front() const;
   const T & back()  const;
   const T & operator[](size_t index) const;
   T& operator[](size_t index);

   // 
   // Insert
   //
   void push_front(const T& t);
   void push_back(const T& t);

   //
   // Remove
   //
   void clear() { numElements = 0; iaFront = 0; }
   void pop_front();
   void pop_back();

   //
   // Status
   //
   size_t size() const { return numElements; }
   bool empty() const { return numElements == 0; }

   
private:
   
    // Utility Functions
    int iaFromID(int id) const
    {
        return (iaFront + id) % numCapacity;
    };
    void resize(int newCapacity);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};



/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
    iterator() : pDeque(nullptr), id(0) {}
    iterator(custom::deque<T>* pDeque, int id) : pDeque(pDeque), id(id) {}
    iterator(const iterator& rhs) : pDeque(rhs.pDeque), id(rhs.id) {}

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
       pDeque = rhs.pDeque;
       id = rhs.id;
       return *this;
   }

   //
   // Compare
   //
   bool operator==(const iterator& rhs) const { return id == rhs.id; }
   bool operator!=(const iterator& rhs) const { return id != rhs.id; }

   // 
   // Access
   //
   const T & operator * () const
   {
       return pDeque->data[pDeque->iaFromID(id)];
   }
   T& operator * () 
   {
       return pDeque->data[pDeque->iaFromID(id)];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
       return id - it.id;
   }
   iterator& operator += (int offset)
   {
       id += offset;
       return *this;
   }
   iterator& operator ++ ()
   {
       ++id;
       return *this;
   }
   iterator operator ++ (int postfix)
   {
       iterator tmp = *this;
       ++id;
       return tmp;
   }
   iterator& operator -- ()
   {
       --id;
       return *this;
   }
   iterator  operator -- (int postfix)
   {
       iterator tmp = *this;
       --id;
       return tmp;
   }

private:

   // Member variables
   int id;             // deque index
   deque<T> *pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - non-default
 ***************************************************/
template <class T>
deque <T> :: deque(int newCapacity) : data(new T[newCapacity]), numCapacity(newCapacity), numElements(0), iaFront(0) {}

/****************************************************
 * DEQUE : CONSTRUCTOR - copy
 ***************************************************/
template <class T>
deque <T> :: deque(const deque <T> & rhs) : data(nullptr), numCapacity(0), numElements(0), iaFront(0) {
    *this = rhs;
}


/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque <T> & deque <T> :: operator = (const deque <T> & rhs)
{
    if (this != &rhs) {
        if (numCapacity < rhs.numElements) {
            delete[] data;
            data = new T[rhs.numCapacity];
            numCapacity = rhs.numCapacity;
        }

        numElements = rhs.numElements;
        iaFront = 0;

        for (size_t i = 0; i < numElements; ++i) {
            data[i] = rhs.data[rhs.iaFromID(i)];
        }
    }
    return *this;
}


/**************************************************
 * DEQUE :: FRONT
 * Fetch the item that is at the beginning of the deque
 *************************************************/
template <class T>
const T & deque <T> :: front() const 
{
    if (empty()) throw std::underflow_error("Deque is empty");
    return data[iaFront];
}
template <class T>
T& deque <T> ::front()
{
    if (empty()) throw std::underflow_error("Deque is empty");
    return data[iaFront];
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque <T> :: back() const 
{
    if (empty()) throw std::underflow_error("Deque is empty");
    return data[iaFromID(numElements - 1)];
}
template <class T>
T& deque <T> ::back()
{
    if (empty()) throw std::underflow_error("Deque is empty");
    return data[iaFromID(numElements - 1)];
}

/**************************************************
 * DEQUE :: SUBSCRIPT
 * Fetch the item in the deque
 *************************************************/
template <class T>
const T& deque <T> ::operator[](size_t index) const
{
    if (index >= numElements) throw std::out_of_range("Index out of range");
    return data[iaFromID(index)];
}
template <class T>
T& deque <T> ::operator[](size_t index)
{
    if (index >= numElements) throw std::out_of_range("Index out of range");
    return data[iaFromID(index)];
}

/*****************************************************
 * DEQUE : POP_BACK
 *****************************************************/
template <class T>
void deque <T> :: pop_back()
{
    if (empty()) throw std::underflow_error("Deque is empty");
    --numElements;
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque <T> :: pop_front()
{
    if (empty()) throw std::underflow_error("Deque is empty");
    iaFront = (iaFront + 1) % numCapacity;
    --numElements;
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque <T> :: push_back(const T & t) 
{
    if (numElements == numCapacity) resize(numCapacity == 0 ? 1 : numCapacity * 2);
    data[iaFromID(numElements)] = t;
    ++numElements;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque <T> :: push_front(const T & t) 
{
    if (numElements == numCapacity) resize(numCapacity == 0 ? 1 : numCapacity * 2);
    iaFront = (iaFront - 1 + numCapacity) % numCapacity;
    data[iaFront] = t;
    ++numElements;
}

/****************************************************
 * DEQUE :: RESIZE
 * Resize the deque so the numCapacity matches the newCapacity
 ***************************************************/
template <class T>
void deque <T> :: resize(int newCapacity) 
{
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < numElements; ++i) {
        newData[i] = data[iaFromID(i)];
    }
    delete[] data;
    data = newData;
    numCapacity = newCapacity;
    iaFront = 0;
}

} // namespace custom
