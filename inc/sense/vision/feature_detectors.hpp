#pragma once

#include "../../math/linalg.hpp"

using namespace exo::math

namespace exo
{
namespace sense
{
namespace vision
{

template<typename T, size_t R, size_t C>
Mat<T, R, C> partial(Mat<T, R, C> const& m)
{

}

template<typename T, size_t R, size_t C, size_t F_N>
void harris_stephens(Mat<T, R, C> I, Vec<size_t, 2>& features[F_N])
{
	
}

template<typename T, size_t R, size_t C, size_t F_N>
void shi_tomasi(Mat<T, R, C> I, Vec<size_t, 2>& features[F_N])
{

}

} // vision

} // sense

} // exo