#include <string>

std::string runLengthEncoding(const std::string& str){
    //edge case
    if (str.empty()) {
        return "";
    }
    char currentChar=str[0]; //important step to avoid str[i] and str [i-1] comparison
    int counter=1;
    std::string result;
    for (int i=1;i<str.size();++i){
        if(str[i]==currentChar){
            counter++;
            //split too long runs
            if (counter==10) {
                result+="9"+std::string(1,currentChar);
                counter=1;//<------reset counter this quarantees that counter never will be 11 for instance
            }
        }
        else{//finish current run for example AAA and swithc  to B
            result+=std::to_string(counter)+currentChar;
            currentChar=str[i]; //update currentChar
            counter=1; // reset
        }
    }
    //final run
    result+=std::to_string(counter)+currentChar;
    return result;
}

