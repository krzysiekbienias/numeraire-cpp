#include <vector>
#include "stack/colliding_asteroids.hpp"


std::vector<int> collidingAsteroids(const std::vector<int>& asteroids){
    bool alive=true; // flag to monitor if current asteroid survive collition with the one of the top of the stack
    std::vector<int> stack; //will simulate final stage
    for (int asteroid:asteroids){
        // stack.back()>0 (->), asteroid <0 (<-)
        while(alive && !stack.empty()  && stack.back()>0 && asteroid<0 ) {
            if (abs(asteroid)>stack.back()){
                stack.pop_back(); //keep fighting
                
            }
            //both asteroid explode
            else if (abs(asteroid)==stack.back()){
                stack.pop_back();
                alive=false; // becasue current asteroid do not survive.
        }
            else{
                // current asteroid is distroyed so we take another one.
                alive=false;
            }
            //if asteroid survived all collisions or no collistion was possible [for instance first one goes to left]
            
            
        }
        if (alive){
            stack.push_back(asteroid);
        }
        
    }
    return stack;
}
