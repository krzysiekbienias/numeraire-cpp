

// Implement your binary_tree_simple logic here.
#include "binary_tree/binary_tree_simple.hpp"


BinaryTree::BinaryTree(int val) : value(val), left(nullptr), right(nullptr) {}

BinaryTree::~BinaryTree() {
    delete left;
    delete right;
}
std::vector<int> BinaryTree::branchSum() const{
    
    std::vector<int> sums;
    branchSumHelper(this, 0, sums);
    return sums;
}


void BinaryTree::branchSumHelper(const BinaryTree* node, int runningSum, std::vector<int> &sums) const{
    if (node==nullptr) return ;
    int newSum=runningSum+node->value;
    if (node->left==nullptr && node->right==nullptr){
        sums.push_back(newSum);
    }
    
    branchSumHelper(node->left,newSum,sums);
    branchSumHelper(node->right,newSum,sums);
}

