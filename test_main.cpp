#include "levenshtein.h"
#include "wagner_fischer_engine.h"

#include <cstdlib>
#include <iostream>

namespace {

    struct TestCase {
        const char* text_a;
        const char* text_b;
        int distance;
    };

    //  Basic sanity tests.
    TestCase basic_tests[] =
    {
        { "1234567890abcdefghijklmnop", "ABCDEF", 26 },
        { "1234567890abcdefX", "X", 16 },
        { "1234567890abcdefg", "X", 17 },
        { "1234567890Xabcdefg", "X", 17 },
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
    };


    //  Generate a small reasonably random integer less than the given limit.
    //  Good enough as long as the given limit is below RAND_MAX.
    int random_int(int limit)
    {
        return std::rand() % limit;
    }


    class LevenshteinTester
    {
    public:
        bool run_basic_tests();
        bool run_random_tests(int count);

    private:
        void generate_random_input();
        bool distances_do_not_match(const char* method);
        bool test_both_implementations();
        bool test_the_own_implementation();

        std::string m_text_a;
        std::string m_text_b;
        int m_expected_distance;
        int m_computed_distance;

        Ylioja::WagnerFischerEngine m_engine;
    };


    bool LevenshteinTester::run_basic_tests()
    {
        for (const auto& testcase : basic_tests)
        {
            m_text_a = testcase.text_a;
            m_text_b = testcase.text_b;
            m_expected_distance = testcase.distance;
            if (!test_both_implementations())
            {
                return false;
            }
        }

        return true;
    }


    bool LevenshteinTester::run_random_tests(int count)
    {
        while (count-- > 0)
        {
            generate_random_input();
            m_text_b = m_text_a;

            generate_random_input();

            //  Expected result as computed by the "Gold Standard" implementation.
            m_expected_distance = m_engine.levenshtein_distance(m_text_a, m_text_b, true);
            
            if (!test_the_own_implementation())
            {
                return false;
            }
        }

        return true;
    }


    void LevenshteinTester::generate_random_input()
    {
        m_text_a.clear();
        int size = 1 + random_int(90);
        while (size--)
        {
            m_text_a += 'a' + random_int(5);
        }
    }


    bool LevenshteinTester::distances_do_not_match(const char* method)
    {
        if (m_computed_distance == m_expected_distance)
        {
            return false;   // Distances match
        }

        std::cout
            << "Invalid result from " << method
            << " expected: " << m_expected_distance
            << ", got: " << m_computed_distance
            << " instead\n";

        std::cout << "Called with text_a: \"" << m_text_a << "\", text_b: \"" << m_text_b << "\"\n";

        return true;    //  No match
    }


    bool LevenshteinTester::test_both_implementations()
    {
        m_computed_distance = m_engine.levenshtein_distance(m_text_a, m_text_b, true);
        if (distances_do_not_match("Wagner-Fischer"))
        {
            return false;
        }

        m_computed_distance = m_engine.levenshtein_distance(m_text_b, m_text_a, true);
        if (distances_do_not_match("Reverse Wagner-Fischer"))
        {
            return false;
        }

        return test_the_own_implementation();
    }


    bool LevenshteinTester::test_the_own_implementation()
    {
        m_computed_distance = Ylioja::levenshtein_distance(m_text_a, m_text_b);
        if (distances_do_not_match("Ylioja::levenshtein_distance()"))
        {
            return false;
        }

        m_computed_distance = Ylioja::levenshtein_distance(m_text_b, m_text_a);
        if (distances_do_not_match("Reverse Ylioja::levenshtein_distance()"))
        {
            return false;
        }
        
        return true;
    }

}   //  namespace


int main(int, char**)
{
    LevenshteinTester tester;
    std::cout << "Running basic tests.\n";
    bool success = tester.run_basic_tests();
    if (!success)
    {
        std::cout << "One of the basic tests FAILED\n";
        return 1;
    }

    int count = 10000;
    std::cout << "Running " << count << " random tests.\n";
    success = tester.run_random_tests(count);
    if (!success)
    {
        std::cout << "One of the random tests FAILED\n";
        return 2;
    }

    std::cout << "All tests passed OK\n";
    return 0;
}

 