#ifndef STL_FUNCTION_H
#define STL_FUNCTION_H

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

}
#endif
