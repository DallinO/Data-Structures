/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include <stdexcept>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

	template<class T>
	class priority_queue
	{
		friend class ::TestPQueue; // give the unit test class access to the privates
		template <class TT>
		friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
	public:

		//
		// construct
		//
		priority_queue() {}

		priority_queue(const priority_queue& rhs) : container(rhs.container) {}

		priority_queue(priority_queue&& rhs)
			: container(std::move(rhs.container))
		{
		}

		template <class Iterator>
		priority_queue(Iterator first, Iterator last)
			: container()
		{
			container.reserve(last - first);
			for (auto it = first; it != last; ++it)
				push(*it);
		}

		explicit priority_queue(custom::vector<T>&& rhs)
			: container(std::move(rhs))
		{
			container.reserve(rhs.size());
			for (auto it = rhs.begin(); it != rhs.end(); ++it)
				push(std::move(*it));
			heapify();
		}

		explicit priority_queue(custom::vector<T>& rhs)
			: container(rhs)
		{
			container.reserve(rhs.size());
			for (auto it = rhs.begin(); it != rhs.end(); ++it)
				push(*it);
		}

		~priority_queue() {}

		//
		// Access
		//
		const T& top() const {
			if (empty()) {
				throw std::out_of_range("std:out_of_range");
			}
			return container.front();
		}

		//
		// Insert
		//
		void push(const T& t) {
			container.push_back(t);
			size_t index = container.size() / 2;
			while (index && percolateDown(index))
				index /= 2;
		}

		void push(T&& t) {
			container.push_back(std::move(t));
			size_t index = container.size() / 2;
			while (index && percolateDown(index))
				index /= 2;
		}

		//
		// Remove
		//
		void pop() {

			if (container.empty())
			{
				return;
			}

			// Swap the first and last element
			else if (container.size() == 1)
			{
				container.clear();
				return;
			}


			// Percolate down from the root if the container is not empty
			else
			{
				std::swap(container[0], container.back());
				container.pop_back();
				percolateDown(1);
			}
		}


		//
		// Status
		//
		size_t size() const {
			return container.size();
		}

		bool empty() const {
			return container.empty();
		}

	private:

		void heapify() {
			for (size_t i = (container.size() / 2); i > 0; --i)
			{
				percolateDown(i);
			}
		}

		bool percolateDown(size_t index) {
			//int indexLeft = index * 2 + 1;  // calculate left child index
			//int indexRight = indexLeft + 1; // calculate right child index
			//int size = container.size();    // size of the priority queue

			//// Determine which child is bigger
			//int indexBigger = indexLeft;
			//if (indexRight < size && container[indexLeft] < container[indexRight]) {
			//    indexBigger = indexRight;
			//}

			//// Swap if the bigger child is greater than the parent
			//bool changeMade = false;
			//if (indexBigger < size && container[index] > container[indexBigger]) {
			//    std::swap(container[index], container[indexBigger]);
			//    changeMade = true;
			//}

			//// Recursively percolate down if a swap was made
			//if (changeMade) {
			//    return percolateDown(indexBigger);
			//}

			//return changeMade;  // Return whether a change (swap) was made


			assert(index >= 1);
			assert(index <= size());


			size_t indexLeft = 2 * index;
			if (indexLeft > size())
				return false;

			size_t indexRight = indexLeft + 1;
			size_t indexBigger = (indexRight <= size() && (container[indexLeft - 1] < container[indexRight - 1])) ? indexRight : indexLeft;

			if (container[index - 1] < container[indexBigger - 1])
			{
				std::swap(container[index - 1], container[indexBigger - 1]);
				percolateDown(indexBigger);
				return true;
			}

			return false;
		}

		custom::vector<T> container;
	};

	template<class TT>
	void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs)
	{
		std::swap(lhs.container, rhs.container);
	}

} // namespace custom
