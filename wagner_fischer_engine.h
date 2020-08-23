#pragma once

#include <string>
#include <vector>

namespace Ylioja {

    //  Implementation of the Levenshtein distance algorithm from 
    //  The String-to-String Correction Problem, Robert A Wagner, Michael J.Fischer
    //  Journal of the ACM, 21:168-178, January 1974.
    //  https://doi.org/10.1145/321796.321811
    //
    //  This implementation is intended to serve as the "Gold Standard"
    //  for testing and verifying other, hopefully faster and more space
    //  efficient versions.
    //
    class WagnerFischerEngine
    {
    public:
        int levenshtein_distance(const std::string& word_a, const std::string& word_b, bool verify);

    private:
        void verify_result(const std::string& word_a, const std::string& word_b) const;
        std::string prepare_edit_sequence(const std::string& word_a, const std::string& word_b) const;

        //  Const and non-const 2D matrix like access to the distances vector
        int index(int row, int col) const { return row * m_column_count + col; }
        int distance(int row, int col) const { return m_distances[index(row, col)]; }
        int& distance(int row, int col) { return m_distances[index(row, col)]; }

        //  Dimensions of the distances matrix
        int m_row_count;
        int m_column_count;
        
        std::vector<int> m_distances;
    };

}  //  namespace Ylioja
