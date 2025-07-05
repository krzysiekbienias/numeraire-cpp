#include <iostream>


bool isValidSequence(const std::vector<int>& array, const std::vector<int>& sequence) {
    size_t array_index=0;
    size_t sequence_index=0;
    while (array_index<array.size() and sequence_index<sequence.size()) {
        if (array[array_index]==sequence[sequence_index]) {
            array_index++;
            sequence_index++;
        }
        else
        {array_index++;}
    }
    return sequence_index==sequence.size();
}
