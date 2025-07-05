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

template <typename T>
void printVector(const std::vector<T>& vec, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& item : vec) std::cout << item << " ";
    std::cout << "\n";
}

template <typename T>
void printList(const std::list<T>& lst, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& item : lst) std::cout << item << " ";
    std::cout << "\n";
}

template <typename T>
void printDeque(const std::deque<T>& dq, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& item : dq) std::cout << item << " ";
    std::cout << "\n";
}

template <typename T>
void printSet(const std::set<T>& s, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& item : s) std::cout << item << " ";
    std::cout << "\n";
}

template <typename T>
void printUnorderedSet(const std::unordered_set<T>& us, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& item : us) std::cout << item << " ";
    std::cout << "\n";
}

template <typename K, typename V>
void printMap(const std::map<K, V>& m, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [key, value] : m) {
        std::cout << "  " << key << " -> " << value << "\n";
    }
}

template <typename K, typename V>
void printUnorderedMap(const std::unordered_map<K, V>& m, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [key, value] : m) {
        std::cout << "  " << key << " -> " << value << "\n";
    }
}



template <typename K, typename V>
void printMapOfDeque(const std::map<K, std::deque<V>>& m, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [key, dq] : m) {
        std::cout << "  " << key << " -> ";
        for (const auto& item : dq) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
}

template <typename K, typename V>
void printMapOfUnorderedSets(const std::map<K, std::unordered_set<V>>& m, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [key, uset] : m) {
        std::cout << "  " << key << " -> { ";
        for (const auto& item : uset) {
            std::cout << item << " ";
        }
        std::cout << "}\n";
    }
}


template <typename K, typename V>
void printUnorderedMapOfUnorderedSets(const std::unordered_map<K, std::unordered_set<V>>& m, const std::string& label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [key, uset] : m) {
        std::cout << "  " << key << " -> { ";
        for (const auto& item : uset) {
            std::cout << item << " ";
        }
        std::cout << "}\n";
    }
}
}
