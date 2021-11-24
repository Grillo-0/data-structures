#include "../array/array.hpp"
#include "../linked_list/linked_list.hpp"
#pragma once

template <typename T>
class SparceMatrix {
private:
    T m_baseValue;
    unsigned int m_lines;

    struct m_Node {
        T m_value;
        unsigned int m_col;
    };
    LinkedList<m_Node>* m_matrix;

public:
    SparceMatrix<T>(T baseValue, const unsigned int& lines);
    ~SparceMatrix<T>();
    void insert(const unsigned int& line, const unsigned int& col, T value);
    auto get(const unsigned int& line, const unsigned int& col) -> T;
};

template <typename T>
SparceMatrix<T>::SparceMatrix(T baseValue, const unsigned int& lines)
    : m_baseValue(baseValue)
    , m_lines(lines)
{
    m_matrix = new LinkedList<m_Node>[m_lines];
}

template <typename T>
SparceMatrix<T>::~SparceMatrix()
{
    //    for (int i = 0; i < m_lines; i++)
    //        delete m_matrix[i];
    //
    delete[] m_matrix;
}

template <typename T>
void SparceMatrix<T>::insert(const unsigned int& line, const unsigned int& col, T value)
{
    for (auto it : m_matrix[line]) {
        if (it.m_value.m_col == col) {
            if (value == m_baseValue) {
                m_matrix[line].erase(it);
            } else {
                it.m_value.m_value = value;
            }
            return;
        }
    }

    if (value == m_baseValue)
        return;

    m_matrix[line].push_front({ value, col });
}

template <typename T>
auto SparceMatrix<T>::get(const unsigned int& line, const unsigned int& col) -> T
{
    for (auto it : m_matrix[line]) {
        if (it.m_value.m_col == col)
            return it.m_value.m_value;
    }
    return m_baseValue;
}
