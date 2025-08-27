#pragma once
#include <vector>
#include <string>

// Declare your phone_number_mnemonics interface here.
void backtrack(int index, const std::string & digits, const std::vector<std::string>& mapping,
               std::string& current, std::vector<std::string> result);

std::vector<std::string> phoneNumberMnemonics(std::string phoneNumber);
