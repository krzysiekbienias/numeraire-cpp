#include "arrays/sorted_squared.hpp"
#include "binary_tree/binary_tree_simple.hpp"
#include "stack/daily_temperatures.hpp"
#include "stack/evaluate_expression.hpp"
#include <fstream>
#include <iostream>

#include "arrays/longest_increasing_matrix_path.hpp"
#include "greedy/jump_to_the_end.hpp"
#include "linked_list/single_linked_list.hpp"
#include "math_and_geometry/line_through_points.hpp"
#include "priority_queue/stones_war.hpp"
#include "queue/students_lunch.hpp"
#include "queue/time_to_buy_tickets.hpp"
#include "recursion/nth_fibbonaci.hpp"
#include "sliding_window/longest_substring_without_repeating_characters.hpp"
#include "utils/print_utils.hpp"
#include "sorting/kth_largest_element.hpp"
#include "heap/kth_largest_element.hpp"
#include "backtracking/permutations.hpp"
#include "matrix/row_by_row.hpp"
#include "matrix/column_by_column.hpp"
#include"graphs/graph.hpp"
#include"graphs/graph_node.hpp"
#include "dynamic_programming/paths_in_matrix.hpp"

int main() {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 1, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0}
    };
    pathsInMatrix(matrix);
    
    
  return 0;
}
