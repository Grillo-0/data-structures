#include "avl-tree.hpp"
#include <gtest/gtest.h>

class AVLTreeTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        tree.insert(7);
    }

    AVLTree<int> tree;
};

TEST_F(AVLTreeTest, Contains)
{
    ASSERT_TRUE(tree.contains(7));
}

TEST_F(AVLTreeTest, Remove)
{
    tree.remove(7);
    ASSERT_FALSE(tree.contains(7));
}

TEST_F(AVLTreeTest, IsEmpty)
{
    ASSERT_FALSE(tree.isEmpty());
    tree.remove(7);
    ASSERT_TRUE(tree.isEmpty());
}

TEST_F(AVLTreeTest, MakeEmpty)
{
    tree.makeEmpty();
    EXPECT_TRUE(tree.isEmpty());
}

TEST_F(AVLTreeTest, FindMax)
{
    tree.insert(8);
    tree.insert(9);
    tree.insert(4);
    EXPECT_EQ(tree.findMax(), 9);
}

TEST_F(AVLTreeTest, FindMin)
{
    tree.insert(8);
    tree.insert(4);
    tree.insert(9);
    EXPECT_EQ(tree.findMin(), 4);
}

TEST_F(AVLTreeTest, InOrdIterator)
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

TEST_F(AVLTreeTest, PrintTree)
{

    std::stringstream check;
    check << "digraph {" << std::endl;
    check << 8 << "->" << 7 << std::endl;
    check << 8 << "->" << 9 << std::endl;
    check << 9 << "->" << 10 << std::endl;
    check << "}" << std::endl;
    for (int i = 8; i < 11; ++i) {
        tree.insert(i);
    }

    std::stringstream result;
    tree.print(result);
    EXPECT_EQ(check.str(), result.str());
}
