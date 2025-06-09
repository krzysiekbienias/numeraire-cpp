
#include <vector>
#include "graphs/graph_node.hpp"
// Implement your graph_node logic here.


GraphNode::GraphNode(int id):m_id(id){}

int GraphNode::getId() const{
    return m_id;
}


const std::vector<GraphNode*>& GraphNode::getNeighbors()const{
    return m_neighbors;
}

void GraphNode::addNeighbor(GraphNode *neighbor){
    m_neighbors.push_back(neighbor);
}
