#ifndef STL_TREE_H
#define STL_TREE_H

#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_construct.h"
#include <utility>
namespace ministl
{
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = false;
	const __rb_tree_color_type __rb_tree_black = true;
	
	// node design
	struct __rb_tree_node_base
	{
		typedef __rb_tree_node_base* base_ptr;
		typedef __rb_tree_color_type color_type;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimun(base_ptr x)
		{
			while (x->left != nullptr)
				x = x->left;
			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (x->right!=nullptr)
				x = x->right;
			return x;
		}
	};

	template<typename T>
	struct __rb_tree_node : public __rb_tree_node_base
	{
		typedef __rb_tree_node<T>* link_type;
		T value;
	};
	
	//iterator design
	struct __rb_tree_base_iterator
	{
		typedef __rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_tag;
		typedef ptrdiff_t difference_type;
		base_ptr node;

		void increment()
		{
			if (node->right != nullptr)
			{
				node = node->right;
				while (node->left != nullptr)
					node = node->left;
			}
			else
			{
				base_ptr y = node->parent;
				while (y->right == node)
				{
					node = y;
					y = y->parent;
				}
				if (node->right != y)
					node = y;
			}
		}

		void decrement()
		{
			if (node->color==__rb_tree_red&&node->parent->parent==node)
			{
				node = node->right;
			}
			else if (node->left != nullptr)
			{
				node = node->left;
				while (node->right != nullptr)
					node = node->right;
			}
			else
			{
				base_ptr y = node->parent;
				while (y->left==node)
				{
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	};

	template<typename Value,typename Ref,typename Ptr>
	struct __rb_tree_node_iterator: public __rb_tree_base_iterator
	{
		typedef Value value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_node_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_node_iterator<Value, const Value&, const Value*> const_iteraor;
		typedef __rb_tree_node_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_node<Value>* link_type;

		__rb_tree_node_iterator(link_type p) { node = p; }
		__rb_tree_node_iterator(__rb_tree_node_base* p) { node = p; }

		reference operator*() const { return ((link_type)node)->value; }
		pointer operator->() const { return &((link_type)node)->value; }
		
		self& operator++() { increment(); return *this; }
		self operator++(int)
		{
			self tmp = *this;
			increment();
			return tmp;
		}
		self& operator--() { decrement(); return *this; }
		self operator--(int)
		{
			self tmp = *this;
			decrement();
			return tmp;
		}
		bool operator==(iterator x)
		{
			return x.node == node;
		}
		bool operator!=(iterator x)
		{
			return !operator==(x);
		}
	};

	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* y = x->right;
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != nullptr)
			y->right->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;

		y->right = x;
		x->parent = y;
	}

	inline void  __rb_tree_reblance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
	{
		x->color = __rb_tree_red;
		while (x != root && x->parent->color == __rb_tree_red)
		{
			//std::cout << "ddddddddddddd" << std::endl;
			if (x->parent->parent->left == x->parent)
			{
				__rb_tree_node_base* y = x->parent->parent->right;
				if (y != nullptr && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else
				{
					//std::cout << "ccccc" << std::endl;
					if (x == x->parent->right)
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
					//std::cout << "hhhh" << std::endl;

				}
			}
			else
			{
				__rb_tree_node_base* y = x->parent->parent->left;
				if (y != nullptr && y->color == __rb_tree_red)
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black;
	}

	template<typename Key,typename Value,typename KeyOfValue,typename Compare,typename Alloc=alloc>
	class rb_tree
	{
	private:
		typedef simple_alloc<__rb_tree_node<Value>, Alloc> data_alloc;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef __rb_tree_node<Value>* link_type;
		typedef __rb_tree_node_base* base_ptr;
		typedef Value& reference;
		typedef Value* pointer;
		typedef __rb_tree_color_type color_type;
		typedef size_t size_type;
		typedef __rb_tree_node_iterator<value_type, reference, pointer> iterator;
		typedef typename __rb_tree_node_iterator<value_type, reference, pointer>::const_iteraor const_iterator;
	private:
		link_type get_node() { return (link_type)data_alloc::allocate(); }
		link_type create_node(const value_type& x)
		{
			link_type p = get_node();
			construct(&(p->value), x);
			return p;
		}
		void put_node(link_type p)
		{
			destory(&(p->value));
			destory_node(p);
		}
		void destory_node(link_type p) { data_alloc::deallocate(p); }
	private:
		size_type node_count;
		link_type header;
		Compare key_compare;
	private:
		link_type& root() { return (link_type&)header->parent; }
		link_type& leftmost(){ return (link_type&)header->left; }
		link_type& rightmost(){ return (link_type&)header->right; }
		static link_type& left(base_ptr x) { return (link_type&)(x->left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value; }
		static const Key& key(base_ptr x) { return KeyOfValue()((((link_type)x)->value)); }
		static const color_type color(base_ptr x) { return x->color; }
		static link_type minimum(link_type x) { return (link_type)x->minimun(x); }
		static link_type maximum(link_type x) { return (link_type)x->maximum(x); }
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		void init()
		{
			header = get_node();
			header->color = __rb_tree_red;
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}
	public:
		iterator insert_equal(const Value& x);
		std::pair<iterator,bool> insert_unique(const Value& x);
		iterator begin() { return leftmost(); }
		iterator end() { return header; }
		iterator find(const Key& k);
		bool empty() { return header->parent == nullptr; }
		size_type size() { return node_count; }
		rb_tree(const Compare& comp = Compare())
		{
			node_count = 0;
			key_compare=comp;
			init();
		}
	};

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k)
	{
		link_type y = header;
		link_type x = root();

		while (x != nullptr)
		{
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator 
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
	{
		link_type y = header;
		link_type x = root();
		while (x != nullptr)
		{
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x): right(x);
		}
		return __insert(x, y, x);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
	std::pair< typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
	{
		link_type y = header;
		link_type x = root();
		/*if(x!=nullptr)
			std::cout << "root is " << x->value << std::endl;*/
		bool comp = true;
		while (x != nullptr)
		{
			y = x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		//std::cout << "y is " << y->value << std::endl;
		iterator j = iterator(y);
		if (comp)
		{
			if (j == leftmost())
				return { __insert(x, y, v),true };
			else
				--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(v)))
			return { __insert(x, y, v),true };
		return std::pair<iterator, bool>{j, false};
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v)
	{
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		/*std::cout << v << " is ready to insert" << std::endl;
		std::cout << "parent is " << y->value << std::endl;*/
		if (y == header || x!=nullptr || key_compare(KeyOfValue()(v), key(y)))
		{
			//std::cout << "the insert value is " << v << std::endl;
			z = create_node(v);
			left(y) = z;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
				//leftmost() = z;
			}
			else if (y == leftmost())
				leftmost() = z;
		}
		else
		{
			z = create_node(v);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;
		}
		parent(z) = y;
		left(z) = nullptr;
		right(z) = nullptr;

		++node_count;
		__rb_tree_reblance(z, header->parent);

		//std::cout << "the root is " << root()->value << " after blance" << std::endl;
		return iterator(z);
	}
}
#endif // !STL_TREE_H