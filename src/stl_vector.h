#ifndef STL_VECTOR_H
#define STL_VECTOR_H

#include "stl_alloc.h"
#include "stl_construct.h"

namespace ministl
{
	template<typename T, typename Alloc = alloc>
	class _vector
	{
	public:
		typedef T value_type;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end_of_mem - start); }
		bool empty() const { return start == finish; };
		reference operator[](size_type pos) { return *(start + pos); }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }
		void push_back(const T& value);
		void pop_back()
		{
			destory(finish - 1);
			--finish;
		}
		iterator erase(iterator pos);
		//void resize(size_type count);
		//void resize(size_type count, const T& x);
		void clear() { destory(start, finish), finish = start; }
		iterator insert(iterator pos, const T& value);
		//iterator insert(iterator pos, size_type count, const T& value);

	public:
		_vector() :start(nullptr), finish(nullptr), end_of_mem(nullptr) {}
		_vector(size_t n, const T& value) { fill_initialize(n, value); }
		_vector(int n, const T& value) { fill_initialize(n, value); }
		~_vector() { destory(start, finish), deallocate(); }
	private:
		iterator start;
		iterator finish;
		iterator end_of_mem;
	private:
		typedef simple_alloc<T, Alloc> data_allocator;
		iterator allocate_and_fill(size_t n, const T& value)
		{
			iterator res = data_allocator::allocate(n);
			iterator tmp = res;
			for (int i = 0; i < n; i++)
				construct(tmp + i, value);
			return res;
		}
		void fill_initialize(size_t n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_mem = finish;
		}
		void deallocate()
		{
			data_allocator::deallocate(start, capacity());
			start = finish = nullptr;
		}
	};

	template<typename T, typename Alloc>
	typename _vector<T, Alloc>::iterator _vector<T, Alloc>::erase(iterator pos)
	{
		iterator res = pos;
		while (pos != finish - 1)
		{
			*pos = *(pos + 1);
			pos++;
		}
		--finish;
		destory(finish);
		return pos;
	}

	template<typename T, typename Alloc>
	void _vector<T, Alloc>::push_back(const T& value)
	{
		if (capacity() >= size() + 1)
		{
			construct(finish, value);
			finish++;
		}
		else
		{
			size_type len = capacity() == 0 ? 1 : capacity() * 2;
			iterator new_start = data_allocator::allocate(len);
			iterator new_end_of_mem = new_start + len;
			iterator new_finish = new_start;
			for (int i = 0; i < capacity(); i++)
			{
				construct(new_finish, *(start + i));
				destory(start + i);
				++new_finish;
			}
			deallocate();
			construct(new_finish, value);
			++new_finish;
			start = new_start;
			finish = new_finish;
			end_of_mem = new_end_of_mem;
		}
	}

	template<typename T, typename Alloc>
	typename _vector<T, Alloc>::iterator _vector<T, Alloc>::insert(iterator pos, const T& value)
	{
		if (capacity() >= size() + 1)
		{
			iterator res = finish;
			while (res != pos)
			{
				construct(res, *(res - 1));
				res--;
				destory(res);
			}
			finish++;
			construct(pos, value);
			return pos;
		}
		else
		{
			size_type len = capacity() == 0 ? 1 : capacity() * 2;
			iterator new_start = data_allocator::allocate(len);
			iterator new_end_of_mem = new_start + len;
			iterator new_finish = new_start;
			for (iterator i = begin(); i != pos; i++)
			{
				construct(new_finish, *i);
				destory(i);
				new_finish++;
			}
			construct(new_finish++, value);
			for (iterator i = pos; i != finish; i++)
			{
				construct(new_finish, *i);
				destory(i);
				new_finish++;
			}
			start = new_start;
			finish = new_finish;
			end_of_mem = new_end_of_mem;
			return pos;
		}
	}

	/*
	template<typename T,typename Alloc>
	void _vector<T, Alloc>::resize(size_type count)
	{
		if (count < size())
		{
			iterator res = start + count;
			while (res != finish)
			{
				destory(res);
				res++;
			}
			finish = start + count;
		}
		else
		{
			resize(count, T());
		}
	}

	template<typename T,typename Alloc>
	void _vector<T, Alloc>::resize(size_type count, const T& value)
	{
		if (count < size())
			resize(count);
		else
			insert(finish, count, value);
	}
	*/
}


#endif // !STL_VECTOR_H





