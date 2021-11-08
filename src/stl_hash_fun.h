#ifndef STL_HASH_FUN_H
#define STL_HASH_FUN_H

namespace ministl
{
	template<typename Key>
	struct hash {};

	inline size_t __stl_hash_string(const char* a)
	{
		unsigned int h = 0;
		int i = 0;
		while (a[i]!='\0')
		{
			h = 5 * h + a[i];
		}
		return size_t(h);
	}
	
	template<>
	struct hash<char*>
	{
		size_t operator()(char* s) { return __stl_hash_string(s); }
	};

	template<>
	struct hash<const char*>
	{
		size_t operator()(char* s) { return __stl_hash_string(s); }
	};

	template<>
	struct hash<char>
	{
		size_t operator()(char s) { return s; }
	};

	template<>
	struct hash<int>
	{
		size_t operator()(int s) const { return s; }
	};

	template<>
	struct hash<unsigned int>
	{
		size_t operator()(unsigned int s) { return s; }
	};


}
#endif // !STL_HASH_FUN_H
