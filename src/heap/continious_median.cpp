#include <queue>
#include <functional>
#include <vector>
#include "heap/continious_median.hpp"

ContinousMedianHandler::ContinousMedianHandler():median(0.0){};

void ContinousMedianHandler::insert(int number){
    // if left half, represented by maxHeap is empty or new element to be inserted is less than element on the top teh insert this element into left half
    if (m_maxHeap.empty() || number<m_maxHeap.top()){
        m_maxHeap.push(number);
        //check for potential rebalancing.
        if (m_maxHeap.size()>m_minHeap.size()+1){
            int elementToTransfer =m_maxHeap.top();
            m_maxHeap.pop();
            m_minHeap.push(elementToTransfer);
        }
    }
        else{
            m_minHeap.push(number);
            //check for rebalancing
            if (m_maxHeap.size()<m_minHeap.size()) {
                int elementToTransfer =m_minHeap.top();
                m_minHeap.pop();
                m_maxHeap.push(elementToTransfer );
                
            }
        }
}

int ContinousMedianHandler::getMaxHeapTop(){
    if (m_maxHeap.empty()) {
        throw std::runtime_error("The Heap is empty.");
    }
    else{
        return m_maxHeap.top();
    }
}

int ContinousMedianHandler::getMinHeapTop(){
    if (m_minHeap.empty()) {
        throw std::runtime_error("The Heap is empty.");
    }
    else{
        return m_minHeap.top();
    }
}

double ContinousMedianHandler::getMedian(){
    int streamLenght =m_maxHeap.size()+m_minHeap.size();
    if (streamLenght%2==0){
        return (m_maxHeap.top()+m_minHeap.top())/2.0;
    }
    else return m_maxHeap.top();
}
