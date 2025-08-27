#include "arrays/sorted_squared.hpp"
#include "binary_tree/binary_tree_simple.hpp"
#include "stack/daily_temperatures.hpp"
#include "stack/evaluate_expression.hpp"
#include <fstream>
#include <iostream>
#include "heap/kth_largest_element.hpp"
#include "utils/print_utils.hpp"
#include "utils/print_utils_templates.hpp"
#include "graphs/graph.hpp"
#include "graphs/count_islands.hpp"
#include "graphs/island_perimeter.hpp"
#include "graphs/river_sizes.hpp"
#include "dynamic_programming/paths_in_matrix.hpp"
#include "dynamic_programming/minimum_path_cost.hpp"
#include "dynamic_programming/gold_mine.hpp"
#include "dynamic_programming/lis_len.hpp"
#include "arrays/min_reward.hpp"
#include "greedy/jump_to_the_end.hpp"
//#include "linked_list/single_linked_list.hpp"
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
#include "binary_tree/binary_tree_node.hpp"
#include "binary_tree/branch_sums.hpp"
#include "matrix/row_by_row.hpp"
#include "matrix/column_by_column.hpp"
#include "graphs/graph.hpp"
#include "graphs/graph_node.hpp"
#include "dynamic_programming/paths_in_matrix.hpp"
#include "dynamic_programming/knapsack_problem01.hpp"
#include "dynamic_programming/min_cost_for_tickets.hpp"
#include "math_and_geometry/largest_triangle_area.hpp"
#include "strings/roman_to_intiger.hpp"
#include "matrix/row_by_row.hpp"
#include "competitive_programming/binary_exponential.hpp"
#include "strings/ceasar_cipher_encryptor.hpp"
#include "sorting/bubble_sort.hpp"
#include "sorting/count_inversions.hpp"
#include "sorting/insertion_sort.hpp"
#include "sorting/dutch_national_flag.hpp"
#include "searching/local_maxima_array.hpp"
#include "strings/first_non_repeating_character.hpp"
#include "linked_list/find_loop.hpp"

int main() {

    std::cout << "Welcome to Clicker++! 🧟‍♂️\n";
    BinaryTreeNode* root = new BinaryTreeNode(1);
    root->left  = new BinaryTreeNode(2);
    root->right = new BinaryTreeNode(3);
    root->left->left   = new BinaryTreeNode(4);
    root->left->right  = new BinaryTreeNode(5);
    root->right->left  = new BinaryTreeNode(6);
    root->right->right = new BinaryTreeNode(7);
    root->left->left->left   = new BinaryTreeNode(8);
    root->left->left->right  = new BinaryTreeNode(9);
    root->left->right->left = new BinaryTreeNode(10);

    std::cout << root->left->left->right->m_value;

    branchSums(root);

   delete root;

    return 0;
}

