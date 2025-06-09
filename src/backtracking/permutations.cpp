#include <vector>
#include <unordered_set>
#include <backtracking/permutations.hpp>

// Implement your parmutations logic here.
void backtrack(const std::vector<int>& nums,
               std::vector<int>& candidate,
               std::unordered_set<int>& used,
               std::vector<std::vector<int>>& results){
    if (nums.size()==candidate.size()){
        results.push_back(candidate);
        return;
    }
    for(int el : nums){
        if (!used.count(el)){
            candidate.push_back(el);
            used.insert(el);
            backtrack(nums,candidate,used,results);
            candidate.pop_back();
            used.erase(el);
            
        }
        
    }
}

std::vector<std::vector<int>> permutations (std::vector<int> nums){
    std::vector<std::vector<int>> results;
    std::vector<int> candidate;
    std::unordered_set<int> used;
    backtrack(nums, candidate, used, results);
    return results;
}

