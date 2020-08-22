#include "wagner_fischer_engine.h"

#include <iostream>

namespace {

    struct TestCase {
        const char* text_a;
        const char* text_b;
        int distance;
    };

    //  Simple very basic sanity tests.
    TestCase basic_tests[] =
    {
        { "a", "", 1 },
        { "a", "b", 1 },
        { "ab", "a", 1 },
        { "ab", "b", 1 },
        { "ab", "", 2 },
        { "ab", "c", 2 },
        { "ab", "cd", 2 },
        { "abc", "bc", 1 },
        { "abc", "ac", 1 },
        { "abc", "ab", 1 },
        { "abcd", "", 4 },
        { "abcd", "e", 4 },
        { "abcd", "cd", 2 },
        { "abcd", "ad", 2 },
        { "abcd", "ab", 2 },
        { "abcdefg", "Xabcdefg", 1 },
        { "abcdefg", "abXcdefg", 1 },
        { "abcdefg", "abcdXefg", 1 },
        { "abcdefg", "abcdefXg", 1 },
        { "abcdefg", "AabXcdefg", 2 },
        { "abcdefg", "abAcdXefg", 2 },
        { "abcdefg", "abcdefXgA", 2 },
        { "abcdefg", "ABabXcdefg", 3 },
        { "abcdefg", "abAcdXBefg", 3 },
        { "abcdefg", "abcBdefXgA", 3 },
        { "abcdefg", "abdefg", 1 },
        { "abcdefg", "abcdfg", 1 },
        { "abcdefg", "abcdef", 1 },
        { "abcdefg", "bdefg", 2 },
        { "abcdefg", "acdfg", 2 },
        { "abcdefg", "abdef", 2 },
        { "abcdefg", "bdfg", 3 },
        { "abcdefg", "acfg", 3 },
        { "abcdefg", "abdf", 3 },
        { "abcdefg", "bXdfg", 3 },
        { "abcdefg", "acYfg", 3 },
        { "abcdefg", "abdZf", 3 },
        { "abcdefg", "AbXdfg", 3 },
        { "abcdefg", "acYAfg", 3 },
        { "abcdefg", "abdZfA", 3 },
        { "abcdefg", "ABbXdfg", 4 },
        { "abcdefg", "acYABfg", 4 },
        { "abcdefg", "abdZfAB", 4 },
        { "abcdefg", "ABXdfg", 4 },
        { "abcdefg", "acYABg", 4 },
        { "abcdefg", "abdZAB", 4 },
        { "1234567890abcdef", "ABCDEF", 16 },
        { "1234567890abcdefX", "X", 16 },
        { "1234567890abcdefg", "X", 17 },
        { "1234567890Xabcdefg", "X", 17 },
    };


    //  Returns true if no problems
    bool run_basic_tests(Ylioja::WagnerFischerEngine& engine)
    {
        std::string worda;
        std::string wordb;
        for (const auto& testcase : basic_tests)
        {
            worda = testcase.text_a;
            wordb = testcase.text_b;
            int expected = testcase.distance;

            //  Check with the "Gold Standard" implementation of Wagner and Fischer algorithm
            int distance = engine.levenshtein_distance(worda, wordb, true);
            if (distance != expected)
            {
                return false;
            }

            //  Should get the same result both ways
            distance = engine.levenshtein_distance(wordb, worda, true);
            if (distance != expected)
            {
                return false;
            }
        }

        return true;
    }

}


int main(int, char**)
{
    Ylioja::WagnerFischerEngine engine;
    std::cout << "Running basic test\n";
    bool success = run_basic_tests(engine);
    if (!success)
    {
        std::cout << "Test FAILED\n";
        return 1;
    }

    std::cout << "Test OK\n";
    return 0;
}

 