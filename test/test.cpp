#include <iostream>
#include <vector>
#include "stl_vector.h"


template<typename iter0,typename iter1>
void show(iter0 first0, iter0 end0,iter1 first1,iter1 end1)
{
    size_t sizeFirst = end0 - first0;
    size_t sizeSecond = end1 - first1;
    while (first0 != end0)
        std::cout << *first0 << ' ', first0++;
    std::cout << std::endl;
    while (first1 != end1)
        std::cout << *first1 << ' ', first1++;
    std::cout << std::endl;
}

void testForVector()
{
    std::vector<int> stdvec(5,1);
    ministl::_vector<int> minivec(5,1);

    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator,ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.pop_back();
    minivec.pop_back();
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.insert(stdvec.begin(), 2);
    minivec.insert(minivec.begin(), 2);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.insert(stdvec.begin() + 2, 3);
    minivec.insert(minivec.begin() + 2, 3);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());
    
    stdvec.erase(stdvec.begin() + 4);
    minivec.erase(minivec.begin() + 4);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.insert(stdvec.begin() + 4, 5);
    minivec.insert(minivec.begin() + 4, 5);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.push_back(6);
    minivec.push_back(6);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.erase(stdvec.begin() + 4);
    minivec.erase(minivec.begin() + 4);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.clear();
    minivec.clear();
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());

    stdvec.push_back(7);
    minivec.push_back(7);
    std::cout << "stdvec capacity is " << stdvec.capacity() << " ,minivec capacity is " << minivec.capacity() << std::endl;
    std::cout << "stdvec size is " << stdvec.size() << " ,minivec size is " << minivec.size() << std::endl;
    show<std::vector<int>::iterator, ministl::_vector<int>::iterator>(stdvec.begin(), stdvec.end(), minivec.begin(), minivec.end());
    std::cout << "stdvec front and back is " << stdvec.front() << ' ' << stdvec.back() << " ,ministd front and back is " << minivec.front() << ' ' << minivec.back() << std::endl;
}

int main()
{
    testForVector();
    return 0;
}
