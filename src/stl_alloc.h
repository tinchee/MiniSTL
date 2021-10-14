#include <new>

namespace ministl
{
	// the first level alloc
	template<int inst>
	class __malloc_alloc_template
	{
	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t n);
	private:
		static void* oom_malloc(size_t n);
		static void* oom_relloc(void* p, size_t n);
		static void (*oom_handler)();
	};

	template<int inst>
	void (*__malloc_alloc_template<inst>::oom_handler)() = nullptr;

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		while (true)
		{
			if (oom_handler == nullptr)
			{
				throw std::bad_alloc();
				break;
			}
			oom_handler();
			void* p = allocate(n);
			if (p != nullptr)
				return p;
		}
	}

	template<int inst>
	void* __malloc_alloc_template<inst>::allocate(size_t n)
	{
		void* p = malloc(n);
		if (p == nullptr)
			p = oom_malloc(n);
		return p;
	}

	template<int inst>
	void __malloc_alloc_template<inst>::deallocate(void* p, size_t)
	{
		free(p);
	}

	template<int inst>
	void* __malloc_alloc_template<inst>::oom_relloc(void* p, size_t n)
	{
		while (true)
		{
			if (oom_handler == nullptr)
			{
				throw std::bad_alloc();
				break;
			}
			oom_handler();
			void* res = realloc(p, n);
			if (res != nullptr)
				return res;
		}
	}

	template<int inst>
	void* __malloc_alloc_template<inst>::reallocate(void* p, size_t n)
	{
		void* res = realloc(p, n);
		if (res == nullptr)
			res = oom_relloc(res, n);
		return res;
	}

	typedef __malloc_alloc_template<0> malloc_alloc;


	// the second level alloc

	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum { __NUM_ALIGN = __MAX_BYTES / __ALIGN };


	template<bool thread, int inst>
	class __default_alloc_template
	{
	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		//static void* reallocate(void* p, size_t n);
	private:
		static char* start_mem_pool;  // void* can not sub
		static char* end_mem_pool;
		static void* refill(size_t n);
		static char* chunk_fill(size_t n, int& njobs);
		union obj
		{
			union obj* free_list_link;
			char client_data[1];
		};
		static obj* free_list[__NUM_ALIGN];
		static size_t ROUND_UP(size_t n)
		{
			size_t res = (n + __ALIGN - 1) / __ALIGN * __ALIGN;
			return res;
		}
		static size_t CAL_INDEX(size_t n)
		{
			size_t index = (n + __ALIGN - 1) / __ALIGN - 1;
			return index;
		}
	};

	template<bool thread, int inst>
	typename __default_alloc_template<thread, inst>::obj*
		__default_alloc_template<thread, inst>::free_list[__NUM_ALIGN] = { nullptr,nullptr,nullptr,nullptr,
																		nullptr,nullptr, nullptr,nullptr,
																		nullptr,nullptr, nullptr,nullptr,
																		nullptr,nullptr,nullptr,nullptr };
	template<bool thread, int inst>
	char* __default_alloc_template<thread, inst>::end_mem_pool = nullptr;
	template<bool thread, int inst>
	char* __default_alloc_template<thread, inst>::start_mem_pool = nullptr;

	template<bool thread, int inst>
	void* __default_alloc_template<thread, inst>::allocate(size_t n)
	{
		if (n > (size_t)__MAX_BYTES)
		{
			void* res = malloc_alloc::allocate(n);
			return res;
		}
		obj** my_free_list = free_list + CAL_INDEX(n);
		obj* result = *my_free_list;
		if (result == nullptr)
		{
			void* res = refill(n);
			return res;
		}
		else
		{
			*my_free_list = result->free_list_link;
			return result;
		}

	}

	template<bool thread, int inst>
	void* __default_alloc_template<thread, inst>::refill(size_t n)
	{
		int njobs = 8;
		char* chunk = chunk_fill(n, njobs);

		if (njobs == 1)
			return chunk;
		obj** my_free_list = free_list + CAL_INDEX(n);
		char* result = chunk;

		*my_free_list = (obj*)(chunk + n);
		char* next = chunk + n;
		char* current = nullptr;

		for (int i = 1; i < njobs; i++)
		{
			current = next;
			next = ((char*)next + n);
			if (i == njobs - 1)
				((obj*)current)->free_list_link = nullptr;
			else
				((obj*)current)->free_list_link = (obj*)next;
		}
		return result;
	}

	template<bool thread, int inst>
	char* __default_alloc_template<thread, inst>::chunk_fill(size_t n, int& njobs)
	{
		size_t total_n = n * njobs;
		size_t free_n = end_mem_pool - start_mem_pool;

		if (free_n >= total_n)  //enough
		{
			char* res = start_mem_pool;
			start_mem_pool = start_mem_pool + total_n;
			return res;
		}
		else if (free_n >= n)  //only one njobs
		{
			njobs = free_n / n;
			char* res = start_mem_pool;
			start_mem_pool = start_mem_pool + njobs * n;
			return res;
		}
		else  //can not pay one njobs
		{
			if (free_n > 0)
			{
				obj** my_free_list = free_list + CAL_INDEX(free_n);
				obj* current = *my_free_list;
				*my_free_list = (obj*)start_mem_pool;
				(*my_free_list)->free_list_link = current;
				start_mem_pool = end_mem_pool = nullptr;
			}

			size_t get_n = 2 * total_n;
			start_mem_pool = (char*)malloc(get_n);
			if (start_mem_pool == nullptr)
			{
				for (size_t i = n; i <= __MAX_BYTES; i += __ALIGN)
				{
					obj** my_free_list = free_list + i;
					obj* res = *my_free_list;
					if (res != nullptr)
					{
						*my_free_list = res->free_list_link;
						start_mem_pool = (char*)res;
						end_mem_pool = start_mem_pool + i;
						chunk_fill(n, njobs);
					}
				}
				start_mem_pool = (char*)malloc_alloc::allocate(get_n);
			}
			end_mem_pool = start_mem_pool + get_n;
			return chunk_fill(n, njobs);
		}
	}

	template<bool thread, int inst>
	void __default_alloc_template<thread, inst>::deallocate(void* p, size_t n)
	{
		if (n > 128)
		{
			malloc_alloc::deallocate(p, n);
			return;
		}
		else
		{
			obj** my_free_list = free_list + CAL_INDEX(n);
			obj* res = *my_free_list;
			*my_free_list = (obj*)p;
			((obj*)p)->free_list_link = res;
		}
		return;
	}

	typedef __default_alloc_template<false, 0> alloc;


	template<typename T, typename Alloc>
	class simple_alloc
	{
	public:
		static T* allocate(size_t n)
		{
			T* res = (T*)Alloc::allocate(n * sizeof(T));
			return res;
		}
		static T* allocate()
		{
			T* res = (T*)Alloc::allocate(sizeof(T));
			return res;
		}
		static void deallocate(T* p, size_t n)
		{
			Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T* p)
		{
			Alloc::deallocate(p, sizeof(T));
		}
	};
}

