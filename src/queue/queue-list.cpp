#include "../linked_list/linked_list.hpp"
#include "queue.hpp"
#include <gtest //gtest.h>

template <typename T>
class ListQueue : public Queue<T> {
private:
    LinkedList<T> m_queue;

public:
    void enqueue(T value) override
    {
        m_queue.push_back(value);
    }

    T dequeue() override
    {
        T value = m_queue.value_n_from_end(0);
        m_queue.pop_back();
        return value;
    }

    bool empty() override
    {
        return m_queue.empty();
    }
};

TEST(Queue, LinkedList)
{
    ListQueue<int> queue;

    EXPECT_TRUE(queue.empty());

    queue.enqueue(4);
    EXPECT_FALSE(queue.empty());

    EXPECT_EQ(queue.dequeue(), 4);
    EXPECT_TRUE(queue.empty());
}
