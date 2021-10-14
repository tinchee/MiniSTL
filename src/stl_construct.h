#include "stl_iterator.h"
#include "type_traits.h"

namespace ministl
{
	template<typename T1, typename T2>
	inline void construct(T1* p, const T2& x)
	{
		new (p) T1(x);
		return;
	}

	template<typename T>
	inline void destory(T* p)
	{
		p->~T();
		return;
	}

	template<typename Iter>
	inline void __destory_aux(Iter firstIter, Iter secondIter, __true_type)
	{
		return;
	}

	template<typename Iter>
	inline void __destory_aux(Iter firstIter, Iter secondIter, __false_type)
	{
		while (firstIter != secondIter)
		{
			//can not use value_type function, the function return a null ptr,
			//it's only represent value type in function declaration
			destory(&*firstIter);
			++firstIter;
		}
	}

	template<typename Iter, typename T>
	inline void __destory(Iter firstIter, Iter secondIter, T* p)
	{
		typedef typename __type_traits<T>::has_trivial_destructor has_trivial_destructor;
		__destory_aux(firstIter, secondIter, has_trivial_destructor());
		return;
	}

	template<typename Iter>
	inline void destory(Iter firstIter, Iter secondIter)
	{
		__destory(firstIter, secondIter, value_type(firstIter));
		return;
	}
}