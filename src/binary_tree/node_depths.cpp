#include "binary_tree/node_depths.hpp"
#include "binary_tree/binary_tree_node.hpp"

void nodeDepthHelper(BinaryTreeNode* root,int currentDepth,int& sum) {
    if (root==nullptr) return;
    sum+=currentDepth;
    nodeDepthHelper(root->left,currentDepth+1,sum);
    nodeDepthHelper(root->right,currentDepth+1,sum);

}

int nodeDepths(BinaryTreeNode* root) {
    int sum=0;
    nodeDepthHelper(root,0,sum);
    return sum;
}
