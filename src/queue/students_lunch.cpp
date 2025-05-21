#include <vector>
#include "queue/students_lunch.hpp"
#include<queue>

// Implement your students_lunch logic here.
int countStudents(const std::vector<int>& students, const std::vector<int>&sandwiches){
    std::queue<int> studentQueue;
    for (int student:students){
        studentQueue.push(student);
    }
    int sandwichIndex=0;
    int rotations=0;
    while(!studentQueue.empty()){
        // if student at the front wants the sandwitch at the stop of the stack (current element from left) both are removed becasue the student takes the sandwitch
        if (studentQueue.front()==sandwiches[sandwichIndex]) {
            studentQueue.pop();
            sandwichIndex++;
            rotations=0; //reset rotations becasue we made a progress
        }
        else{
            studentQueue.push(studentQueue.front());
            studentQueue.pop();
            rotations++;
        }
        if (rotations==studentQueue.size()){
            break;
        
        }
    }
    return studentQueue.size();
}
