/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Aaron Fox
 ************************************************************************/

#pragma once

#include <cassert>   // because I am paranoid
#include <new>       // std::bad_alloc
#include <memory>    // for std::allocator
#include <stdexcept> // for std::out_of_range
#include <algorithm> // for std::min

const char* OUT_OF_RANGE = "Vector index was out of range.";

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

    /*****************************************
     * VECTOR
     * Just like the std :: vector <T> class
     ****************************************/
    template <typename T, typename A = std::allocator<T>>
    class vector
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:

        //
        // Construct
        //
        vector(const A& a = A());
        vector(size_t numElements, const A& a = A());
        vector(size_t numElements, const T& t, const A& a = A());
        vector(const std::initializer_list<T>& l, const A& a = A());
        vector(const vector& rhs);
        vector(vector&& rhs);
        ~vector();

        //
        // Assign
        //
        void swap(vector& rhs)
        {
            std::swap(data, rhs.data);
            std::swap(numElements, rhs.numElements);
            std::swap(numCapacity, rhs.numCapacity);
        }
        vector& operator = (const vector& rhs);
        vector& operator = (vector&& rhs);

        //
        // Iterator
        //
        class iterator;
        iterator begin()
        {
            T* p = &front();
            return iterator(p);
        }
        iterator end()
        {
            T* p = &back();
            return iterator(++p);
        }

        //
        // Access
        //
        T& operator [] (size_t index);
        const T& operator [] (size_t index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        //
        // Insert
        //
        void push_back(const T& t);
        void push_back(T&& t);
        void reserve(size_t newCapacity);
        void resize(size_t newElements);
        void resize(size_t newElements, const T& t);

        //
        // Remove
        //
        void clear()
        {
            for (size_t i = numElements; i > 0; i--)
                alloc.destroy(&data[i - 1]);

            numElements = 0;
        }
        void pop_back()
        {
            if (numElements > 0)
            {
                alloc.destroy(&data[numElements - 1]);
                numElements--;
            }
        }
        void shrink_to_fit();

        //
        // Status
        //
        size_t  size()          const { return numElements; }
        size_t  capacity()      const { return numCapacity; }
        bool empty()            const { return numElements == 0; }

    private:

        A    alloc;                // use allocator for memory allocation
        T* data;                 // user data, a dynamically-allocated array
        size_t  numCapacity;       // the capacity of the array
        size_t  numElements;       // the number of items currently used
    };

    /**************************************************
     * VECTOR ITERATOR
     * An iterator through vector.  You only need to
     * support the following:
     *   1. Constructors (default and copy)
     *   2. Not equals operator
     *   3. Increment (prefix and postfix)
     *   4. Dereference
     * This particular iterator is a bi-directional meaning
     * that ++ and -- both work.  Not all iterators are that way.
     *************************************************/
    template <typename T, typename A>
    class vector <T, A> ::iterator
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:
        // constructors, destructors, and assignment operator
        iterator() { p = nullptr; }
        iterator(T* p) { this->p = p; }
        iterator(const iterator& rhs) { this->p = rhs.p; }
        iterator(size_t index, vector<T>& v) { p = v.data + index; }
        iterator& operator = (const iterator& rhs)
        {
            p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const { return !(*this == rhs); }
        bool operator == (const iterator& rhs) const { return p == rhs.p; }

        // dereference operator
        T& operator * ()
        {
            return *p;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            p++;
            return *this;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator copy(*this);
            p++;
            return copy;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            p--;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator copy(*this);
            p--;
            return *this;
        }

    private:
        T* p;
    };


    /*****************************************
     * VECTOR :: NON-DEFAULT constructors
     * non-default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(const A& a) : alloc(a), numElements(0), numCapacity(0), data(nullptr) { }

    /*****************************************
     * VECTOR :: NON-DEFAULT constructors
     * non-default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(size_t num, const T& t, const A& a) : vector<T, A>(a)
    {
        resize(num, t);
    }

    /*****************************************
     * VECTOR :: INITIALIZATION LIST constructors
     * Create a vector with an initialization list.
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(const std::initializer_list<T>& l, const A& a) : alloc(a)
    {
        numElements = l.size();
        numCapacity = numElements;
        data = alloc.allocate(numCapacity);
        std::uninitialized_copy(l.begin(), l.end(), data);
    }

    /*****************************************
     * VECTOR :: NON-DEFAULT constructors
     * non-default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(size_t num, const A& a) : vector<T, A>(a)
    {
        resize(num);
    }

    /*****************************************
     * VECTOR :: COPY CONSTRUCTOR
     * Allocate the space for numElements and
     * call the copy constructor on each element
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(const vector& rhs)
    {
        alloc = rhs.alloc;
        numCapacity = rhs.numElements;
        numElements = rhs.numElements;
        data = alloc.allocate(numCapacity);
        for (size_t i = 0; i < numElements; i++)
            alloc.construct(&data[i], rhs.data[i]);
    }

    /*****************************************
     * VECTOR :: MOVE CONSTRUCTOR
     * Steal the values from the RHS and set it to zero.
     ****************************************/
    template <typename T, typename A>
    vector <T, A> ::vector(vector&& rhs)
    {
        alloc = rhs.alloc;
        data = rhs.data;
        numElements = rhs.numElements;
        numCapacity = rhs.numCapacity;
        rhs.data = nullptr;
        rhs.numElements = 0;
        rhs.numCapacity = 0;
    }

    /*****************************************
     * VECTOR :: DESTRUCTOR
     * Call the destructor for each element from 0..numElements
     * and then free the memory
     ****************************************/
    template <typename T, typename A>
    vector <T, A> :: ~vector()
    {
        for (size_t i = numElements; i > 0; i--)
        {
            alloc.destroy(&data[i - 1]);
        }
        alloc.deallocate(data, numCapacity);
    }

    /***************************************
     * VECTOR :: RESIZE
     * This method will adjust the size to newElements.
     * This will either grow or shrink newElements.
     *     INPUT  : newCapacity the size of the new buffer
     *     OUTPUT :
     **************************************/
    template <typename T, typename A>
    void vector <T, A> ::resize(size_t newElements)
    {
        if (newElements < numElements)
        {
            for (size_t i = newElements; i < numElements; i++)
                alloc.destroy(&data[i]);
        }
        else if (newElements > numElements)
        {
            if (newElements > numCapacity)
                reserve(newElements);
            for (size_t i = numElements; i < newElements; i++)
                new (&data[i]) T();
        }
        numElements = newElements;
    }

    template <typename T, typename A>
    void vector <T, A> ::resize(size_t newElements, const T& t)
    {
        if (newElements < numElements)
        {
            for (size_t i = newElements; i < numElements; i++)
                alloc.destroy(&data[i]);
        }
        else if (newElements > numElements)
        {
            if (newElements > numCapacity)
                reserve(newElements);
            for (size_t i = numElements; i < newElements; i++)
                alloc.construct(&data[i], t);
        }
        numElements = newElements;
    }

    /***************************************
     * VECTOR :: RESERVE
     * This method will grow the current buffer
     * to newCapacity.  It will also copy all
     * the data from the old buffer into the new
     *     INPUT  : newCapacity the size of the new buffer
     *     OUTPUT :
     **************************************/
    template <typename T, typename A>
    void vector <T, A> ::reserve(size_t newCapacity)
    {
        if (newCapacity > numCapacity)
        {
            T* newData = alloc.allocate(newCapacity);
            for (size_t i = 0; i < numElements; i++)
            {
                alloc.construct(&newData[i], std::move(data[i]));
                alloc.destroy(&data[i]);
            }

            alloc.deallocate(data, numCapacity);
            data = newData;
            numCapacity = newCapacity;
        }
    }

    /***************************************
     * VECTOR :: SHRINK TO FIT
     * Get rid of any extra capacity
     *     INPUT  :
     *     OUTPUT :
     **************************************/
    template <typename T, typename A>
    void vector <T, A> ::shrink_to_fit()
    {
        if (numElements < numCapacity)
        {
            T* newData = alloc.allocate(numElements);
            for (size_t i = 0; i < numElements; i++)
            {
                alloc.construct(&newData[i], data[i]);
                alloc.destroy(&data[i]);
            }

            alloc.deallocate(data, numCapacity);
            data = newData;
            numCapacity = numElements;
        }
    }



    /*****************************************
     * VECTOR :: SUBSCRIPT
     * Read-Write access
     ****************************************/
    template <typename T, typename A>
    T& vector <T, A> :: operator [] (size_t index)
    {
        if (index >= numElements || index < 0)
            throw std::out_of_range(OUT_OF_RANGE);
        return data[index];
    }

    /******************************************
     * VECTOR :: SUBSCRIPT
     * Read-Write access
     *****************************************/
    template <typename T, typename A>
    const T& vector <T, A> :: operator [] (size_t index) const
    {
        if (index >= numElements || index < 0)
            throw std::out_of_range(OUT_OF_RANGE);
        return data[index];
    }

    /*****************************************
     * VECTOR :: FRONT
     * Read-Write access
     ****************************************/
    template <typename T, typename A>
    T& vector <T, A> ::front()
    {
        //if (numElements == 0)
        //   throw std::out_of_range(OUT_OF_RANGE);
        return data[0];
    }

    /******************************************
     * VECTOR :: FRONT
     * Read-Write access
     *****************************************/
    template <typename T, typename A>
    const T& vector <T, A> ::front() const
    {
        //if (numElements == 0)
        //   throw std::out_of_range(OUT_OF_RANGE);
        return data[0];
    }

    /*****************************************
     * VECTOR :: FRONT
     * Read-Write access
     ****************************************/
    template <typename T, typename A>
    T& vector <T, A> ::back()
    {
        if (numElements == 0)
            throw std::out_of_range(OUT_OF_RANGE);
        return data[numElements - 1];
    }

    /******************************************
     * VECTOR :: FRONT
     * Read-Write access
     *****************************************/
    template <typename T, typename A>
    const T& vector <T, A> ::back() const
    {
        if (numElements == 0)
            throw std::out_of_range(OUT_OF_RANGE);
        return data[numElements - 1];
    }

    /***************************************
     * VECTOR :: PUSH BACK
     * This method will add the element 't' to the
     * end of the current buffer.  It will also grow
     * the buffer as needed to accomodate the new element
     *     INPUT  : 't' the new element to be added
     *     OUTPUT : *this
     **************************************/
    template <typename T, typename A>
    void vector <T, A> ::push_back(const T& t)
    {
        if (numElements == numCapacity)
            reserve(numCapacity == 0 ? 1 : numCapacity * 2);

        alloc.construct(&data[numElements++], t);
    }

    template <typename T, typename A>
    void vector <T, A> ::push_back(T&& t)
    {
        if (numElements == numCapacity)
            reserve(numCapacity == 0 ? 1 : numCapacity * 2);

        alloc.construct(&data[numElements++], std::move(t));
    }

    /***************************************
     * VECTOR :: ASSIGNMENT
     * This operator will copy the contents of the
     * rhs onto *this, growing the buffer as needed
     *     INPUT  : rhs the vector to copy from
     *     OUTPUT : *this
     **************************************/
    template <typename T, typename A>
    vector <T, A>& vector <T, A> :: operator = (const vector& rhs)
    {
        if (numCapacity < rhs.numElements)
            clear();

        reserve(rhs.numElements);

        // Assign values already put in.
        for (size_t i = 0; i < std::min(numElements, rhs.numElements); i++)
            data[i] = rhs.data[i];

        // Create new values that aren't yet in.
        for (size_t i = std::min(numElements, rhs.numElements); i < rhs.numElements; i++)
            alloc.construct(&data[i], rhs.data[i]);

        // Destroy extra values.
        for (size_t i = rhs.numElements; i < numElements; i++)
            alloc.destroy(&data[i]);

        numElements = rhs.numElements;

        return *this;
    }
    template <typename T, typename A>
    vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
    {
        clear();
        data = std::move(rhs.data);
        numElements = rhs.numElements;
        numCapacity = rhs.numCapacity;

        // Reset the other vector
        rhs.data = nullptr;
        rhs.numElements = 0;
        rhs.numCapacity = 0;
        return *this;
    }




} // namespace custom

