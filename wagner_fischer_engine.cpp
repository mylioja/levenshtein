#include "wagner_fischer_engine.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace Ylioja {

    namespace {

        //  Edit operations used in the edit sequence string
        //  Operations are relative to the string a
        enum {
            KEEP = 'K',     //  Keep the byte as it is - no change
            DELETE = 'D',   //  Delete the byte
            INSERT = 'I',   //  Insert a byte from string b
            REPLACE = 'R',  //  Replace the byte by one from string b
        };


        int compare(int a, int b)
        {
            return (a == b) ? 0 : 1;
        }


        int minimum(int a, int b, int c)
        {
            return std::min(std::min(a, b), c);
        }


        //  Append an edit operation to the edit sequence
        void add_edit(std::string& sequence, char operation, char ch)
        {
            sequence += ch;
            sequence += operation;
        }


        void fatal_error(const char* message)
        {
            std::cerr << "Fatal error in WagnerFischerEngine: " << message << '\n';
            std::cerr << "Calling std::abort()" << std::endl;
            std::abort();
        }

        void must_match(int a, int b)
        {
            if (a != b)
            {
                fatal_error("Character mismatch in edit string");
            }
        }

    }   //  namespace


    //  Levenshtein distance algorithm from 
    //  The String-to-String Correction Problem, Robert A Wagner, Michael J.Fischer
    //  Journal of the ACM, 21:168-178, January 1974.
    //  https://doi.org/10.1145/321796.321811
    //
    //  Can optionally compute an edit sequence using the "Algorithm Y" from the same paper,
    //  and use it to double check the result.
    //
    int WagnerFischerEngine::levenshtein_distance(const std::string& word_a, const std::string& word_b, bool verify)
    {
        int size_a = word_a.size();
        int size_b = word_b.size();

        //  Distance matrix dimensions are one larger than the corresponding text sizes
        m_row_count = size_a + 1;
        m_column_count = size_b + 1;

        size_t required_size = m_row_count * m_column_count;
        if (required_size > m_distances.size())
        {
            m_distances.resize(required_size);
        }

        //  Column 0 represents deletions from word_a
        for (int ia = 0; ia <= size_a; ++ia)
        {
            distance(ia, 0) = ia;
        }

        //  Row 0 represents insertions to word_b
        for (int ib = 1; ib <= size_b; ++ib)
        {
            distance(0, ib) = ib;
        }

        //  Incrementally compute the rest row by row, left to right,
        //  always choosing the minimum distance found so far.
        for (int ia = 1; ia <= size_a; ++ia)
        {
            for (int ib = 1; ib <= size_b; ++ib)
            {
                int deletion = distance(ia - 1, ib) + 1;
                int insertion = distance(ia, ib - 1) + 1;
                int replacement = distance(ia - 1, ib - 1) + compare(word_a[ia - 1], word_b[ib - 1]);
                distance(ia, ib) = minimum(deletion, insertion, replacement);
            }
        }

        //  Rightmost value on the last row is the final result
        int result = distance(size_a, size_b);

        //  Result is now ready. Verify it if requested.
        if (verify)
        {
            verify_result(word_a, word_b);
        }

        return result;
    }


    //  Verify the result by editing word_a according to the computed backtrace.
    //  The edited result text must be equal to word_b.
    //
    bool WagnerFischerEngine::verify_result(const std::string& word_a, const std::string& word_b) const
    {
        std::string edit = prepare_edit_sequence(word_a, word_b);

        std::string text;
        text.reserve(word_b.size());

        int ia = 0; //  index to word_a
        int ib = 0; //  index to word_b
        int ix = 0; //  index to the edit sequence
        const int ix_end = edit.size();
        while (ix < ix_end)
        {
            char operation = edit[ix++];
            char ch = edit[ix++];
            switch (operation)
            {
            case DELETE:
                must_match(ch, word_a[ia++]);
                break;

            case INSERT:
                must_match(ch, word_b[ib++]);
                text.push_back(ch);
                break;

            case REPLACE:
                must_match(ch, word_a[ia++]);
                ch = edit[ix++];
                must_match(ch, word_b[ib++]);
                text.push_back(ch);
                break;

            case KEEP:
                must_match(ch, word_a[ia++]);
                must_match(ch, word_b[ib++]);
                text.push_back(ch);
                break;

            default:
                fatal_error("Invalid operation in edit sequence");
                return false;
            }
        }

        if (text != word_b)
        {
            fatal_error("Edited result not as expected");
            return false;
        }

        return true;
    }


    //
    //  Prepare an edit sequence to allow double checking the result.
    //  This is based on the backtrace "Algorithm Y" from Wagner and Fischer.
    //  Multiple edit sequences with the same minimum length may be possible.
    //  Only one of the possible choices is taken: The one given by "Algorithm Y".
    //
    std::string WagnerFischerEngine::prepare_edit_sequence(const std::string& word_a, const std::string& word_b) const
    {
        int ia = m_row_count - 1;
        int ib = m_column_count - 1;
  
        std::string sequence;
        sequence.reserve(3 * word_a.size());    //  Can't be longer than this

        while (ia && ib)
        {
            if (distance(ia, ib) == distance(ia - 1, ib) + 1)
            {
                ia = ia - 1;
                add_edit(sequence, DELETE, word_a[ia]);  // Delete a character from word_a
            }
            else if (distance(ia, ib) == distance(ia, ib - 1) + 1)
            {
                ib = ib - 1;
                add_edit(sequence, INSERT, word_b[ib]);  // Insert a character from word_b into word_a
            }
            else
            {
                ia = ia - 1;
                ib = ib - 1;
                if (word_a[ia] != word_b[ib])
                {
                    sequence += word_b[ib];  // The replacement character from word_b
                    add_edit(sequence, REPLACE, word_a[ia]);  // Replace a character in word_a with a character from word_b
                }
                else
                {
                    //  Not really an edit, so not counted as part of the edit distance
                    add_edit(sequence, KEEP, word_a[ia]);  // Keep the character in word_a unchanged
                }
            }
        }

        while (ia)
        {
            ia = ia - 1;
            add_edit(sequence, DELETE, word_a[ia]);
        }

        while (ib)
        {
            ib = ib - 1;
            add_edit(sequence, INSERT, word_b[ib]);
        }

        //  Reverse the sequence for easier left to right reading
        std::reverse(sequence.begin(), sequence.end());

        return sequence;
    }

}  //  namespace Ylioja
