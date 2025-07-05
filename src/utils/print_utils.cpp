#include <iostream>
#include "linked_list/single_linked_list.hpp"

namespace print_utils {

void printBoxedLabel(const std::string& label, int totalWidth = 40) {
    const int padding = 2; // for '| ' and ' |'
    int contentWidth = totalWidth - padding;

    std::cout << "\n+" << std::string(contentWidth, '=') << "+\n";
    std::cout << "| " << label << std::string(contentWidth - label.length(), ' ') << "|\n";
    std::cout << "+" << std::string(contentWidth, '=') << "+\n";
}


void printLinkedList(const LinkedList& list, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    Node* current = list.head;
    while (current != nullptr) {
        std::cout << "[" << current->value << "]";
        if (current->next != nullptr)
            std::cout << " -> ";
        current = current->next;
    }
    std::cout << " -> null\n";
}
}


