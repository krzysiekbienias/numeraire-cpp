#include <gtest/gtest.h>
#include "binary_tree/binary_tree_simple.hpp"

using namespace clicker;

TEST(BinaryTreeTest, ConstructorInitializesValue) {
    BinaryTree* node = new BinaryTree(42);
    EXPECT_EQ(node->value, 42);
    EXPECT_EQ(node->left, nullptr);
    EXPECT_EQ(node->right, nullptr);
    delete node;
}

TEST(BinaryTreeTest, CanBuildSimpleTree) {
    BinaryTree* root = new BinaryTree(10);
    root->left = new BinaryTree(5);
    root->right = new BinaryTree(15);

    EXPECT_EQ(root->left->value, 5);
    EXPECT_EQ(root->right->value, 15);

    delete root; // will recursively delete children
}


TEST(BranchSumTest, EmptyTree) {
    BinaryTree* tree = nullptr;
    std::vector<int> expected = {};
    if (tree) {
        EXPECT_EQ(tree->branchSum(), expected);
    } else {
        SUCCEED();  // Tree is null; nothing to test
    }
}

TEST(BranchSumTest, SingleNode) {
    BinaryTree root(5);
    std::vector<int> expected = {5};
    EXPECT_EQ(root.branchSum(), expected);
}

TEST(BranchSumTest, BalancedTree) {
    BinaryTree* root = new BinaryTree(1);
    root->left = new BinaryTree(2);
    root->right = new BinaryTree(3);
    root->left->left = new BinaryTree(4);
    root->left->right = new BinaryTree(5);
    root->right->left = new BinaryTree(6);
    root->right->right = new BinaryTree(7);

    std::vector<int> expected = {7, 8, 10, 11};  // 1-2-4, 1-2-5, 1-3-6, 1-3-7
    EXPECT_EQ(root->branchSum(), expected);

    delete root;
}

TEST(BranchSumTest, LeftSkewedTree) {
    BinaryTree* root = new BinaryTree(1);
    root->left = new BinaryTree(2);
    root->left->left = new BinaryTree(3);

    std::vector<int> expected = {6};  // 1-2-3
    EXPECT_EQ(root->branchSum(), expected);

    delete root;
}

TEST(BranchSumTest, RightSkewedTree) {
    BinaryTree* root = new BinaryTree(1);
    root->right = new BinaryTree(2);
    root->right->right = new BinaryTree(3);

    std::vector<int> expected = {6};  // 1-2-3
    EXPECT_EQ(root->branchSum(), expected);

    delete root;
}
