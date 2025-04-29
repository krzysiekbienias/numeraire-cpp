#include <vector>

// Implement your move_element_to_the_end logic here.
std::vector<int> moveElementToTheEnd(const std::vector<int>& array ,int toMove){
    std::vector<int> arrayCoppy=array;
    int pointer{0};
    for (int i=0;i<arrayCoppy.size();++i){
        if (arrayCoppy[i]!=toMove){
            std::swap(arrayCoppy[i], arrayCoppy[pointer]);
            ++pointer;
        }
    }
    return arrayCoppy;
}

// Since the task requires in-place modification, I pass the array by non-const reference.
// The function returns void because all changes are made directly to the provided array.
// I accept toMove by value because it’s a simple integer.

void moveElementToTheEndInPlace(std::vector<int>& array ,int toMove){
    int pointer{0};
    for (int i=0;i<array.size();++i){
        if (array[i]!=toMove){
            std::swap(array[i], array[pointer]);
            ++pointer;
        }
    }
}
