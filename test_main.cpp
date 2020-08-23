#include "levenshtein.h"
#include "wagner_fischer_engine.h"

#include <iostream>

namespace {

    class LevenshteinTester
    {
    public:
        void set_data(const char* text_a, const char* text_b, int expected_distance);
        bool distances_match(const char* msg);
        bool test_both();
        bool test_own();

    private:
        std::string m_text_a;
        std::string m_text_b;
        int m_expected_distance;

        int m_computed_distance;

        Ylioja::WagnerFischerEngine m_engine;
    };


    void LevenshteinTester::set_data(const char* text_a, const char* text_b, int expected_distance)
    {
        m_text_a = text_a;
        m_text_b = text_b;
        m_expected_distance = expected_distance;
    }


    bool LevenshteinTester::distances_match(const char* msg)
    {
        if (m_computed_distance == m_expected_distance)
        {
            return true;
        }

        std::cout
            << "Invalid result from " << msg
            << " - Expected: " << m_expected_distance
            << ", Got: " << m_computed_distance
            << '\n';

        std::cout << "Called with text_a: \"" << m_text_a << "\"\n";
        std::cout << "Called with text_b: \"" << m_text_b << "\"\n";

        return false;
    }


    bool LevenshteinTester::test_both()
    {
        m_computed_distance = m_engine.levenshtein_distance(m_text_a, m_text_b, true);
        if (!distances_match("Wagner Fischer"))
        {
            return false;
        }

        m_computed_distance = m_engine.levenshtein_distance(m_text_b, m_text_a, true);
        if (!distances_match("Reverse Wagner Fischer"))
        {
            return false;
        }

        return test_own();
    }


    bool LevenshteinTester::test_own()
    {
        m_computed_distance = Ylioja::levenshtein_distance(m_text_a, m_text_b);
        if (!distances_match("Ylioja::levenshtein_distance()"))
        {
            return false;
        }

        m_computed_distance = Ylioja::levenshtein_distance(m_text_a, m_text_b);
        if (!distances_match("Reverse Ylioja::levenshtein_distance()"))
        {
            return false;
        }
        
        return true;
    }


    struct TestCase {
        const char* text_a;
        const char* text_b;
        int distance;
    };

    //  Basic sanity tests.
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
    bool run_basic_tests(LevenshteinTester& tester)
    {
        for (const auto& testcase : basic_tests)
        {
            tester.set_data(testcase.text_a, testcase.text_b, testcase.distance);
            if (!tester.test_both())
            {
                return false;
            }
        }

        return true;
    }

}


int main(int, char**)
{
    LevenshteinTester tester;
    std::cout << "Running basic test\n";
    bool success = run_basic_tests(tester);
    if (!success)
    {
        std::cout << "Test FAILED\n";
        return 1;
    }

    std::cout << "Test OK\n";
    return 0;
}

 