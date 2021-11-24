#include "linked_list.hpp"
#include <gtest/gtest.h>

TEST(LinkedList, InsertOrd)
{
    LinkedList<int> list;

    auto isBigger = [](int left, int right) {
        return left > right;
    };

    list.insert_ord(9, isBigger);
    list.insert_ord(8, isBigger);
    list.insert_ord(7, isBigger);

    EXPECT_EQ(list[0], 7);
    EXPECT_EQ(list[1], 8);
    EXPECT_EQ(list[2], 9);
}
