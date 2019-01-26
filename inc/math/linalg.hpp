#pragma once
#include "../exo.hpp"
#include <math.h>

namespace exo
{
	namespace math
	{
		template<typename S, ssize_t D>
		struct Vec
		{
			Vec()
			{
				for (int i = D; i--;) { v[i] = 0; }
			}

			Vec(S* arr)
			{
				for (int i = D; i--;) { v[i] = arr[i]; }
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


			inline Vec<S,D> operator+(Vec<S,D>& v) { return *this + std::move(v); }
			inline Vec<S,D> operator+(Vec<S,D>&& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] + v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator-(Vec<S,D>& v) { return *this - std::move(v); }
			inline Vec<S,D> operator-(Vec<S,D>&& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] - v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator*(Vec<S,D>& v) { return *this * std::move(v); }
			inline Vec<S,D> operator*(Vec<S,D>&& v)
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

			inline Vec<S,D> operator/(Vec<S,D>& v) { return *this / std::move(v); }
			inline Vec<S,D>  operator/(Vec<S,D>&& v)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] / v.v[i];
				}
				return out;
			}

			inline Vec<S,D> operator/(S s)
			{
				Vec<S,D> out;
				for (auto i = D; i--;)
				{
					out.v[i] = this->v[i] / s;
				}
				return out;
			}

