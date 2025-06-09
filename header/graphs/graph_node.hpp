#pragma once

// Declare your graph_node interface here.
#include <vector>

class GraphNode
{
private:
    int m_id;
    std::vector<GraphNode*> m_neighbors;
    
public:
    explicit GraphNode(int id);
    int getId() const;
    const std::vector<GraphNode*>& getNeighbors() const;
    
    void addNeighbor(GraphNode * neighbor);
};
