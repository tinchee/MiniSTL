#ifndef STL_HASHTABLE_H
#define STL_HASHTABLE_H

#include "stl_alloc.h"
#include "stl_iterator.h"
#include <cstddef>
#include <vector>
#include "stl_hash_fun.h"

namespace ministl
{
	static const int __stl_num_primes = 12;
	static const unsigned int __stl_prime_list[__stl_num_primes] = { 53,97,193,389,769,1543,3079,6151,12289,24593,49157,98317 };


	inline unsigned int __stl_next_prime(unsigned int n)
	{
		const unsigned int* pos = std::lower_bound(__stl_prime_list, __stl_prime_list + __stl_num_primes, n);
		return pos == __stl_prime_list + __stl_num_primes ? *(__stl_prime_list + __stl_num_primes - 1) : *pos;
	}

	template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	struct __hashtable_iterator;

	template<typename Value>
	struct __hashtable_node
	{
		__hashtable_node* next;
		Value val;
	};

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
	class hashtable
	{
	public:
		friend __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef Value value_type;
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef Key key_type;
	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;
		typedef __hashtable_node<Value> node;
		typedef simple_alloc<node, Alloc> data_alloc;
		std::vector<node*> buckets;
		size_type num_elements;

	public:
		hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) { init(n); }
		hashtable() :num_elements(0) { buckets.clear(); }
		~hashtable() { clear(); }
		std::pair<iterator, bool> insert_unique(const value_type& obj);
		std::pair<iterator, bool> insert_unique_noresize(const value_type& obj);
		iterator insert_equal(const value_type& obj);
		iterator insert_equal_noresize(const value_type& obj);
		void resize(size_type num_elements_hint);
		void clear();
		size_type size() { return num_elements; }
		bool empty() { return num_elements == 0; }
		iterator find(const value_type& val);
		iterator begin();
		iterator end() { return iterator(nullptr, this); }
		size_type erase(const value_type& x);
		
	private:
		void init(size_type n);
		size_type bkt_num(const value_type& obj, size_t n) const;
		size_type bkt_num(const value_type& obj) const;
		size_type bkt_num_key(const key_type& key) const;
		size_type bkt_num_key(const key_type& key, size_t n)const;
		node* new_node(const value_type& obj);
		void delete_node(node* n);
	};

	template<typename Value,typename Key,typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	size_t hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const value_type& x)
	{
		size_t n=hash(x);
		node* first=buckets[n];
		if(equals(x,first->val))
		{
			buckets[n]=first->next;
			delete_node(first);
			--num_elements;
			return 1;
		}
		else {
			while(first->next!=nullptr)
			{
				if(equals(first->next->val,x))
				{
					node* tmp=first->next;
					first->next=first->next->next;
					delete_node(tmp);
					--num_elements;
					return 1;
				}
				first=first->next;
			}
		}
		return 0;
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::begin()
	{
		for (int i = 0; i < buckets.size(); i++)
		{
			if (buckets[i] != nullptr)
				return iterator(buckets[i], this);
		}
		return iterator(nullptr, this);
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find(const value_type& val)
	{
		size_type n = bkt_num(val);
		node* first = buckets[n];
		while (first != nullptr)
		{
			if (equals(get_key(first->val), val))
				return iterator(first, this);
			first = first->next;
		}
		return iterator(first, this);
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num_key(const key_type& key,size_t n) const
	{
		return hash(key) % n;
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num(const value_type& obj) const
	{
		return bkt_num_key(get_key(obj));
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num(const value_type& obj,size_t n) const
	{
		return bkt_num_key(get_key(obj), buckets.size());
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::bkt_num_key(const key_type& key) const
	{
		return bkt_num_key(get_key(key), buckets.size());
	}

	

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear()
	{
		for (size_type i = 0; i < buckets.size(); i++)
		{
			node* first = buckets[i];
			while (first != nullptr)
			{
				buckets[i] = first->next;
				delete_node(first);
				first = buckets[i];
			}
			buckets[i] = nullptr;
		}
		num_elements = 0;
	}
	
	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::node* 
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::new_node(const value_type& value)
	{
		node* tmp = data_alloc::allocate();
		construct(&tmp->val, value);
		return tmp;
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::delete_node(node* n)
	{
		destory(&n->val);
		data_alloc::deallocate(n);
	}
	
	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::init(size_type n)
	{
		const size_type n_buckets = __stl_next_prime(n);
		buckets.resize(n_buckets);
		for (int i = 0; i < n_buckets; i++)
			buckets[i] = nullptr;
		//buckets.insert(buckets.end(), n_buckets, nullptr);
		num_elements = 0;
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,bool>
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	std::pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj)
	{
		size_type n = bkt_num(obj);
		node* first = buckets[n];
		while (first != nullptr)
		{
			if (equals(first->val, obj))
				return std::make_pair<iterator, bool>(iterator(first, this), false);
		}

		node* tmp = new_node(obj);
		tmp->next = buckets[n];
		buckets[n] = tmp;
		++num_elements;
		return std::make_pair<iterator, bool>(iterator(tmp, this), true);
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
	{
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n)
		{
			const size_type n = __stl_next_prime(num_elements_hint);
			if (n > old_n)
			{
				std::vector<node*> tmp(n, nullptr);

				for (size_type i = 0; i < buckets.size(); i++)
				{
					node* first = buckets[i];
					while (first != nullptr)
					{
						size_type new_i = bkt_num(first->val, n);
						buckets[i] = first->next;
						first->next = tmp[new_i];
						tmp[new_i] = first;
						first = buckets[i];
					}
				}
				buckets.swap(tmp);
			}
		}
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type& obj)
	{
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}

	template <typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
		hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj)
	{
		size_type n = bkt_num(obj);
		node* first = buckets[n];
		while (first != nullptr)
		{
			if (equals(first->val, obj))
			{
				node* tmp = new_node(obj);
				tmp->next = first->next;
				first->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}

	template<typename Value,typename Key,typename HashFcn,typename ExtractKey,typename EqualKey,typename Alloc>
	struct __hashtable_iterator
	{
		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Key key_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Value& reference;
		typedef Value* pointer;

		typedef __hashtable_node<Value> node;
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hts;
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;

		node* cur;
		hts* ht;

		__hashtable_iterator(node* n, hts* tab) :cur(n), ht(tab) {}
		__hashtable_iterator() {}
		reference operator*() { return cur->val; }
		pointer operator->() { return &cur->val; }
		iterator& operator++();
		iterator operator++(int);

		bool operator==(iterator iter) { return iter.cur == cur; }
		bool operator!=(iterator iter) { return iter.cur != cur; }
	};

	template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	__hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>&
		__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
	{
		const node* old = cur;
		cur = cur->next;
		if (!cur)
		{
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}

	template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
	__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
	{
		iterator old = *this;
		++* this;
		return old;
	}
	
}

#endif // !STL_HASHTABLE_H
