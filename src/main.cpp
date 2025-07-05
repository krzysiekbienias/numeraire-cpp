#include <iostream>
#include "heap/kth_largest_element.hpp"
#include "utils/print_utils.hpp"
#include "graphs/graph.hpp"
#include "graphs/count_islands.hpp"
#include "graphs/island_perimeter.hpp"
#include "graphs/river_sizes.hpp"
#include "dynamic_programming/paths_in_matrix.hpp"
#include "dynamic_programming/minimum_path_cost.hpp"
#include "dynamic_programming/gold_mine.hpp"
#include "dynamic_programming/lis_len.hpp"
#include "arrays/min_reward.hpp"


#include "matrix/row_by_row.hpp"

int main() {
    std::cout << "Welcome to Clicker++! 🧟‍♂️\n";
    std::cout<<"WARNING: Please remember not commit main and CMakeLists.txt file becaus it is set up on older architecture. \n";
    std::vector<int> scores = {8, 4, 2, 1, 3, 6, 7, 9, 5};
    std::cout<<minReward(scores)<<'\n';
    
    
    return 0;
}

