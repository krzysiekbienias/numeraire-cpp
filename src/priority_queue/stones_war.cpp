#include<vector>
#include<queue>
#include"priority_queue/stones_war.hpp"

// Implement your stones_war logic here.
int lastStoneWeight(const std::vector<int>& stones){
    
    if (stones.size()==0) return 0;
    if (stones.size()==1) return stones[0];
    if (stones.size()==2) return std::abs(stones[0]-stones[1]);
    
    std::priority_queue<int>pg;
    for (int stone:stones){
        pg.push(stone);
    }
    while(pg.size()>1){
        int top_element=pg.top();
        pg.pop();
        int second_element=pg.top();
        pg.pop();
        int diff = std::abs(top_element-second_element);
        if (diff>0) pg.push(diff);
    }
    return pg.empty() ? 0 : pg.top();
}
