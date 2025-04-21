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
