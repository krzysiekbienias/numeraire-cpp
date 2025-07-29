#pragma once
#include <queue>
#include <functional>
#include <vector>

// Declare your continious_median interface here.
class ContinousMedianHandler{
private:
    std::priority_queue<int> m_maxHeap;
    std::priority_queue<int,std::vector<int>,std::greater<int>> m_minHeap;
    double median;

public:
    ContinousMedianHandler();
    void insert(int number);
    double getMedian();
    int getMaxHeapTop();
    int getMinHeapTop();
    
};

