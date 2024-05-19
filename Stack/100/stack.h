/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
    template<class T>
    class stack
    {
        friend class ::TestStack; // give unit tests access to the privates
    public:

        stack() : container() {}
        stack(const stack<T>& rhs) : container(rhs.container) {}
        stack(stack<T>&& rhs) noexcept : container(std::move(rhs.container)) {}
        stack(const vector<T>& rhs) : container(rhs) {}
        stack(vector<T>&& rhs) noexcept : container(std::move(rhs)) {}
        ~stack() = default;

        stack<T>& operator=(const stack<T>& rhs)
        {
            container = rhs.container;
            return *this;
        }
        stack<T>& operator=(stack<T>&& rhs) noexcept
        {
            container = std::move(rhs.container);
            return *this;
        }
        void swap(stack<T>& rhs)
        {
            container.swap(rhs.container);
        }

        T& top()
        {
            return container.back();
        }
        const T& top() const
        {
            return container.back();
        }
        void push(const T& t)
        {
            container.push_back(t);
        }
        void push(T&& t)
        {
            container.push_back(std::move(t));
        }
        void pop()
        {
            container.pop_back();
        }
        size_t size() const
        {
            return container.size();
        }
        bool empty() const
        {
            return container.empty();
        }

    private:

        vector<T> container;  // underlying container
    };




} // custom namespace


