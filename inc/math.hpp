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
			Vec(S* arr)
			{
				for (int i = D; i--;)
				{
					v[i] = arr[i];
				}	
			}
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


			template<ssize_t ND>
			inline Vec<S, ND> as_dimension()
			{
				return Vec<S, ND>(this->v);
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

			static Vec<S,3> cross(Vec<S,3>& a, Vec<S,3>& b)
			{
				return Vec::cross(std::move(a), std::move(b));
			}

			static Vec<S,3> cross(Vec<S,3>&& a, Vec<S,3>&& b)
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

		struct Quat : public Vec<float, 4>
		{
			Quat() : Vec({ 0, 0, 0, 1 })
			{
				// NOP
			}

			Quat(float x, float y, float z, float w) : Vec({ x, y, z, w })
			{
				// NOP
			}

			Quat operator*(Quat& other)
			{
				return *this * std::move(other);
			}

			Quat operator*(Quat&& other)
			{
				auto t3 = this->as_dimension<3>();
				auto o3 = this->as_dimension<3>();

				auto r = Vec::cross(t3, o3);
				auto w = t3 * other[3];
				r += w;
				w = o3 * this->v[3];
				r += w;

				return { 
					r[0], r[1], r[2],
					this->v[3] * other.v[3] - t3.dot(o3)
				};
			}

			Quat& operator*=(Quat&& other)
			{
				*this = *this * other;

				return *this;
			}
		};
	}
}
