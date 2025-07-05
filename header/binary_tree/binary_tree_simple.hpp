#pragma once
#include <vector>

// Declare your binary_tree_simple interface here.
namespace clicker {
            
    class BinaryTree {
    public:
        int value;
        BinaryTree* left;
        BinaryTree* right;

        BinaryTree(int val);
        ~BinaryTree();
        
        std::vector<int>branchSum() const; //AlgoExpert
    private:
        void branchSumHelper(const BinaryTree* node,int runningSum,std::vector<int>& sums) const;
        
        
    };
}

