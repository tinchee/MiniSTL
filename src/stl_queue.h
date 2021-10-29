#ifndef STL_QUEUE
#define STL_QUEUE

#include "stl_deque.h"

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
}


#endif // !STL_QUEUE

