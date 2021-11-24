#pragma once

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>

template <typename T>
class ArrayIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

private:
    pointer _ptr;

public:
    ArrayIterator(pointer ptr)
        : _ptr(ptr) {};
    reference operator*() const { return *_ptr; }
    pointer operator->() { return _ptr; }
    ArrayIterator& operator++()
    {
        _ptr++;
        return *this;
    }
    ArrayIterator operator++(int)
    {
        ArrayIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const ArrayIterator& a, const ArrayIterator& b) { return a._ptr == b._ptr; }
    friend bool operator!=(const ArrayIterator& a, const ArrayIterator& b) { return a._ptr != b._ptr; }
};

template <typename T>
class MyArray {
private:
    T* _arr;
    int _size;
    int _capacity;

public:
    MyArray()
        : _size(0)
        , _capacity(1)
        , _arr(new T[_capacity])
    {
    }

    ~MyArray()
    {
        delete _arr;
    }

    int size()
    {
        return _size;
    }

    int capacity()
    {
        return _capacity;
    }

    bool is_empty()
    {
        return _size == 0;
    }

    T at(const int index)
    {
        if (index > _size)
            throw std::invalid_argument("index out of bounds");
        return _arr[index];
    }

    T operator[](const int index)
    {
        return this->at(index);
    }

    ArrayIterator<T> begin() { return ArrayIterator<T>(&_arr[0]); }

    ArrayIterator<T> end() { return ArrayIterator<T>(&_arr[_size]); }

    void push(const T item)
    {
        _arr[_size] = item;
        _size++;
        if (_size == _capacity)
            this->resize(_capacity * 2);
    }

    void insert(const int index, const T item)
    {
        if (index > _size)
            throw std::invalid_argument("index out of size");
        for (int i = _size; i >= index; i--) {
            _arr[i + 1] = _arr[i];
        }
        _arr[index] = item;
        _size++;
        if (_size == _capacity)
            this->resize(_capacity * 2);
    }

    void prepend(const T item)
    {
        this->insert(0, item);
    }

    void pop()
    {
        _size--;
        if (_size < _capacity / 4)
            this->resize(_capacity / 2);
    }

    void del(const int index)
    {
        for (int i = index; i < _size; i++) {
            T tmp = _arr[i];
            _arr[i] = _arr[i + 1];
            _arr[i + 1] = tmp;
        }
        this->pop();
    }

    void remove(const T item)
    {
        for (int i = 0; i < _size; i++) {
            if (_arr[i] == item) {
                this->del(i);
                i--;
            }
        }
    }

    int find(const T item)
    {
        for (int i = 0; i < _size; i++) {
            if (_arr[i] == item) {
                return i;
            }
        }
        return -1;
    }

private:
    void resize(const int new_capacity)
    {
        _arr = (T*)std::realloc(_arr, sizeof(T) * new_capacity);
        _capacity = new_capacity;
    }
};
