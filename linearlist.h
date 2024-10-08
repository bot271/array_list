//
//  linearlist.h
//  linear list
//
//  Created by Mingze Lee on 2024/10/7.
//
#ifndef LINEARLIST_H_
#define LINEARLIST_H_
#include <iostream>
#include <algorithm>
#include "illegal_parameter_value.h"

template <typename T>
class linearList
{
public:
    virtual ~linearList() { };
    virtual bool empty() const = 0;
    virtual int size() const = 0;
    virtual T & get(int) const = 0;
    virtual int indexOf(const T &) const = 0;
    virtual void erase(int) = 0;
    virtual void insert(int, const T &) = 0;
    virtual void output(std::ostream &) const = 0;
};

template <typename T>
class arrayList : public linearList<T>
{
protected:
    void checkIndex(int) const;
    T * element;
    int arrayLength;
    int listSize;

public:
    arrayList(int initialCapacity = 10);
    arrayList(const arrayList<T> &);
    ~arrayList() { delete element; };
    bool empty() const { return listSize == 0; };
    int size() const { return listSize; };
    T & get(int) const;
    int indexOf(const T &) const;
    void erase(int);
    void insert(int, const T &);
    void output(std::ostream &) const;
    friend std::ostream & operator<<(std::ostream &, const arrayList<T> &);
    int capacity() const { return arrayLength; };
    
    class iterator
    {
    protected:
        T * position;
    public:
        iterator(T * pos = nullptr) : position(pos) { };
        T& operator*() const { return *position; };
        T* operator-> () const { return &* position; };
        iterator& operator++() { ++position; return * this; };
        iterator operator++(int) { iterator old = *this; ++position; return old; };
        //operator++() is the operation ++x, while oeprator(int) is the operation x++
        iterator& operator--() { --position; return * this; };
        iterator operator--(int) { iterator old = *this; --position; return old; };
        bool operator==(const iterator & right) { return position == right.position; };
        bool operator!=(const iterator & right) { return position != right.position; };
    };
    iterator begin() { return iterator(element); };
    iterator end() { return iterator(element + listSize); };
};

template <typename T>
void changeLength(T *& a, int oldLength, int newLength)
{
    if (newLength < 0)
        throw illegalParameterValue("Length must be nonnegative");
    T * temp = new T [newLength];
    int minL = newLength < oldLength? newLength : oldLength;
    std::copy(a, a + minL, temp);
    delete [] a;
    a = temp;
}

template <typename T>
arrayList<T>::arrayList(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        std::cout << "Initial capacity = " << initialCapacity << " must be positive.";
        throw illegalParameterValue();
    }
    arrayLength = initialCapacity;
    element = new T [initialCapacity];
    listSize = 0;
}

template <typename T>
arrayList<T>::arrayList(const arrayList & theList)
{
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T [theList.arrayLength];
    copy(theList.element, theList.element + listSize, element);
}

template <typename T>
void arrayList<T>::checkIndex(int index) const
{
    if (index < 0 || index >= listSize)
    {
        std::cout << "Index = " << index << ", and list size = " << listSize << "!\n";
        throw illegalParameterValue();
    }
}

template <typename T>
T & arrayList<T>::get(int index) const
{
    checkIndex(index);
    return *(element + index);
}

template <typename T>
int arrayList<T>::indexOf(const T & target) const
{
    int index = (int) (std::find(element, element + arrayLength, target) - element);
    if (index == arrayLength)
        return -1;
    return index;
}

template <typename T>
void arrayList<T>::erase(int index)
{
    checkIndex(index);
    std::copy(element + index + 1, element + listSize, element + index);
    element[--listSize].~T();
    //The upper statement is for when class T involves dynamic memory allocation,
    //when T is datatypes like int or char, the compiler would ignore ~T().
    //Or we can use the following:
    //if consexpr (std::is_trivially_destructible<T>::value)
    //{element.[listSize - 1].~T();
}

template <typename T>
void arrayList<T>::insert(int index, const T & target)
{
    if (index < 0 || index > listSize)
    {
        std::cout << "index = " << index << ", size = " << listSize;
        throw illegalParameterValue();
    }
    if (listSize == arrayLength)
    {
        changeLength(element, arrayLength, 2 * arrayLength);
        std::copy_backward(element + index, element + listSize, element + listSize + 1);
        element[index] = target;
        ++listSize;
    }
}

template <typename T>
void arrayList<T>::output(std::ostream & os) const
{
    copy(element, element + listSize, std::ostream_iterator<T>(os, " "));
}

template <typename T>
std::ostream & operator<<(std::ostream & os, arrayList<T> & arL)
{
    arL.output(os);
    return os;
}

#endif
