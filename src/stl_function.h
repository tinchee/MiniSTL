#ifndef STL_FUNCTION_H
#define STL_FUNCTION_H
#include "type_traits.h"
#include "stl_iterator.h"

namespace ministl {

template <typename Value> struct equal_to
{
    bool operator()(const Value &a, const Value &b)
    {
        return a == b;
    }
};

template <typename Value> struct identify
{
    const Value &operator()(const Value &a) const
    {
        return a;
    }
};

template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator second, OutputIterator result, input_iterator_tag)
{
    for (; first != second; ++first)
    {
        *result = *first;
        ++result;
    }
    return result;
}

template<typename InputIterator, typename OutputIterator,typename Distance>
inline OutputIterator __copy_d(InputIterator first, InputIterator second, OutputIterator result, Distance*)
{
    for (int i = second - first; i > 0; i--)
    {
        *result = *first;
        ++result;
        ++first;
    }
    return result;
}


template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator second, OutputIterator result, random_access_iterator_tag)
{
    return __copy_d(first, second, result, distance_type(first));
}


template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first, InputIterator second, OutputIterator result)
    {
        return __copy(first, second, result, iterator_category(first));
    }
};

template<typename T>
inline T* __copy_t(const T* first, const T* second, T* result, __true_type)
{
    memmove(result, first, sizeof(T) * (second - first));
    return result + second - first;
}

template<typename T>
inline T* __copy_t(const T* first, const T* second, T* result, __false_type)
{
    return __copy_d(first, second, result, (ptrdiff_t*)0);
}

template<typename T>
struct __copy_dispatch<T*,T>
{
    T* operator()(T* first, T* second,T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, second, t());
    }
};

template<typename T>
struct __copy_dispatch<const T*, T>
{
    T* operator()(const T* first, const T* second, const T* result)
    {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, second, t());
    }
};

template<typename InputIterator,typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator second, OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>(first, second, result);
}


char* copy(const char* first, const char* second, char* result)
{
    memmove((void*)result, first, second - first);
    return result + (second - first);
}

}
#endif
