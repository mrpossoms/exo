#include "exo.hpp"
#include "nix.hpp"
#include "sense/vision/feature_detectors.hpp"
#include "math/linalg.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test shi-tomasi corner detection"

using namespace exo::math;

template<size_t R, size_t C>
void print_matrix (Mat<float, R, C>& M)
{
    for (int r = 0; r < R; ++r)
    {
        for (int c = 0; c < C; ++c)
        {
            std::cerr << (M[r][c] > 5 ? "x" : " ");
        }
        std::cerr << "\n";
    }
}

#include "test.h"
{
    Mat<float, 8, 8> I = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 9, 9, 9, 9, 9, 9, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
    };

    Mat<float, 8, 8> corners;

    exo::sense::vision::shi_tomasi(I, corners);

    print_matrix(I);
    std::cerr << "------------------------------\n";
    print_matrix(corners);

    return 0;
}
