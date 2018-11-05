#pragma once
#include "exo.hpp"

namespace exo
{
	namespace math
	{
		template<typename S, ssize_t D>
		struct Vec
		{
			Vec() = default;
			Vec(std::initializer_list<S> init)
			{
				if (init.size() < D)
				{
					for (auto i = D; i--;)
					{
						v[i] = *init.begin();
					}
				}
				else
				{
					auto i = 0;
					for (auto s : init)
					{
						v[i++] = s;
					}
				}
			}

			inline S& operator[](int i)
			{
				return v[i];
			}

			inline Vec<S,D> operator+(Vec<S,D>& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] + v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator-(Vec<S,D>& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] - v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator*(Vec<S,D>& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] * v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator*(S s)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] * s;
				}
				return out;
			}

			inline Vec<S,D>  operator/(Vec<S,D>& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] / v.v[i];
				}
				return out;
			}


			inline Vec<S,D>& operator+=(Vec<S,D>& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] += v.v[i];
				}
				return *this;
			}

			inline Vec<S,D>& operator-=(Vec<S,D>& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] -= v.v[i];
				}
				return *this;
			}

			inline Vec<S,D>& operator*=(Vec<S,D>& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] *= v.v[i];
				}
				return *this;
			}

			inline Vec<S,D>& operator*=(S s)
			{
				for (auto i = D; i--;)
				{
					this->v[i] *= s;
				}
				return *this;
			}

			inline Vec<S,D>& operator/=(Vec<S,D>& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] /= v.v[i];
				}
				return *this;
			}


			S dot(Vec<S,D>& v)
			{
				S sum;
				for (auto i = D; i--;)
				{
					sum += this->v[i] * v.v[i];
				}
				return sum;
			}


			static Vec<S,3> cross(Vec<S,3> a, Vec<S,3> b)
			{
				return {
					a[1]*b[2] - a[2]*b[1],
					a[2]*b[0] - a[0]*b[2],
					a[0]*b[1] - a[1]*b[0],
				};
			}

			S v[D]; // value store
		};

		template<typename S, ssize_t R, ssize_t C>
		struct Mat
		{
			Mat(std::initializer_list<std::initializer_list<S>> init)
			{
				int ri = 0;
				for (auto row : init)
				{
					int ci = 0;
					for (auto c : row)
					{
						init[ri][ci] = c;
						ci += 1;
					}
					ri += 1;
				}

			}

			S m[R][C];
		};
	}
}
