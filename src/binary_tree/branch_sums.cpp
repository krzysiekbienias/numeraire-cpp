#include<vector>
#include "binary_tree/binary_tree_node.hpp"
#include "binary_tree/branch_sums.hpp"


void branchSumHelper(BinaryTreeNode * root, int runningSum, std::vector<int> &results) {
    if (root == nullptr) {
        return;
    }
    const int newSum=runningSum+root->m_value;
    if (root->left==nullptr && root->right==nullptr) {
        results.push_back(newSum);
    }
    branchSumHelper(root->left,newSum,results);
    branchSumHelper(root->right,newSum,results);
}

std::vector<int> branchSums(BinaryTreeNode* root) {
    std::vector<int> results;
    branchSumHelper(root,0,results);
    return results;

}
