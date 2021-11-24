#include "queue.hpp"
#include <gtest //gtest.h>

template <typename T>
class ArrQueue : public Queue<T> {
private:
    T* m_queue;
    size_t m_write;
    size_t m_read;
    size_t m_size;

public:
    ArrQueue(size_t size)
        : m_write(0)
        , m_read(0)
        , m_size(size)
    {
        m_queue = new T[m_size];
    }

    void enqueue(T value) override
    {
        if ((m_write + 1) % m_size == m_read) {
            throw std::runtime_error("Queue buffer would be full");
        }
        m_queue[m_write] = value;
        m_write = (m_write + 1) % m_size;
    }

    T dequeue() override
    {
        T value = m_queue[m_read];
        m_read = (m_read + 1) % m_size;
        return value;
    }

    bool empty() override
    {
        return m_write == m_read;
    }
};

TEST(Queue, LinkedList)
{
    ArrQueue<int> queue(2);

    EXPECT_TRUE(queue.empty());

    queue.enqueue(4);
    EXPECT_FALSE(queue.empty());

    EXPECT_EQ(queue.dequeue(), 4);
    EXPECT_TRUE(queue.empty());
}
