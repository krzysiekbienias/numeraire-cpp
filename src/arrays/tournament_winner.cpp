#include <vector>
#include <iostream>

// Implement your tournament_winner logic here.


std::string getWinner(std::vector<std::string>teams,int result) {
    if(result==0) {
        std::cout<<"Away team won"<<std::endl;
        return teams[1];
    }
    else {
        std::cout<<"Home team won"<<std::endl;
        return teams[0];
    }
}

std::string tournamentWinner(std::vector<std::vector<std::string>> competitions, std::vector<int> results) {
    // prepare a container for teams/
    
    std::unordered_map<std::string,int> table;
    for (const std::vector<std::string>& match : competitions) {
        for (const std::string & team : match) {
            table[team]=0;
        }
    }
    std::cout<<"In competition we have following teams:"<<std::endl;
    for (const auto & [k,v]:table) {
        std::cout << k<<":"<<v<<'\n';
    }
    
    for (size_t i =0;i<std::min(competitions.size(),results.size());i++) {
        std::string winnig_team=getWinner(competitions[i],results[i]);
        table[winnig_team]+=3;
    }
    
    std::string winner;
    int maxPoint=std::numeric_limits<int>::min();
    for (const auto& [k,v]:table){
        if (v>maxPoint) {
            maxPoint=v;
            winner=k;
        }
    }
    std::cout<<"Team "<<winner<<" took first place"<<std::endl;
    
    return winner;

 }
