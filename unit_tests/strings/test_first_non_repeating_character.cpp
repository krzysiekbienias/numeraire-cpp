#include "gtest/gtest.h"

#include "strings/first_non_repeating_character.hpp"

TEST(FirstNonRepeatingCharacterTest, BasicCases) {
    EXPECT_EQ(firstNonRepeatingCharacter("aabccbd"), 6); // 'd'
    EXPECT_EQ(firstNonRepeatingCharacter("leetcode"), 0); // 'l'
    EXPECT_EQ(firstNonRepeatingCharacter("loveleetcode"), 2); // 'v'
}

TEST(FirstNonRepeatingCharacterTest, AllRepeatingCharacters) {
    EXPECT_EQ(firstNonRepeatingCharacter("aabbcc"), -1);
    EXPECT_EQ(firstNonRepeatingCharacter("zzzz"), -1);
}

TEST(FirstNonRepeatingCharacterTest, FirstCharIsUnique) {
    EXPECT_EQ(firstNonRepeatingCharacter("zabcab"), 0); // 'z'
}

TEST(FirstNonRepeatingCharacterTest, LastCharIsUnique) {
    EXPECT_EQ(firstNonRepeatingCharacter("aabbccd"), 6); // 'd'
}

TEST(FirstNonRepeatingCharacterTest, OnlyOneCharacter) {
    EXPECT_EQ(firstNonRepeatingCharacter("x"), 0);
}

TEST(FirstNonRepeatingCharacterTest, EmptyString) {
    EXPECT_EQ(firstNonRepeatingCharacter(""), -1);
}
