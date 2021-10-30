#ifndef STL_QUEUE
#define STL_QUEUE

#include "stl_deque.h"
#include "stl_vector.h"
#include "stl_heap.h"

namespace ministl
{
	template<typename T, typename Squence = _deque<T> >
	class _queue
	{
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;
	public:
		_queue() = default;

		reference front() { return c.front(); }
		void pop() { c.pop_front(); }
		void push(const value_type& value) { c.push_back(value); }
		size_type size() { return c.size(); }
		bool empty() { return c.empty(); }
		reference back() { return c.back(); }
	private:
		Squence c;
	};

	template<typename T,typename Squence=_vector<T>>
	class priority_queue
	{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef size_t size_type;
	public:
		//priority_queue() :c() {}
	public:
		const_reference top() { return c.front(); }
		bool empty() { return c.empty(); }
		size_type size() { return c.size(); }
		void push(const T& value)
		{
			c.push_back(value);
			push_heap(c.begin(), c.end());
		}
		void pop()
		{
			pop_heap(c.begin(), c.end());
			c.pop_back();
		}
	private:
		Squence c;
	};
}


#endif // !STL_QUEUE

