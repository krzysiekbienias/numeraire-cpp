#include <vector>

// Implement your daily_temperatures logic here.
std::vector<int> dailyTemperatures(const std::vector<int>& temperatures){
    std::vector<int>answer(temperatures.size(),0);//zeros handles no warmer dau case by default.
    std:: vector<int> stack;//it stores indices
    for (int i =temperatures.size()-1;i>=0;--i){//we are moving from right to left. I'll know warmer days ahead
        while(!stack.empty() && temperatures[stack.back()]<=temperatures[i]){
            stack.pop_back(); //popping until we find a warmer day
        }
        if (!stack.empty()){
            answer[i]=stack.back()-i; //difference in indeces
        }
        stack.push_back(i);
    }
    return answer;
    
}
