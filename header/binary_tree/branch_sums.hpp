#pragma once
#include<vector>
#include "binary_tree/binary_tree_node.hpp"

std::vector<int> branchSums(BinaryTreeNode* root);

void branchSumsHelper(BinaryTreeNode* root,int runningSum,std::vector<int>&results);
