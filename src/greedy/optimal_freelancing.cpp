#include<vector>
#include<unordered_map>
#include<string>


// Implement your optimal_freelancing logic here.
int optimalFreelancing(std::vector<std::unordered_map<std::string, int>> jobs){
    // Transform input job maps into structured Job objects for efficient processing
    const int LENGTH_OF_THE_WEEK=7;
    int totalProfit{0};
    
    struct Job{
        int deadline;
        int payment;
    };
    std::vector<Job> structuredJobs;
    for (const auto jobMap:jobs){
        int deadline=jobMap.at("deadline");
        int payment=jobMap.at("payment");
        structuredJobs.push_back(Job{deadline,payment});
    }
    // Now continue with:
    //1. Sorting by payment descending.
    
    std::sort(structuredJobs.begin(),structuredJobs.end(),[](const Job & jobOne, const Job & jobTwo){
        return jobOne.payment>jobTwo.payment;
    });
    //2. Slot allocation.
    
    std::vector<bool> timeline(LENGTH_OF_THE_WEEK,false);
    for(const auto& job:structuredJobs){
        int maxTime=std::min(job.deadline,LENGTH_OF_THE_WEEK); // it ensures that you never try to schedule a job past day 7, if deadline is larger.
        for(int day=maxTime-1;day>=0;--day){
            if (!timeline[day]) {
                timeline[day]=true; //book the day
                totalProfit+=job.payment; //add profit
                break; //move to next job
            }
        }
    }
    return totalProfit;
}
