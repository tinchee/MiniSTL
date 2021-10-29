#ifndef STL_STACK
#define STL_STACK

#include "stl_deque.h"

namespace ministl
{
	template<typename T, typename Sequence = _deque<T> >
	class _stack
	{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef size_t size_type;
	public:
		_stack() = default;

		reference top() { return c.back(); }
		bool empty() { return c.empty(); }
		size_type size() { return c.size(); }
		void push(const T& value) { c.push_back(value); }
		void pop() { return c.pop_back(); }

	private:
		Sequence c;
	};
}


#endif // !STL_STACK
