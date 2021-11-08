#ifndef STL_HASH_SET_H
#define STL_HASH_SET

#include "stl_hashtable.h"
#include "stl_hash_fun.h"
#include "stl_alloc.h"
#include "stl_function.h"
#include <cstddef>

namespace ministl
{
	template<typename Value,typename HashFcn=hash<Value>,typename EqualKey=equal_to<Value>,typename Alloc=alloc>
	class hash_set
	{
	private:
		typedef hashtable<Value, Value, HashFcn, identify<Value>, EqualKey, Alloc> ht;
		ht rep;
	public:
		typedef typename ht::iterator iterator;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef Value value_type;
	public:
		hash_set() :rep(100, hasher(), key_equal()) {}
		size_type size() { return rep.size(); }
		bool empty() { return rep.empty(); }
		iterator insert(const value_type& val) {	return rep.insert_unique(val).first; }
		iterator find(const value_type& val) { return rep.find(val); }
		void clear() { rep.clear(); }
		size_type erase(const value_type& val){ return rep.erase(val); }
		iterator begin() { return rep.begin(); }
		iterator end() { return rep.end(); }
		size_t count(const value_type& x){ return find(x)==end()? 0:1; }
	};
}




#endif // !STL_HASH_SET_H
