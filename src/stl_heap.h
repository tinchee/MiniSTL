#ifndef STL_HEAP
#define STL_HEAP

#include "stl_vector.h"

namespace ministl
{
	template<typename RandomAcessIterator, typename Distance, typename T>
	void __push_heap(RandomAcessIterator first, Distance holeIndex, Distance topIndex, T value)
	{
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < value)
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template<typename RandomAccessIterator, typename Distance, typename T>
	void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator second, Distance*, T*)
	{
		__push_heap(first, Distance(second - first - 1), Distance(0), *(second - 1));
	}


	template<typename RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator second)
	{
		__push_heap_aux(first, second, distance_type(first), value_type(first));
	}

	template<typename RandomAccessIter, typename Distance, typename T>
	void __pop_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value)
	{
		Distance topIndex = holeIndex;
		Distance sonIndex = (holeIndex + 1) * 2;
		while (sonIndex < len)
		{
			if (*(first + sonIndex) > *(first + sonIndex - 1))
				*(first + holeIndex) = *(first + sonIndex), holeIndex = sonIndex;
			else
				*(first + holeIndex) = *(first + sonIndex - 1), holeIndex = sonIndex - 1;
			sonIndex = (holeIndex + 1) * 2;
		}
		if (sonIndex == len)
		{
			*(first + holeIndex) = *(first + sonIndex - 1);
			holeIndex = sonIndex - 1;
		}
		__push_heap(first, holeIndex, topIndex, value);
	}

	template<typename RandomAccessIter, typename Distance, typename T>
	void __pop_heap_aux(RandomAccessIter first, RandomAccessIter second, Distance*, T*)
	{
		T value = *(second - 1);
		*(second - 1) = *first;
		__pop_heap(first, Distance(0), Distance(second - first - 1), value);
	}


	template<typename RandomAccessIter>
	void pop_heap(RandomAccessIter first, RandomAccessIter second)
	{
		__pop_heap_aux(first, second, distance_type(first), value_type(first));
	}



	template<typename RandomAcessIter, typename Distance, typename T>
	void __make_heap(RandomAcessIter first, RandomAcessIter second, Distance*, T*)
	{
		Distance len = second - first;
		Distance parent = len / 2 - 1;
		while (true)
		{
			__pop_heap(first, parent, len, *(first + parent));
			if (parent == 0) return;
			parent--;
		}
	}


	template<typename RandomAccessIter>
	void make_heap(RandomAccessIter first, RandomAccessIter second)
	{
		__make_heap(first, second, distance_type(first), value_type(first));
	}


}

#endif // !
