#pragma once

#include <functional>
#include <stdexcept>

template <typename T>
class LinkedList {
protected:
    struct Node {
        T m_value;
        struct Node* m_next;
        struct Node* m_prev;

        Node(T value)
            : m_value(value)
            , m_next(nullptr)
            , m_prev(nullptr) {};

        Node(T value, struct Node* prev, struct Node* next)
            : m_value(value)
            , m_next(next)
            , m_prev(prev)
        {
            if (m_next != nullptr) {
                m_next->m_prev = this;
            }
            if (m_prev != nullptr) {
                m_prev->m_next = this;
            }
        }
    };

    struct Node* m_head;
    struct Node* m_tail;
    int m_size;

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = struct Node;
        using pointer = struct Node*;
        using reference = struct Node&;

        Iterator(pointer ptr)
            : m_ptr(ptr) {};

        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        Iterator& operator++()
        {
            m_ptr = m_ptr->m_next;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b)
        {
            return a.m_ptr == b.m_ptr;
        }
        friend bool operator!=(const Iterator& a, const Iterator& b)
        {
            return a.m_ptr != b.m_ptr;
        }

    private:
        pointer m_ptr;
    };

public:
    Iterator begin() { return Iterator(m_head); }

    Iterator end()
    {
        if (m_size == 0) {
            return Iterator(m_tail);
        }
        return Iterator(m_tail->m_next);
    }

    LinkedList();

    ~LinkedList();

    int size();

    bool empty();

    T value_at(int index);

    T operator[](int index);

    void push_front(T value);

    void pop_front();

    void push_back(T value);

    void pop_back();

    T front();

    T back();

    void insert(int index, T value);
    void insert_ord(T value, std::function<bool(T left, T right)>);

    void erase(int index);

    void erase(Node it);

    T value_n_from_end(int n);

    void reverse();

    int remove_value(int value);
};

template <typename T>
LinkedList<T>::LinkedList()
    : m_head(nullptr)
    , m_tail(nullptr)
    , m_size(0) {};

template <typename T>
LinkedList<T>::~LinkedList()
{
    if (m_size == 0)
        return;
    struct Node* curr = m_head;
    while (curr != m_tail) {
        curr = curr->m_next;
        delete curr->m_prev;
    }
    delete curr;
};

template <typename T>
int LinkedList<T>::size()
{
    return m_size;
}

template <typename T>
bool LinkedList<T>::empty()
{
    return m_size == 0;
}

template <typename T>
T LinkedList<T>::value_at(int index)
{
    if (index < 0)
        throw std::invalid_argument("index cannot be negative");
    if (index == m_size - 1)
        return m_tail->m_value;
    struct Node* tmp = m_head;

    for (int i = 0; i < index; i++) {
        tmp = tmp->m_next;
        if (tmp == nullptr)
            throw std::runtime_error("index out of range");
    }
    return tmp->m_value;
}

template <typename T>
T LinkedList<T>::operator[](int index)
{
    return value_at(index);
}

template <typename T>
void LinkedList<T>::push_front(T value)
{
    m_head = new Node(value, nullptr, m_head);
    m_size++;
    if (m_size == 1) {
        m_tail = m_head;
    }
}

template <typename T>
void LinkedList<T>::pop_front()
{
    if (m_size == 1) {
        delete m_head;
    } else {
        m_head = m_head->m_next;
        delete m_head->m_prev;
    }
    m_size--;
}

template <typename T>
void LinkedList<T>::push_back(T value)
{
    if (m_size < 1) {
        push_front(value);
        return;
    }
    m_tail = new Node(value, m_tail, nullptr);
    m_size++;
}

template <typename T>
void LinkedList<T>::pop_back()
{
    if (m_size == 1) {
        pop_front();
        return;
    }

    m_tail = m_tail->m_prev;
    delete m_tail->m_next;
    m_tail->m_next = nullptr;
    m_size--;
}

template <typename T>
T LinkedList<T>::front()
{
    return m_head->m_value;
}

template <typename T>
T LinkedList<T>::back()
{
    return m_tail->m_value;
}

template <typename T>
void LinkedList<T>::insert(int index, T value)
{
    if (index >= m_size || index < 0)
        throw std::invalid_argument("index out of range");
    if (index == 0) {
        push_front(value);
        return;
    }

    struct Node* tmp = m_head;
    for (int i = 0; i < index; i++) {
        tmp = tmp->m_next;
    }
    tmp = new Node(value, tmp->m_prev, tmp);
    m_size++;
}

template <typename T>
void LinkedList<T>::insert_ord(T value, std::function<bool(T left, T right)> comparator)
{
    if (m_size == 0 || !comparator(value, m_head->m_value)) {
        push_front(value);
        return;
    }

    struct Node* tmp = m_head;
    while (comparator(value, tmp->m_value)) {
        tmp = tmp->m_next;
    }
    tmp = new Node(value, tmp->m_prev, tmp);
    m_size++;
}

template <typename T>
void LinkedList<T>::erase(int index)
{
    if (index >= m_size || index < 0)
        throw std::invalid_argument("index out of range");
    if (index == 0) {
        pop_front();
        return;
    }

    struct Node* tmp = m_head;
    for (int i = 0; i < index; i++) {
        tmp = tmp->m_next;
    }
    tmp->prev->m_next = tmp->m_next;
    tmp->next->m_prev = tmp->m_prev;
    delete tmp;
    m_size--;
}

template <typename T>
void LinkedList<T>::erase(LinkedList<T>::Node it)
{
    if (it.m_next != nullptr) {
        it.m_next->m_prev = it.m_prev;
    }
    if (it.m_prev != nullptr) {
        it.m_prev->m_next = it.m_next;
    }
    m_size--;
}

template <typename T>
T LinkedList<T>::value_n_from_end(int n)
{
    return value_at(m_size - n - 1);
}

template <typename T>
void LinkedList<T>::reverse()
{
    struct Node* curr = m_head;
    struct Node* next = nullptr;
    while (curr != nullptr) {
        next = curr->m_next;
        curr->m_next = curr->m_prev;
        curr->m_prev = next;
        curr = next;
    }
    std::swap(m_tail, m_head);
}

template <typename T>
int LinkedList<T>::remove_value(int value)
{
    if (m_head->m_value == value) {
        pop_front();
        return 0;
    }

    struct Node* tmp = m_head;
    while (tmp != nullptr && tmp->m_value != value) {
        tmp = tmp->m_next;
    }

    if (tmp == nullptr)
        return -1;

    if (tmp == m_tail)
        m_tail = tmp->m_prev;

    if (tmp->m_prev != nullptr)
        tmp->m_prev->m_next = tmp->m_next;

    if (tmp->m_next != nullptr)
        tmp->m_next->m_prev = tmp->m_prev;
    delete tmp;

    m_size--;
    return 0;
}
