#ifndef STL_SET_H
#define STL_SET_H

#include "stl_tree.h"
#include <functional>

namespace ministl
{
	template<typename Key, typename Compare = std::less<Key>, typename Alloc = alloc>
	class _set
	{
	public:
		typedef Key key_type;
		typedef Key value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
	private:
		template<typename T>
		struct identify {
			const T& operator()(const T& x) { return x; }
		};
		typedef rb_tree<key_type, value_type, identify<value_type>, key_compare, Alloc> rep_type;
		rep_type t;
	public:
		typedef size_t size_type;
		typedef typename rep_type::iterator iterator;
	public:
		_set() :t(Compare()) {}
		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		bool empty() { return t.empty(); }
		iterator insert(const value_type& val);
		iterator find(const value_type& val);
		size_type size() { return t.size(); }
	};

	template<typename Key, typename Compare, typename Alloc>
	typename _set<Key, Compare, Alloc>::iterator _set<Key, Compare, Alloc>::insert(const value_type& val)
	{
		return t.insert_unique(val).first;
	}

	template<typename Key,typename Compare,typename Alloc>
	typename _set<Key, Compare, Alloc>::iterator _set<Key, Compare, Alloc>::find(const value_type& val)
	{
		return t.find(val);
	}
}

#endif // !STL_SET_H
