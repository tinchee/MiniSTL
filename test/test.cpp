#include <iostream>
#include <vector>
#include "stl_vector.h"
#include <list>
#include <string>
#include "stl_list.h"

template<typename iter0,typename iter1>
void show(iter0 first0, iter0 end0,iter1 first1,iter1 end1)
{
    //size_t sizeFirst = end0 - first0;
    //size_t sizeSecond = end1 - first1;
    while (first0 != end0)
        std::cout << *first0 << ' ', first0++;
    std::cout << std::endl;
    while (first1 != end1)
        std::cout << *first1 << ' ', first1++;
    if (first1 == end1 && first1 != nullptr)
        std::cout << std::endl;
}

void printTestedApi(std::string str)
{
    std::cout << "Testing for " << str << "..." << std::endl;
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
    std::cout << "stdvec front and back is" << stdvec.front() << ' ' << stdvec.back() << " ,ministd front and back is" << minivec.front() << ' ' << minivec.back() << std::endl;
}



void testForList()
{
    ministl::_list<int> minilist;
    std::list<int> stdlist;

    ministl::_list<int>::iterator miniiter;
    std::list<int>::iterator stditer;

    printTestedApi("push_back");
    for (int i = 1; i <= 20; i++)
    {
        minilist.push_back(i);
        stdlist.push_back(i);
    }
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(),minilist.end());

    printTestedApi("front");
    std::cout << stdlist.front() << ' ' << minilist.front() << ' ' << std::endl;
    stdlist.front() = 21, minilist.front() = 21;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("back");
    std::cout << stdlist.back() << ' ' << minilist.back() << ' ' << std::endl;
    stdlist.back() = 22, minilist.back() = 22;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("size");
    std::cout << stdlist.size() << ' ' << minilist.size() << std::endl;

    printTestedApi("insert");
    miniiter = minilist.insert(minilist.begin(), 23);
    stditer = stdlist.insert(stdlist.begin(), 23);
    std::cout << *stditer << ' ' << *miniiter << std::endl;
    miniiter = minilist.insert(minilist.end(), 24);
    stditer = stdlist.insert(stdlist.end(), 24);
    std::cout << *stditer << ' ' << *miniiter << std::endl;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("size");
    std::cout << stdlist.size() << ' ' << minilist.size() << std::endl;

    printTestedApi("push_front");
    for (int i = 90; i <= 100; i++)
    {
        minilist.push_front(i);
        stdlist.push_front(i);
    }
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("size");
    std::cout << stdlist.size() << ' ' << minilist.size() << std::endl;

    printTestedApi("erase");
    stditer = stdlist.erase(++stdlist.begin());
    miniiter = minilist.erase(++minilist.begin());
    std::cout << *stditer << ' ' << *miniiter << std::endl;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("pop_front");
    stdlist.pop_front();
    minilist.pop_front();
    std::cout << "front " << stdlist.front() << ' ' << minilist.front() << std::endl;
    std::cout << "back " << stdlist.back() << ' ' << minilist.back() << std::endl;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("pop_back");
    stdlist.pop_back();
    minilist.pop_back();
    std::cout << "front " << stdlist.front() << ' ' << minilist.front() << std::endl;
    std::cout << "back " << stdlist.back() << ' ' << minilist.back() << std::endl;
    show<std::list<int>::iterator, ministl::_list<int>::iterator>(stdlist.begin(), stdlist.end(), minilist.begin(), minilist.end());

    printTestedApi("swap");
    ministl::_list<int> tmplist;
    for (int i = 45; i <= 60; i++)
        tmplist.push_back(i); 
    std::cout << "before swap op:" << std::endl;
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(tmplist.begin(), tmplist.end(), minilist.begin(), minilist.end());
    minilist.swap(tmplist);
    std::cout << "after swap op:" << std::endl;
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(tmplist.begin(), tmplist.end(), minilist.begin(), minilist.end());
    minilist.swap(tmplist);

    
    printTestedApi("sort");
    minilist.sort();
    tmplist.sort();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());
    printTestedApi("size");
    std::cout << minilist.size() << ' ' << tmplist.size() << std::endl;

    printTestedApi("merge");
    std::cout << "before merge op:" << std::endl;
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());
    minilist.merge(tmplist);
    std::cout << "after merge op:" << std::endl;
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());
    printTestedApi("size");
    std::cout << minilist.size() << ' ' << tmplist.size() << std::endl;

    printTestedApi("reverse and unique");
    minilist.reverse();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);
    minilist.push_back(2);
    minilist.push_front(98);
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);
    minilist.unique();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);

    printTestedApi("splice");
    miniiter = minilist.begin();
    for (int i = 0; i < 3; i++)
        miniiter++, tmplist.push_back(i + 90);
    minilist.splice(minilist.begin(), tmplist, ++tmplist.begin());
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());
    minilist.splice(minilist.begin(), tmplist, tmplist.begin(),++tmplist.begin());
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());
    minilist.splice(miniiter, tmplist);
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), tmplist.begin(), tmplist.end());

    printTestedApi("remove");
    minilist.sort();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);
    minilist.remove(90);
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);
    minilist.unique();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);

    printTestedApi("clear");
    minilist.clear();
    show<ministl::_list<int>::iterator, ministl::_list<int>::iterator>(minilist.begin(), minilist.end(), nullptr, nullptr);
    printTestedApi("size");
    std::cout << minilist.size() << std::endl;
    
}
int main()
{
    std::cout << "................Testing vector................" << std::endl;
    testForVector();
    std::cout << std::endl;
    std::cout << "................Testing list................" << std::endl;
    testForList();

    return 0;
}