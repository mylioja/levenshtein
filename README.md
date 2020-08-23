# levenshtein

A fast and simple Levenshtein string distance algorithm.

I needed a good Levenshtein string distance algorithm for a personal Optical charater reader (OCR) project.
After browsing through tens of publicly available open source implementations I decided to write my own.

This one is optimized for a single use-case: Computing the distance between two short (less than 100 bytes) strings.

The repository contains also an accurate and simple implementation of the classic Wagner-Fischer "Algorithm X" from

The String-to-String Correction Problem, Robert A Wagner, Michael J.Fischer
Journal of the ACM, 21:168-178, January 1974.
https://doi.org/10.1145/321796.321811

It's used as the "Gold Standard" to verify the faster version.

It can internally check itself by creating a minimal edit sequence using the "Algorithm Y" from the same paper,
and then applying the edit operations to the first string. The result must be equal to the other string.
