/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author:
 *    Dallin Olson
 * Notes:
 *    I spent about three hours on this assignment. The hardest part was
 *    relearning some of the c++ syntax. I also had to familiarize myself
 *    with templates, operators, pointers and embedded classes.
 ************************************************************************/

#pragma once
#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array
{
public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() 
   { 
      /* your code goes here; it should return an iterator referring to the first element in the array */
      return iterator(__elems_); 
   }
   iterator end()   
   { 
      /* your code goes here; it should return an iterator referring to the first element off the end of the array */
      return iterator(__elems_ + N); 
   }

   // 
   // Access
   //

   // Subscript
   T& operator [] (size_t index)       
   { 
      /*your code goes here; it should return the i-th element in the array */
       return __elems_[index];
   }
   const T& operator [] (size_t index) const 
   { 
      /* your code goes here; it should return the i-th element in the array */
       return __elems_[index];
   }

   // At
   T& at(size_t index)  
   { 
      /* your code goes here; it should return the i-th element in the array */
       if (index >= N) {
           throw std::out_of_range("Index out of range");
       }
       return __elems_[index];
   }
   const T& at(size_t index) const 
   { 
      /* your code goes here; it should return the i-th element in the array */
       if (index >= N) {
           throw std::out_of_range("Index out of range");
       }
       return __elems_[index];
   }

   // Front and Back
   T& front()       
   { 
      /* your code goes here; it should return the first element in the array */
       return __elems_[0];
   }
   T& back()        
   { 
      /* your code goes here; it should return the last element in the array */
       return __elems_[N - 1];
   }
   const T& front() const 
   { 
      /* your code goes here; it should return the first element in the array */
       return __elems_[0];
   }
   const T& back()  const 
   { 
      /* your code goes here; it should return the last element in the array */
       return __elems_[N - 1];
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const 
   { 
      /* your code goes here, this should not always return 999 */
       return N;
   }
   bool empty() const 
   { 
      /* your code goes here; this should not always return TRUE */
       return N == 0;
   }
   
   //
   // Member Variable
   //

   // statically allocated array of T
   T __elems_[N];        /* your code goes here */
};



/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator
{
/*give unit tests access to the privates*/
    friend class ::TestArray;
private:
/******************************* Learning Note ********************************
Declares a pointer variable named p of type T*. The T here is a template
parameter representing the type of elements stored in the array. So T* is a
pointer to an element of type T. This pointer is used to keep track of the
current position of the iterator within the array.*/
    T * p;
public:

// constructors, destructors, and assignment operator

/******************************* Learning Note ********************************
* iterator(): This is the constructor declaration. It's a default constructor
* because it takes no arguments. Default constructors are automatically called
* when an object is created without any initialization arguments.
*
* `: p(nullptr)`: This is the member initializer list. It initializes the member
* variable p with the value nullptr, which is a null pointer. In this case, p is
* a pointer that points to the current element in the array.
*
* `{}`: This is the body of the constructor, which is empty in this case because
* all the initialization is done in the member initializer list.
*
* So, this constructor initializes the p pointer to nullptr, indicating that the
* iterator is not pointing to any valid element in the array when it's first
* constructed.*/
    iterator() : p(nullptr) {}

/******************************* Learning Note ********************************
* Pointer Constuctor */
    iterator(T * p) : p(p) {}

/******************************* Learning Note ********************************
* Declaration of the copy constructor for the iterator class.
* 
* `iterator`: This specifies the name of the constructor, which is iterator.
* 
* `(const iterator & rhs)`: This part specifies the parameter of the constructor.
* It takes a constant reference to another iterator (rhs), which is the iterator
* being copied.*/ 
    iterator(const iterator& rhs) : p(rhs.p) {}

/******************************* Learning Note ********************************
* Definition of the assignment operator `operator=` for the iterator class. 
* This operator allows you to assign one iterator to another of the same type.
* 
* `iterator&`: This specifies the return type of the assignment operator. It
* returns a reference to the current iterator object `*this`, allowing for 
* chaining of assignments.
* 
* 'operator=': This is the assignment operator itself. It's a special member
* function that is invoked when an object is assigned a new value using the `=`
* operator.
* 
* `(const iterator& rhs)`: This defines the parameter of the assignment operator.
* It takes a constant reference to another iterator `rhs` as its argument. The
* const qualifier indicates that the rhs iterator cannot be modified within the
* assignment operator. */
    iterator& operator = (const iterator& rhs)
    {
        if (this != &rhs)
        {
            p = rhs.p;
        }
        return *this;
    }
   
   // equals, not equals operator
    bool operator != (const iterator& rhs) const 
    { 
        return p != rhs.p; 
    }

    bool operator == (const iterator& rhs) const 
    { 
        return p == rhs.p;
    }
   
    // dereference operator
    T & operator * ()       
    { 
        return *p;
    }

    const T & operator * () const 
    { 
        return *p;
    }
   
/******************************* Learning Note ********************************
* The prefix increment operator is defined as a member function of the iterator
* class. When you use the prefix increment `++it`, it increments the iterator
* to point to the next element in the array and then returns a reference to the
* incremented iterator. Inside the operator, ++p increments the pointer p to
* point to the next element in the array. Finally, return *this; returns a
* reference to the updated iterator. */
    iterator & operator ++ ()
    {
        ++p;
        return *this;
    }
   
/******************************* Learning Note ********************************
* The postfix increment operator is  defined as a member function of the
* iterator class. When you use the postfix increment `it++`, it first returns a
* copy of the iterator before incrementing it to point to the next element in
* the array.
* 
* `iterator temp(*this)`: creates a copy of the current iterator.
* 
* `++(*this)`: increments the current iterator to point to the next element in
* the array.
* 
* `return temp`: returns the copy of the iterator that was made before the
increment operation. */
    iterator operator ++ (int postfix)
    {
        iterator temp(*this);
        ++(*this);
        return temp;
    }
   
};



}; // namespace custom