			inline Vec<S,D> operator+=(Vec<S,D>& v) { return *this += std::move(v); }
			inline Vec<S,D>& operator+=(Vec<S,D>&& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] += v.v[i];
				}
				return *this;
			}

			inline Vec<S,D> operator-=(Vec<S,D>& v) { return *this -= std::move(v); }
			inline Vec<S,D>& operator-=(Vec<S,D>&& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] -= v.v[i];
				}
				return *this;
			}

			inline Vec<S,D> operator*=(Vec<S,D>& v) { return *this *= std::move(v); }
			inline Vec<S,D>& operator*=(Vec<S,D>&& v)
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


			inline Vec<S,D> operator/=(Vec<S,D>& v) { return *this /= std::move(v); }
			inline Vec<S,D>& operator/=(Vec<S,D>&& v)
			{
				for (auto i = D; i--;)
				{
					this->v[i] /= v.v[i];
				}
				return *this;
			}

			inline Vec<S,D>& operator/=(S s)
			{
				for (auto i = D; i--;)
				{
					this->v[i] /= s;
				}
				return *this;
			}

			inline bool operator!=(Vec<S,D>& v) { return !(*this == std::move(v)); }
			inline bool operator!=(Vec<S,D>&& v) { return !(*this == v); }

			inline bool operator==(Vec<S,D>& v) { return *this == std::move(v); }
			inline bool operator==(Vec<S,D>&& v)
			{
				for (auto i = D; i--;)
				{
					if (v[i] != this->v[i]) { return false; }
				}

				return true;
			}

			template<ssize_t ND>
			inline Vec<S, ND> as_dimension()
			{
				return Vec<S, ND>(this->v);
			}

			template<typename T>
			inline Vec<T, D> cast()
			{
				Vec<T, D> v;
				for (int i = 0; i < D; ++i)
				{
					v[i] = (T)this->v[i];
				}

				return v;
			}

			std::string to_string()
			{
				std::string str = "(";
				for (int i = 0; i < D; ++i)
				{
					str += std::to_string(v[i]);
					if (i < D - 1) { str += ", "; }
				} str += ")";

				return str;
			}


			S len() { return sqrt(dot(*this)); }

			Vec<S,D> norm() { return *this / len(); }

			S dot(Vec<S,D>& v)
			{
				S sum = 0;
				for (auto i = D; i--;)
				{
					sum += this->v[i] * v.v[i];
				}
				return sum;
			}

			bool is_near(Vec<S,D>& v, S threshold) { return this->is_near(std::move(v)); }
			bool is_near(Vec<S,D>&& v, S threshold)
			{
				auto diff = *this - v;

				return diff.dot(diff) <= threshold;
			}

			static S cross(Vec<S,2>& a, Vec<S,2>& b)
			{
				return a[0]*b[1] - a[1]*b[0];
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
		using Vec2i = Vec<int, 2>;
		using Vec2f = Vec<float, 2>;
		using Vec3f = Vec<float, 3>;

		template<typename S, ssize_t D>
		struct Ray
		{
			Ray(Vec<S, D> origin, Vec<S, D> direction)
			{
				this->origin = origin;
				this->direction = direction;
			}

			Vec<S, D> origin;
			Vec<S, D> direction;
		};
		using Ray3f = Ray<float, 3>;

		template<typename S, ssize_t R, ssize_t C>
		struct Mat
		{
			Mat()
			{
				bzero(m, sizeof(m));
			}

			Mat(std::initializer_list<std::initializer_list<S>> init)
			{
				int ri = 0;
				for (auto row : init)
				{
					int ci = 0;
					for (auto c : row)
					{
						m[ri][ci] = c;
						ci += 1;
					}
					ri += 1;
				}

			}

			template <ssize_t MC>
			Mat<S, R, MC> operator* (Mat<S, C, MC>& m)
			{
				Mat<S, R, MC> r;

				for (int row = R; row--;)
				for (int col = MC; col--;)
				{
					for (int i = C; i--;)
					{
						r[row][col] += this->m[row][i] * m[i][col];
					}
				}

				return r;
			}

			S* operator[] (ssize_t row)
			{
				if (row < R) { return m[row]; }
				return nullptr;
			}

			bool operator== (Mat<S, R, C> M)
			{
				return memcmp(this->m, M.m, sizeof(M.m)) == 0;
			}

			// template <typename STORAGE, ssize_t ROWS, ssize_t COLS>
			static Mat<S, R, C> I()
			{
				Mat<S, R, C> res;

				for (int r = R; r--;)
				for (int c = C; c--;)
				{
					if (r == c) { res[r][c] = 1; }
				}

				return res;
			}

			static Mat<S, 4, 4> ortho(S left, S right, S top, S bottom, S near, S far)
			{
				auto tmb = top - bottom;
				auto tpb = top + bottom;
				auto rml = right - left;
				auto rpl = right + left;
				auto fmn = far - near;
				auto fpn = far + near;

				return {
					{ 2/rml,     0,      0, -rpl/rml },
					{     0, 2/tmb,      0, -tpb/tmb },
					{     0,     0, -2/fmn, -fpn/fmn },
					{     0,     0,      0,        1 }
				};
			}

			S m[R][C];
		};
		using Mat4f = Mat<float, 4, 4>;

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

			Quat& operator*=(Quat& other)
			{
				*this = *this * std::move(other);
				return *this;
			}

			Quat conjugate()
			{
				auto& q = *this;
				return { -q[0], -q[1], -q[2], q[3] };
			}

			Quat inverse()
			{
				auto inv = this->conjugate();
				auto mag2 = this->dot(*this);
				static_cast<Vec<float, 4>>(inv) /= mag2;
				return inv;
			}

			Vec<float, 3> rotate(Vec<float, 3>& v)
			{
				return this->rotate(std::move(v));
			}

			Vec<float, 3> rotate(Vec<float, 3>&& v)
			{
				auto q_xyz = this->as_dimension<3>();

				auto t = Vec::cross(q_xyz, v);
				t *= 2;

				auto u = Vec::cross(q_xyz, t);
				t *= this->v[3];

				return v + t + u;
			}

			static Quat from_axis_angle(Vec<float, 3> axis, float angle)
			{
				auto a_2 = angle / 2;
				auto a = sinf(a_2);

				axis *= a;

				return { axis[0], axis[1], axis[2], cosf(a_2) };
			}
		};

		template<typename S, ssize_t D>
		struct Basis
		{
			Basis()
			{
				for(int i = D; i--;)
				{
					vectors[i][i] = 1;
				}
			}

			Basis(std::initializer_list<Vec<S, D>> basis_vectors)
			{
				int i = 0;
				for (auto vector : basis_vectors)
				{
					vectors[i++] = vector;
				}
			}

			inline Basis<S, 3> rotate(Quat& q) { return rotate(std::move(q)); }
			inline Basis<S, 3> rotate(Quat&& q)
			{
				return {
					q.rotate(vectors[0]),
					q.rotate(vectors[1]),
					q.rotate(vectors[2])
				};
			}

			Vec<S, D> vectors[D];
			Vec<S, D> origin;
		};
	}
}