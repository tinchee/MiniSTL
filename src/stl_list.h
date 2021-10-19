#ifndef STL_LIST_H
#define STL_LIST_H
#include "stl_alloc.h"
#include "stl_construct.h"

namespace ministl
{
	template<typename T>
	struct __list_node
	{
		__list_node<T>* prev;
		__list_node<T>* next;
		T data;
	};

	template<typename T>
	struct __list_iter
	{
		typedef __list_node<T>* link_type;
		typedef __list_iter<T> iterator;
		typedef __list_iter<T>& reference;
		typedef T value_type;

		link_type node;
		__list_iter() :node(nullptr) {}
		__list_iter(link_type res) :node(res) {}

		reference operator++()
		{
			node = node->next;
			return *this;
		}
		reference operator--()
		{
			node = node->prev;
			return *this;
		}

		iterator operator++(int)
		{
			iterator res = *this;
			node = node->next;
			return res;
		}
		iterator operator--(int)
		{
			iterator res = *this;
			node = node->prev;
			return res;
		}

		value_type& operator*() { return node->data; }
		value_type* operator->() { return &node->data; }

		bool operator==(iterator res) { return res.node == node; }
		bool operator!=(iterator res) { return res.node != node; }
	};

	template<typename T, typename Alloc = alloc>
	class _list
	{
	public:
		typedef __list_iter<T> iterator;
		typedef __list_node<T> list_node;
		typedef __list_node<T>* link_type;
		typedef simple_alloc<list_node, Alloc> data_alloc;
		typedef T value_type;
		typedef value_type& reference;
		typedef size_t size_type;


	public:
		iterator begin() { return  node->next; }
		iterator end() { return node; }
		reference front() { return node->next->data; }
		reference back() { return node->prev->data; }
		size_type size();
		iterator insert(iterator pos, const T& value);
		void push_back(const T& value);
		void push_front(const T& value);
		iterator erase(iterator pos);
		void pop_front() { erase(begin()); };
		void pop_back() { erase(--end()); }
		void swap(_list<T, Alloc>& other);
		void merge(_list<T, Alloc>& other);
		bool empty() { return size() == 0; }
		void clear();
		void reverse();
		void unique();
		void remove(const T& value);
		void splice(iterator pos, _list<T, Alloc>& other);
		void splice(iterator pos, _list<T, Alloc>& other, iterator it);
		void splice(iterator pos, _list<T, Alloc>& other, iterator first, iterator last);
		void sort();

	public:
		explicit _list() :node(get_node()) { node->prev = node, node->next = node; };
		~_list() { clear(), destory_node(node); }

	private:
		void transfer(iterator pos, iterator first, iterator second);
		link_type get_node() { return data_alloc::allocate(); }
		void put_node(link_type p) { return data_alloc::deallocate(p); }
		link_type create_node(const value_type& x)
		{
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}
		void destory_node(link_type p)
		{
			destory(&p->data);
			put_node(p);
		}

		link_type node;
	};

	template<typename T, typename Alloc>
	typename _list<T, Alloc>::size_type _list<T, Alloc>::size()
	{
		link_type res = node->next;
		size_type distance = 0;
		while (res != node)
		{
			distance++;
			res = res->next;
		}
		return distance;
	}

	template<typename T, typename Alloc>
	typename _list<T, Alloc>::iterator _list<T, Alloc>::insert(iterator pos, const T& value)
	{
		link_type res = create_node(value);

		pos.node->prev->next = res;
		res->prev = pos.node->prev;
		res->next = pos.node;
		pos.node->prev = res;
		return res;
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::push_back(const T& value)
	{
		insert(end(), value);
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::push_front(const T& value)
	{
		insert(begin(), value);
	}

	template<typename T, typename Alloc>
	typename _list<T, Alloc>::iterator _list<T, Alloc>::erase(iterator pos)
	{
		pos.node->prev->next = pos.node->next;
		pos.node->next->prev = pos.node->prev;
		link_type res = pos.node->next;
		destory_node(pos.node);
		return res;
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::swap(_list<T, Alloc>& other)
	{
		link_type res = node;
		node = other.node;
		other.node = res;
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::clear()
	{
		link_type res = begin().node;
		while (res != node)
		{
			link_type nxt = res->next;
			destory_node(res);
			res = nxt;
		}
		node->next = node;
		node->prev = node;
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::transfer(iterator pos, iterator first, iterator second)
	{
		if (first == second)
			return;
		if (pos != second)
		{
			second.node->prev->next = pos.node;
			first.node->prev->next = second.node;
			pos.node->prev->next = first.node;
			link_type res = pos.node->prev;
			pos.node->prev = second.node->prev;
			second.node->prev = first.node->prev;
			first.node->prev = res;
		}
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::merge(_list<T, Alloc>& other)
	{
		iterator res = begin();
		iterator first = other.begin();
		iterator second = other.end();
		while (res != end() && first != second)
		{
			if (*first < *res)
			{
				iterator tmp = first;
				transfer(res, tmp, ++first); //Do not use transfer(res, first, ++first); 
				//res++;
			}
			else
				res++;
		}

		if (first != second)
		{
			transfer(end(), first, second);
			// Attention other list is empty
		}
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::reverse()
	{
		if (size() == 0 || size() == 1)
			return;
		iterator res = begin();
		++res;
		while (res != end())
		{
			iterator old = res;
			++res;
			transfer(begin(), old, res);
		}
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::unique()
	{
		if (size() == 0 || size() == 1)
			return;
		iterator res = begin();
		while (res != end())
		{
			iterator tmp = res;
			++res;
			while (res != end() && *res == *tmp)
				erase(res++);
		}
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::remove(const T& value)
	{
		iterator res = begin();
		while (res != end())
		{
			iterator tmp = res;
			if (*res == value)
				erase(res);
			res = ++tmp;
		}
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::splice(iterator pos, _list<T, Alloc>& other)
	{
		transfer(pos, other.begin(), other.end());
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::splice(iterator pos, _list<T, Alloc>& other, iterator it)
	{
		iterator res = it;
		it++;
		transfer(pos, res, it);
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::splice(iterator pos, _list<T, Alloc>& other, iterator first, iterator last)
	{
		transfer(pos, first, last);
	}

	template<typename T, typename Alloc>
	void _list<T, Alloc>::sort()
	{
		if (size() == 0 || size() == 1)
			return;
		_list<T, Alloc> carry;
		_list<T, Alloc> counter[64];
		int fill = 0;
		while (!empty())
		{
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				fill++;
		}
		for (int i = 1; i < fill; i++)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}

}



#endif // !STL_LIST_H

