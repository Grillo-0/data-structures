#pragma once

template <typename T>
class Queue {
    virtual void enqueue(T value) = 0;
    virtual T dequeue() = 0;
    virtual bool empty() = 0;
};
