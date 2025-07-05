#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "linked_list/single_linked_list.hpp"

namespace print_utils {

void printLinkedList(const LinkedList& list, const std::string& label = "");
void printBoxedLabel(const std::string& label, int totalWidth = 40);


}


