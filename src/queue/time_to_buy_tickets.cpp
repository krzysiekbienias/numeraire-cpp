#include<vector>
#include<queue>
#include "queue/time_to_buy_tickets.hpp"
#include <iostream>

// Implement your time_to_buy_tickets logic here.
int timeRequiredToBuy(const std::vector<int>& tickets, int k){
    int timeCounter=0;
    std::queue<std::pair<int,int>> people;
    for (int i=0; i<tickets.size();++i){
        people.push({i,tickets[i]});
    }
    std::cout<<"queue is ready \n";
    while (!people.empty()){
        auto [idx,ticketCount]=people.front();
        //idx and ticketCount will not appear in debug window so it is better to print it
        std::cout << "idx = " << idx << ", ticketCount = " << ticketCount << "\n";
        people.pop();
        ticketCount--;
        timeCounter++;
        if(ticketCount>0){
            people.push({idx,ticketCount});
        }
        if (idx==k && ticketCount==0) {
            break;
        }
            
        }
    return timeCounter;
}
