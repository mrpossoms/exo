#pragma once
#include "../exo.hpp"
#include <math.h>
#include <functional>

namespace exo
{
    namespace math
    {
        //----------------------------------------------------------------------
        //   __   __
        //   \ \ / /__ __
        //    \ V / -_) _|
        //     \_/\___\__|
        //
        template<typename S, size_t D>
        struct Vec
        {
            Vec()
            {
                for (int i = D; i--;) { v[i] = {}; }
            }

            Vec(const S* arr)
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

            inline S operator[](int i) const
            {
                return v[i];
            }

            inline S& operator[](int i)
            {
                return v[i];
            }

            inline S at(int i) const
            {
                return v[i];
            }

            inline S* ptr() { return v; }

            inline Vec<S,D> operator+(const Vec<S,D>& v) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] + v.v[i];
                }
                return out;
            }


            inline Vec<S,D> operator-(const Vec<S,D>& v) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] - v.v[i];
                }
                return out;
            }


            inline Vec<S,D> operator*(const Vec<S,D>& v) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] * v.v[i];
                }
                return out;
            }


            inline Vec<S,D> operator*(const S s) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] * s;
                }
                return out;
            }


            inline Vec<S,D>  operator/(const Vec<S,D>& v) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] / v.v[i];
                }
                return out;
            }


            inline Vec<S,D> operator/(const S s) const
            {
                Vec<S,D> out;
                for (auto i = D; i--;)
                {
                    out.v[i] = this->v[i] / s;
                }
                return out;
            }


            inline Vec<S,D>& operator+=(const Vec<S,D>& v)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] += v.v[i];
                }
                return *this;
            }


            inline Vec<S,D>& operator-=(const Vec<S,D>& v)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] -= v.v[i];
                }
                return *this;
            }


            inline Vec<S,D>& operator*=(const Vec<S,D>& v)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] *= v.v[i];
                }
                return *this;
            }


            inline Vec<S,D>& operator*=(const S s)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] *= s;
                }
                return *this;
            }


            inline Vec<S,D>& operator/=(const Vec<S,D>& v)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] /= v.v[i];
                }
                return *this;
            }


            inline Vec<S,D>& operator/=(const S s)
            {
                for (auto i = D; i--;)
                {
                    this->v[i] /= s;
                }
                return *this;
            }


            inline bool operator!=(const Vec<S,D>& v) const { return !(*this == v); }


            inline bool operator==(const Vec<S,D>& v) const
            {
                for (auto i = D; i--;)
                {
                    if (v.at(i) != this->at(i)) { return false; }
                }

                return true;
            }

            template<size_t ND>
            inline Vec<S, ND> as_dimension() const
            {
                return Vec<S, ND>(this->v);
            }

            template<typename T>
            inline Vec<T, D> cast() const
            {
                Vec<T, D> v;
                for (size_t i = 0; i < D; ++i)
                {
                    v[i] = (T)this->v[i];
                }

                return v;
            }

            template<size_t ND>
            Vec<S, ND> slice(size_t start = 0)
            {
                Vec<S, ND> r;
                for (size_t i = 0; i < ND; ++i) { r[i] = v[i + start]; }
                return r;
            }


            std::string to_string()
            {
                std::string str = "(";
                for (size_t i = 0; i < D; ++i)
                {
                    str += std::to_string(v[i]);
                    if (i < D - 1) { str += ", "; }
                } str += ")";

                return str;
            }


            inline S len()
            {
                auto sqr = dot(*this);
                if (sqr <= 0) { return 0; }
                return sqrtf(static_cast<float>(sqr));
            }


            Vec<S,D> norm() { return *this / len(); }


            S dot(Vec<S,D> const& v) const
            {
                S sum = 0;
                for (auto i = D; i--;)
                {
                    sum += this->v[i] * v.v[i];
                }
                return sum;
            }


            bool is_near(const Vec<S,D>& v, S threshold=0.0001)
            {
                auto diff = *this - v;

                return diff.dot(diff) <= threshold;
            }


            Vec<S,D>& take_min(const Vec<S,D>& v)
            {
                for (size_t i = 0; i < D; ++i)
                {
                    auto& cur = this->v[i];
                    cur = v[i] < cur ? v[i] : cur;
                }

                return *this;
            }


            Vec<S,D>& take_max(const Vec<S,D>& v)
            {
                for (size_t i = 0; i < D; ++i)
                {
                    auto& cur = this->v[i];
                    cur = v[i] > cur ? v[i] : cur;
                }

                return *this;
            }

            static S cross(Vec<S,2>& a, Vec<S,2>& b)
            {
                return a[0]*b[1] - a[1]*b[0];
            }

            static Vec<S,3> cross(Vec<S,3> const& a, Vec<S,3> const& b)
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

        //----------------------------------------------------------------------
        //    ___
        //   | _ \__ _ _  _
        //   |   / _` | || |
        //   |_|_\__,_|\_, |
        //             |__/
        template<typename S, size_t D>
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

        //----------------------------------------------------------------------
        //    __  __      _
        //   |  \/  |__ _| |_
        //   | |\/| / _` |  _|
        //   |_|  |_\__,_|\__|
        //
        template<typename S, size_t R, size_t C>
        struct Mat
        {
            Mat()
            {
                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    m[row][col] = {};
                }
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

            Mat<S, R, C>& initialize(std::function<S (S r, S c)> init)
            {
                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    m[row][col] = init(static_cast<S>(row), static_cast<S>(col));
                }

                return *this;
            }


            inline S* ptr() { return m[0].ptr(); }


            template<typename T>
            inline Mat<T, R, C> cast() const
            {
                Mat<T, R, C> r;
                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = (T)m[row][col];
                }

                return r;
            }

            template<size_t NR, size_t NC>
            Mat<S, NR, NC> slice(size_t start_row, size_t start_col)
            {
                Mat<S, NR, NC> r;

                for (int row = NR; row--;)
                {
                    r[row] = m[row + start_row].template slice<NC>(start_col);
                }

                return r;
            }

            template <size_t MC>
            Mat<S, R, MC> operator* (Mat<S, C, MC> const& m) const
            {
                Mat<S, R, MC> r;

                for (int row = R; row--;)
                for (int col = MC; col--;)
                {
                    for (int i = C; i--;)
                    {
                        r[row][col] += this->m[row][i] * m.m[i][col];
                    }
                }

                return r;
            }


            Vec<S, R> operator* (Vec<S, C> const& v) const
            {
                Vec<S, R> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row] += this->m[row][col] * v[col];
                }

                return r;
            }

            Mat<S, R, C> operator* (S s) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = this->m[row][col] * s;
                }

                return r;
            }

            Mat<S, R, C>& operator*= (S s)
            {
                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    m[row][col] *= s;
                }

                return *this;
            }

            Mat<S, R, C> operator/ (S s) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = this->m[row][col] / s;
                }

                return r;
            }

            Mat<S, R, C>& operator/= (S s)
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    m[row][col] /= s;
                }

                return *this;
            }


            Mat<S, R, C> operator+= (Mat<S, R, C> const& m) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] += m.m[row][col];
                }

                return r;
            }


            Mat<S, R, C> operator+ (Mat<S, R, C> const& m) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = this->m[row][col] + m.m[row][col];
                }

                return r;
            }


            Mat<S, R, C> operator- (Mat<S, R, C> const& m) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = this->m[row][col] - m.m[row][col];
                }

                return r;
            }


            Mat<S, R, C> operator-= (Mat<S, R, C> const& m) const
            {
                Mat<S, R, C> r;

                for (int row = R; row--;)
                for (int col = C; col--;)
                {
                    r[row][col] = this->m[row][col] - m.m[row][col];
                }

                return r;
            }


            inline Vec<S, C>& operator[] (size_t row)
            {
                return m[row];
            }


            inline bool operator== (Mat<S, R, C> M) const
            {
                return memcmp(this->m, M.m, sizeof(M.m)) == 0;
            }

            std::string to_string(Mat<S, R, C> const&) const
            {
                return this->to_string();
            }

            std::string to_string() const
            {
                std::string str = "";
                for (int j = 0; j < R; ++j)
                {
                    str += "|";
                    for (int i = 0; i < C; ++i)
                    {
                        str += std::to_string(m[j][i]);
                        if (i < C - 1) { str += ", "; }
                    } str += "|\n";
                }

                return str;
            }

            S distance(Mat<S, R, C> const& M) const
            {
                S sum = 0;

                for (int r = R; r--;)
                for (int c = C; c--;)
                {
                    auto d = M.m[r][c] - m[r][c];
                    sum += d * d;
                }

                return sqrt(sum);
            }

            void swap_rows(size_t row_i, size_t row_j)
            {
                Vec<S, C> temp_row;

                temp_row = m[row_i];
                m[row_i] = m[row_j];
                m[row_j] = temp_row;
            }

            Mat<S, R, C>& rref()
            {
                int piv_c = 0;

                // compute upper diagonal
                for (int r = 0; r < R; r++)
                {
                    // Check if the piv column of row r is zero. If it is, lets
                    // try to find a row below that has a non-zero column
                    if (m[r][piv_c] == 0)
                    {
                        int swap_ri = -1;
                        for (int ri = r + 1; ri < R; ri++)
                        {
                            if (m[ri][piv_c] != 0)
                            {
                                swap_ri = ri;
                                break;
                            }
                        }

                        if (swap_ri > -1) { swap_rows(swap_ri, r); }
                    }

                    { // next row, scale so leading coefficient is 1
                        float d = 1 / m[r][piv_c];

                        // scale row
                        for (int c = piv_c; c < C; c++) { m[r][c] *= d; }
                    }


                    for (int ri = 0; ri < R; ri++)
                    {
                        // skip zero elements and skip row r
                        if (m[ri][piv_c] == 0 || ri == r) { continue; }

                        float d = m[ri][piv_c];

                        // scale row then subtract the row above to zero out
                        // other elements in this column
                        for (int c = piv_c; c < C; c++)
                        {
                            m[ri][c] -= d * m[r][c];
                        }
                    }

                    ++piv_c;
                }

                return *this;
            }

            Mat<S, R, C * 2> augment()
            {
                const auto Mc = C * 2;
                Mat<S, R, Mc> M;
                S d = 0;

                for (int r = 0; r < R; ++r)
                {
                    // form the identity on the right hand side
                    M[r][r + C] = 1.0;

                    for (int c = 0; c < C; ++c)
                    {
                        M[r][c] = m[r][c];
                    }
                }

                return M;
            }

            Mat<S, R, C> inverse()
            {
                Mat<S, R, C> inv;
                auto _rref = augment().rref();

                for (int r = 0; r < R; r++)
                {
                    inv[r] = _rref[r].template slice<C>(C);
                }

                return inv;
            }

            Mat<S, C, R> transpose()
            {
                Mat<S, C, R> res;

                for (int r = 0; r < C; ++r)
                for (int c = 0; c < R; ++c)
                {
                    res[r][c] = m[c][r];
                }

                return res;
            }


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

            template <class RADIAL_TYPE>
            static Mat<S, 2, 2> rotation(RADIAL_TYPE angle)
            {
                S s = static_cast<S>(sin(angle));
                S c = static_cast<S>(cos(angle));

                return {
                    {  c,  s },
                    { -s,  c },
                };
            }

            template <class RADIAL_TYPE>
            static Mat<S, 4, 4> rotate_x(RADIAL_TYPE t)
            {
                auto c = static_cast<S>(cos(t));
                auto s = static_cast<S>(sin(t));

                return {
                    { 1, 0, 0, 0 },
                    { 0, c, s, 0 },
                    { 0,-s, c, 0 },
                    { 0, 0, 0, 1 }
                };
            }

            template <class RADIAL_TYPE>
            static Mat<S, 4, 4> rotate_y(RADIAL_TYPE t)
            {
                auto c = static_cast<S>(cos(t));
                auto s = static_cast<S>(sin(t));

                return {
                    { c, 0,-s, 0 },
                    { 0, 1, 0, 0 },
                    { s, 0, c, 0 },
                    { 0, 0, 0, 1 }
                };
            }

            template <class RADIAL_TYPE>
            static Mat<S, 4, 4> rotate_z(RADIAL_TYPE t)
            {
                auto c = static_cast<S>(cos(t));
                auto s = static_cast<S>(sin(t));

                return {
                    { c,-s, 0, 0 },
                    { s, c, 0, 0 },
                    { 0, 0, 1, 0 },
                    { 0, 0, 0, 1 }
                };
            }

            static Mat<S, 4, 4> perspective(float y_fov, float aspect, float n=0.1, float f=1000)
            {
                auto const a = 1.f / tan(y_fov / 2.f);

                return {
                    {  a / aspect,     0,                          0,      0 },
                    {           0,     a,                          0,      0 },
                    {           0,     0,       -((f + n) / (f - n)),     -1 },
                    {           0,     0, -((2.f * f * n) / (f - n)),      1 }
                };
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

            static Mat<S, 4, 4> translate(Vec<S, 3> v)
            {
                return {
                    { 1, 0, 0, v[0] },
                    { 0, 1, 0, v[1] },
                    { 0, 0, 1, v[2] },
                    { 0, 0, 0, 1    }
                };
            }

            template <size_t D, size_t N>
            static Mat<S, D, D> estimate_covariance(Mat<S, D, N> samples)
            {
                // covariance matrix
                Mat<S, D, D> Q;

                // compute the mean of the samples
                Vec<S, D> mu = {};
                for (int r = 0; r < D; r++)
                for (int c = 0; c < C; c++)
                {
                    mu[r] += samples[r][c];
                }
                mu /= static_cast<S>(N);

                // subtract the mean from the set of samples
                for (int r = 0; r < D; r++)
                for (int c = 0; c < C; c++)
                {
                    samples[r][c] -= mu[r];
                }

                // compute the matrix
                Q = samples * samples.transpose();
                Q /= (static_cast<S>(N) - 1);

                return Q;
            }

            Vec<S, C> m[R];
        };

        using Mat4f = Mat<float, 4, 4>;
        using Mat2f = Mat<float, 2, 2>;

        //     ___            _
        //    / _ \ _  _ __ _| |_
        //   | (_) | || / _` |  _|
        //    \__\_\\_,_\__,_|\__|
        //
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

            Quat(Vec<float, 4> v) : Vec(v)
            {
                // NOP
            }


            Quat operator*(Quat const& other) const
            {
                auto t3 = this->as_dimension<3>();
                auto o3 = other.as_dimension<3>();

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


            Quat operator*(float s) const
            {
                return { as_dimension<4>() * s };
            }


            Quat& operator*=(Quat const& other)
            {
                *this = *this * std::move(other);
                return *this;
            }

            Quat conjugate() const
            {
                auto& q = *this;
                return { -q[0], -q[1], -q[2], q[3] };
            }

            Quat inverse() const
            {
                auto inv = conjugate();
                auto mag2 = dot(*this);
                static_cast<Vec<float, 4>>(inv) /= mag2;
                return inv;
            }

            Quat slerp_to(Quat const& p1, float t) const
            {
                const auto& p0 = *this;
                auto W = acos(p0.dot(p1));
                auto sin_W = sin(W);
		if (sin_W < 0.0001f) { sin_W = 0.0001f; }
                return p0 * (sin((1 - t) * W) / sin_W) + p1 * (sin(t * W) / sin_W);
            }

            Vec<float, 3> rotate(Vec<float, 3> const& v) const
            {
                auto q_xyz = this->as_dimension<3>();

                auto t = Vec::cross(q_xyz, v);
                t *= 2;

                auto u = Vec::cross(q_xyz, t);
                t *= this->v[3];

                return v + t + u;
            }

            template <class S>
            inline Mat<S, 4, 4> to_matrix()
            {
                auto v = static_cast<Vec<S, 4>>(*this);
                auto a = v[3], b = v[0], c = v[1], d = v[2];
                auto a2 = a * a, b2 = b * b, c2 = c * c, d2 = d * d;

                return {
                    { a2 + b2 - c2 - d2, 2 * (b*c - a*d)  , 2 * (b*d + a*c)  , 0},
                    { 2 * (b*c + a*d)  , a2 - b2 + c2 - d2, 2 * (c*d - a*b)  , 0},
                    { 2 * (b*d - a*c)  , 2 * (c*d + a*b)  , a2 - b2 - c2 + d2, 0},
                    { 0                , 0                , 0                , 1},
                };
            }

            template <class S>
            Vec<S, 3> to_roll_pitch_yaw()
            {
                S roll, pitch, yaw;
                // roll (x-axis rotation)
                S sinr_cosp = +2.0 * (v[3] * v[0] + v[1] * v[2]);
                S cosr_cosp = +1.0 - 2.0 * (v[0] * v[0] + v[1] * v[1]);
                roll = atan2(sinr_cosp, cosr_cosp);

                // pitch (y-axis rotation)
                S sinp = +2.0 * (v[3] * v[1] - v[2] * v[0]);
                if (fabs(sinp) >= 1)
                {
                    pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
                }
                else
                {
                    pitch = asin(sinp);
                }

                // yaw (z-axis rotation)
                S siny_cosp = +2.0 * (v[3] * v[2] + v[0] * v[1]);
                S cosy_cosp = +1.0 - 2.0 * (v[1] * v[1] + v[2] * v[2]);
                yaw = atan2(siny_cosp, cosy_cosp);

                return { roll, pitch, yaw };
            }

            static Quat from_axis_angle(Vec<float, 3> axis, float angle)
            {
                auto a_2 = angle / 2;
                auto a = sinf(a_2);

                axis *= a;

                return { axis[0], axis[1], axis[2], cosf(a_2) };
            }
        };

        //----------------------------------------------------------------------
        //    ___          _
        //   | _ ) __ _ __(_)___
        //   | _ \/ _` (_-< (_-<
        //   |___/\__,_/__/_/__/
        //
        template<typename S, size_t D>
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


            inline Basis<S, 3> rotate(Quat const& q)
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
