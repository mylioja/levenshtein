#pragma once

#include <string>

namespace Ylioja {

    //
    //  This is my fast and simple version of the Wagner-Fischer Levenshtein distance algorithm.
    //  It's optimized for short (up to LEVENSHTEIN_MAX_INPUT_SIZE bytes) strings.
    //  I needed the algorithm for comparing Finnish words, so even the current size limit
    //  is much higher than really needed in my use-case.
    //
    
    //  Computes Levenshtein distance between two strings.
    //
    //  The string arguments are treated as byte strings with each byte counted as a character.
    //
    //  If shorter of the two arguments is longer than LEVENSHTEIN_MAX_INPUT_SIZE,
    //  the function returns -1 to indicate an error.
    //
    int levenshtein_distance(const std::string& word_a, const std::string& word_b);


    //  Shorter of the two arguments must not be longer than this limit.
    //  There's no restriction for the size of the longer word.
    //  
    constexpr int LEVENSHTEIN_MAX_INPUT_SIZE = 100;

}  //  namespace Ylioja

