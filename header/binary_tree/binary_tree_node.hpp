#pragma once

class BinaryTreeNode {
    public:
        int m_value;
        BinaryTreeNode* left;
        BinaryTreeNode* right;
        //constructor

        explicit BinaryTreeNode(int value) {
            this->m_value = value;
            left = nullptr;
            right = nullptr;
        }

        //destructor
        ~BinaryTreeNode() {
            delete left;
            delete right;
        }
};