#include "levenshtein.h"

#include <algorithm>

namespace Ylioja {

    //  Optimized version of the Wagner and Fischer Levenshtein distance algorithm from
    //  The String-to-String Correction Problem, Robert A Wagner, Michael J.Fischer
    //  Journal of the ACM, 21:168-178, January 1974.
    //  https://doi.org/10.1145/321796.321811
    //
    int levenshtein_distance(const std::string& word_a, const std::string& word_b)
    {
        int size_a = word_a.size();
        const char* text_a = word_a.data();

        int size_b = word_b.size();
        const char* text_b = word_b.data();

        //  Make sure text_b is the shorter one
        if (size_b > size_a)
        {
            std::swap(size_a, size_b);
            std::swap(text_a, text_b);
        }

        //  If the shorter text is empty, distance is equal to the size of the other
        if (size_b == 0)
        {
            return size_a;
        }

        //  If the shorter text is too long for our fixed buffer, return -1
        if (size_b > LEVENSHTEIN_MAX_INPUT_SIZE)
        {
            return -1;
        }

        //  Buffer for a single row of the distance matrix for LEVENSHTEIN_MAX_INPUT_SIZE input.
        //  The complete distance matrix isn't required, so the algorithm uses only enough space
        //  to hold one row of the matrix.
        int row[LEVENSHTEIN_MAX_INPUT_SIZE + 1];

        //  Initial values for row 0 
        for (int ib = 0; ib <= size_b; ++ib)
        {
            row[ib] = ib;
        }

        //  Row by row computation based on the Wagner and Fischer "Algorithm X"
        int distance;
        for (int ia = 0; ia < size_a; ++ia)
        {
            distance = ia + 1;
            for (int ib = 0; ib < size_b; ++ib)
            {
                int next_distance = row[ib];
                if (text_a[ia] != text_b[ib])
                {
                    next_distance += 1;
                }

                row[ib] = distance;

                distance += 1;

                if (next_distance < distance)
                {
                    distance = next_distance;
                }

                next_distance = row[ib + 1] + 1;
                if (next_distance < distance)
                {
                    distance = next_distance;
                }
            }

            row[size_b] = distance;
        }

        return distance;
    }

}  //  namespace Ylioja

