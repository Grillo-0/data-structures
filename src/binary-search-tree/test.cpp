#include "binary-search-tree.hpp"
#include <gtest/gtest.h>

class BinarySearchTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree.insert(7);
    }

    BinarySTree<int> tree;
};

TEST_F(BinarySearchTreeTest, Contains)
{
    ASSERT_TRUE(tree.contains(7));
}

TEST_F(BinarySearchTreeTest, Remove)
{
    tree.remove(7);
    ASSERT_FALSE(tree.contains(7));
}

TEST_F(BinarySearchTreeTest, IsEmpty)
{
    ASSERT_FALSE(tree.isEmpty());
    tree.remove(7);
    ASSERT_TRUE(tree.isEmpty());
}

TEST_F(BinarySearchTreeTest, MakeEmpty)
{
    tree.makeEmpty();
    EXPECT_TRUE(tree.isEmpty());
}

TEST_F(BinarySearchTreeTest, FindMax)
{
    tree.insert(8);
    tree.insert(9);
    tree.insert(4);
    EXPECT_EQ(tree.findMax(), 9);
}

TEST_F(BinarySearchTreeTest, FindMin)
{
    tree.insert(8);
    tree.insert(4);
    tree.insert(9);
    EXPECT_EQ(tree.findMin(), 4);
}

TEST_F(BinarySearchTreeTest, InOrdIterator)
{
    const int ptr[] = { 4, 7, 8, 9 };

    tree.insert(8);
    tree.insert(4);
    tree.insert(9);
    int i = 0;
    for (auto it : tree) {
        EXPECT_EQ(it, ptr[i++]);
    }
}
