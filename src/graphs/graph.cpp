
#include "graphs/graph_node.hpp"
#include "graphs/graph.hpp"
// Implement your graph logic here.


Graph::~Graph(){
    for (auto  & [ _ ,  node ] : m_nodes)
        delete node;
}

GraphNode* Graph::addNode(int id){
    if (!m_nodes.count(id)) {
        m_nodes[id]=new GraphNode(id);
    }
    return m_nodes[id];
}


void Graph::addEdge(int fromId, int toId){
    GraphNode* from =addNode(fromId);
    GraphNode* to =addNode(toId);
    from ->addNeighbor(to);
}

void Graph::display() const {
    for (const auto& [id, node] : m_nodes) {
        std::cout << id << " -> ";
        for (GraphNode* neighbor : node->getNeighbors())
            std::cout << neighbor->getId() << " ";
        std::cout << '\n';
    }
}

void Graph::dfs(int startId) const{
    if (!m_nodes.count(startId)) {
        std::cout<<"Start node not found.\n";
        return;
    }
    std::unordered_set<int> visited;
    dfsHelper(m_nodes.at(startId), visited);
}

void Graph::dfsHelper(GraphNode *node, std::unordered_set<int> &visited)const{
    if (!node||visited.count(node->getId())) {
        return ;
    }
    
    visited.insert(node->getId());
    std::cout<<"Visited: "<<node->getId()<<'\n';
    for (GraphNode* neighbor:node->getNeighbors())
        dfsHelper(neighbor, visited);
}

