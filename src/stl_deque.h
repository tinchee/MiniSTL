#ifndef STL_DEQUEUE
#define STL_DEQUEUE
#include "stl_alloc.h"
#include "stl_construct.h"

namespace ministl
{
	inline size_t __deque_buf_size(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}
	template<typename T/*, size_t BufSize = 0*/>
	struct __deque_iterator
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;
		typedef __deque_iterator<T> iterator;

		static size_t buffer_size() { return __deque_buf_size(/*BufSize*/0, sizeof(T)); }
		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + buffer_size();
		}
		T* cur;
		T* first;
		T* last;
		map_pointer node;

		reference operator*() { return *cur; }
		pointer operator->() { return &(*cur); }
		iterator& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		iterator& operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last - 1;
			}
			else
				--cur;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		iterator& operator+=(difference_type n)
		{
			difference_type offsize = n + (cur - first);
			if (offsize < buffer_size() && offsize >= 0)
			{
				cur = first + offsize;
				return *this;
			}
			else
			{
				difference_type	node_offsize = 0;
				if (offsize >= 0)
					node_offsize = offsize / buffer_size();
				else
					node_offsize = (offsize + 1) / buffer_size() - 1;
				set_node(node + node_offsize);
				cur = first + ((offsize + buffer_size()) % buffer_size());
				return *this;
			}
		}
		iterator& operator-=(difference_type n)
		{
			return operator+=(-n);
		}
		iterator& operator-(difference_type n)
		{
			iterator tmp = *this;
			return tmp+=(-n);
		}
		iterator& operator+(difference_type n)
		{
			iterator tmp = *this;
			return tmp+=(n);
		}
		difference_type operator-(iterator iter)
		{
			return (cur - first) + (iter.last - iter.cur) + buffer_size() * (node - iter.node - 1);
		}
		bool operator==(iterator iter)
		{
			return iter.cur == cur;
		}
		bool operator!=(iterator iter)
		{
			return !(iter == *this);
		}
	};

	template<typename T, typename Alloc = alloc>
	class _deque
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef T** map_pointer;
		typedef size_t size_type;
		typedef simple_alloc<T, alloc> data_alloc;
		typedef simple_alloc<T*, alloc> map_alloc;
		typedef __deque_iterator<T> iterator;
	public:
		_deque();
		_deque(const size_type n, const value_type& x) { init(n, x); };
		
	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }
		reference operator[](size_type n) { iterator tmp = start; return *(tmp += n); }
		bool empty() { return start == finish; }
		size_type size() { return finish - start; }
		void push_back(const T& value);
		void push_front(const T& value);
		void clear();
		iterator insert(iterator pos, const T& value);
		iterator erase(iterator pos);
		void pop_back();
		void pop_front();

	private:
		void init(const size_type n, const value_type& x);
		static size_t buffer_size() { return __deque_buf_size(0, sizeof(T)); }
		void push_back_aux(const T& x);
		void push_front_aux(const T& x);
		void reserve_map_back();
		void reserve_map_front();
		void reallocate_map(bool add_at_back);
		iterator insert_aux(iterator iter, const T& value);
		void pop_back_aux();
		void pop_front_aux();

		iterator start;
		iterator finish;
		size_t map_size;
		map_pointer map;
	};

	template<typename T,typename Alloc>
	_deque<T, Alloc>::_deque()
	{
		map_size = 1;
		map = map_alloc::allocate(map_size);
		map_pointer nstart = map;
		*nstart = data_alloc::allocate(buffer_size());
		start.set_node(nstart);
		start.cur = start.first;
		finish = start;
	}

	template<typename T, typename Alloc>
	void _deque<T, Alloc>::init(const size_type n, const value_type& x)
	{
		size_type nodes_num = n / buffer_size() + 1;
		map_size = std::max((size_type)8, nodes_num + 2);
		map = map_alloc::allocate(map_size);

		map_pointer nstart = map + (map_size - nodes_num) / 2;
		map_pointer nfinish = nstart + nodes_num - 1;

		for (map_pointer i = nstart; i <= nfinish; i++)
			*i = data_alloc::allocate(buffer_size());

		start.set_node(nstart);
		start.cur = start.first;
		finish.set_node(nfinish);
		finish.cur = finish.first + n % buffer_size();

		for (iterator iter = start; iter != finish; iter++)
			construct(iter.cur, x);
	}
	template<typename T, typename Alloc>
	void _deque<T, Alloc>::push_back(const T& value)
	{
		if (finish.cur != finish.last - 1)
		{
			construct(finish.cur, value);
			++finish;
		}
		else
			push_back_aux(value);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::push_back_aux(const T& value)
	{
		reserve_map_back();
		*(finish.node + 1) = data_alloc::allocate(buffer_size());
		construct(finish.cur, value);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::push_front(const T& value)
	{
		if (start.cur != start.first)
		{
			construct(start.cur - 1, value);
			--start.cur;
		}
		else
			push_front_aux(value);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::push_front_aux(const T& value)
	{
		reserve_map_front();
		*(start.node - 1) = data_alloc::allocate(buffer_size());
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, value);	
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::reserve_map_back()
	{
		if (finish.node - map == map_size - 1)
			reallocate_map(true);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::reserve_map_front()
	{
		if (start.node == map)
			reallocate_map(false);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::reallocate_map(bool add_at_back)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + 1;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes)
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_back ? 0 : 1);
			if (new_nstart < start.node)
			{
				map_pointer tmp = new_nstart;
				for (map_pointer i = start.node; i != finish.node; i++)
				{
					*tmp = *i;
					i = nullptr;
					tmp++;
				}
			}
			else
			{
				map_pointer tmp = new_nstart + old_num_nodes - 1;
				for (map_pointer i = finish.node; i != start.node; i--)
				{
					*tmp = *i;
					i = nullptr;
					tmp--;
				}
			}
		}
		else
		{
			map_pointer new_map = map_alloc::allocate(map_size + 1 + 2);
			new_nstart = new_map + (map_size + 1 + 2 - new_num_nodes) / 2 + (add_at_back ? 0 : 1);
			map_pointer tmp = new_nstart;
			for (map_pointer i = start.node; i != finish.node+1; i++)
			{
				*tmp = *i;
				//i = nullptr;
				tmp++;
			}
			map_alloc::deallocate(map, map_size);
			map = new_map;
			map_size = map_size + 3;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::clear()
	{
		for (map_pointer i = start.node + 1; i < finish.node; i++)
		{
			destory(*i, *i + buffer_size());
			data_alloc::deallocate(*i, buffer_size());
		}

		if (start.node != finish.node)
		{
			destory(*finish.node, *finish.node + buffer_size());
			data_alloc::deallocate(*finish.node, buffer_size());

			destory(*start.node, *start.node + buffer_size());
		}
		else
		{
			destory(start.cur, finish.cur);
		}
		finish = start;
	}

	template<typename T,typename Alloc>
	typename _deque<T, Alloc>::iterator _deque<T, Alloc>::insert(iterator iter, const T& value)
	{
		if (iter == start)
		{
			push_front(value);
			return start;
		}
		else if (iter == finish)
		{
			push_back(value);
			iterator tmp = finish;
			return --tmp;
		}
		else
			return insert_aux(iter, value);
	}

	template<typename T,typename Alloc>
	typename _deque<T, Alloc>::iterator _deque<T, Alloc>::insert_aux(iterator iter, const T& value)
	{
		push_front(value);
		iterator res = start;
		iterator tmp = res+1;
		while (tmp != iter)
		{
			*res = *tmp;
			++res;
			++tmp;
		}
		*res = value;
		return res;
	}

	template<typename T,typename Alloc>
	typename _deque<T, Alloc>::iterator _deque<T, Alloc>::erase(iterator pos)
	{
		if (pos == start)
		{
			pop_front();
			return start;
		}
		const T value = *start;
		pop_front();
		iterator res = pos - 1;
		while (res != start-1)
		{
			*(res + 1) = *res;
			--res;
		}
		*start = value;
		return pos+1;
	}

	template<typename T,typename Alloc>
	void _deque<T,Alloc>::pop_back()
	{
		if (finish.cur != finish.first)
		{
			--finish.cur;
			destory(finish.cur);
		}
		else
			pop_back_aux();
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::pop_back_aux()
	{
		data_alloc::deallocate(*finish.node, buffer_size());
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destory(finish.cur);
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::pop_front()
	{
		if (start.cur == start.last - 1)
			pop_front_aux();
		else
		{
			destory(start.cur);
			++start.cur;
		}
	}

	template<typename T,typename Alloc>
	void _deque<T, Alloc>::pop_front_aux()
	{
		destory(start.cur);
		data_alloc::deallocate(*start.node, buffer_size());
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

}
#endif
