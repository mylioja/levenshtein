#pragma once

#include <string>

namespace Ylioja {

    //
    //  Fast and simple version of the Wagner and Fischer Levenshtein distance algorithm.
    //  Optimized for short (up to LEVENSHTEIN_MAX_INPUT_SIZE bytes) strings.
    //  I initially needed the algorithm for comparing Finnish words, so even the current
    //  limit is much higher than length of any valid word encountered in real usage.
    //
    
    //  This function computes Levenshtein distance between two strings.
    //
    //  The string arguments are treated as byte strings with one charater per byte.
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

