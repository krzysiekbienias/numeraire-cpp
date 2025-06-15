#pragma once
#include "graph_node.hpp"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
// Declare your graph interface here.

class Graph{
private:
    std::unordered_map<int, GraphNode*> m_nodes;
    void dfsHelper(GraphNode* node, std::unordered_set<int>& visited) const;
    
public:
    ~Graph();
    GraphNode* addNode(int);
    void addEdge(int fromId,int toId,bool directed=false);
    void display(bool directed = false) const;
    void displayAdjacencyList(bool directed = false) const;
    void dfs(int startId) const;
    
};
