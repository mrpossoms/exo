#pragma once

namespace exo
{

namespace units
{

template <class TYPE, unsigned int NUM, unsigned int DEN>
struct Unit
{
	TYPE value;

	Unit() { value = 0; }
	Unit(TYPE v) { value = v; }
	inline operator TYPE() const  { return value; }

	inline Unit operator*(Unit u) { return Unit(value * u.value); }
	inline Unit operator/(Unit u) { return Unit(value / u.value); }
	inline Unit operator+(Unit u) { return Unit(value + u.value); }
	inline Unit operator-(Unit u) { return Unit(value - u.value); }

	inline Unit& operator*=(Unit u) { value *= u.value; return *this; }
	inline Unit& operator/=(Unit u) { value /= u.value; return *this; }
	inline Unit& operator+=(Unit u) { value += u.value; return *this; }
	inline Unit& operator-=(Unit u) { value -= u.value; return *this; }

	inline Unit& operator=(Unit u) { value = u.value; return *this; }

	static inline int numerator()   { return NUM; }
	static inline int denominator() { return DEN; }

	template<class OTHER_TYPE>
	inline TYPE cast() { return static_cast<OTHER_TYPE>(value); }

	template<typename OU>
	inline OU in()
	{
		auto base_units = value * numerator() / (TYPE)denominator();
		auto v = base_units * OU::denominator() / (TYPE)OU::numerator();
		return { v };
	}
};

//------------------------------------------------------------------------------
//    _                  _   _
//   | |   ___ _ _  __ _| |_| |_
//   | |__/ -_) ' \/ _` |  _| ' \
//   |____\___|_||_\__, |\__|_||_|
//                 |___/
template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Meter : public Unit<TYPE, NUM, DEN>
{
	Meter() = default;
	Meter(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
	inline Meter operator*(Meter u) { return Meter(this->value * u.value); }
	inline Meter operator/(Meter u) { return Meter(this->value / u.value); }
	inline Meter operator+(Meter u) { return Meter(this->value + u.value); }
	inline Meter operator-(Meter u) { return Meter(this->value - u.value); }
};

template <class TYPE>
struct Millimeter : public Meter<TYPE, 1, 1000> { Millimeter(TYPE v) : Meter<TYPE, 1, 1000>(v) { } };

template <class TYPE>
struct Centimeter : public Meter<TYPE, 1, 100> { Centimeter(TYPE v) : Meter<TYPE, 1, 100>(v) { } };

template <class TYPE>
struct Decimeter : public Meter<TYPE, 1, 10> { Decimeter(TYPE v) : Meter<TYPE, 1, 10>(v) { } };

template <class TYPE>
struct Kilometer : public Meter<TYPE, 1000, 1> { Kilometer(TYPE v) : Meter<TYPE, 1000, 1>(v) { } };

inline static Kilometer<float>  operator"" _km(long double v) { return { static_cast<float>(v) }; }
inline static Meter<float>      operator"" _m(long double v)  { return { static_cast<float>(v) }; }
inline static Decimeter<float>  operator"" _dm(long double v) { return { static_cast<float>(v) }; }
inline static Centimeter<float> operator"" _cm(long double v) { return { static_cast<float>(v) }; }
inline static Millimeter<float> operator"" _mm(long double v) { return { static_cast<float>(v) }; }

using M = Meter<float>;
using Mm = Millimeter<float>;
using Cm = Centimeter<float>;
using Dm = Decimeter<float>;
using Km = Kilometer<float>;

//------------------------------------------------------------------------------
//      _             _
//     /_\  _ _  __ _| |___
//    / _ \| ' \/ _` | / -_)
//   /_/ \_\_||_\__, |_\___|
//              |___/
template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Degree : public Unit<TYPE, NUM, DEN>
{
	Degree() = default;
	Degree(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
};

template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Radian : public Unit<TYPE, NUM, DEN>
{
	Radian() = default;
	Radian(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }

	static inline Radian<TYPE> from(Degree<TYPE> d) { return static_cast<TYPE>(d.value * M_PI / 180.0); }
};

//------------------------------------------------------------------------------
//    _____ _
//   |_   _(_)_ __  ___
//     | | | | '  \/ -_)
//     |_| |_|_|_|_\___|
//
template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Second : public Unit<TYPE, NUM, DEN>
{
	Second() = default;
	Second(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
	inline Second operator*(Second u) { return Second(this->value * u.value); }
	inline Second operator/(Second u) { return Second(this->value / u.value); }
	inline Second operator+(Second u) { return Second(this->value + u.value); }
	inline Second operator-(Second u) { return Second(this->value - u.value); }
};

//------------------------------------------------------------------------------
//    __  __
//   |  \/  |__ _ ______
//   | |\/| / _` (_-<_-<
//   |_|  |_\__,_/__/__/
//
template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Gram : public Unit<TYPE, NUM, DEN>
{
	Gram() = default;
	Gram(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
	inline Gram operator*(Gram u) { return Gram(this->value * u.value); }
	inline Gram operator/(Gram u) { return Gram(this->value / u.value); }
	inline Gram operator+(Gram u) { return Gram(this->value + u.value); }
	inline Gram operator-(Gram u) { return Gram(this->value - u.value); }
};

//------------------------------------------------------------------------------
//    __  __                    _   _
//   |  \/  |__ _ __ _ _ _  ___| |_(_)____ __
//   | |\/| / _` / _` | ' \/ -_)  _| (_-< '  \
//   |_|  |_\__,_\__, |_||_\___|\__|_/__/_|_|_|
//               |___/
template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Tesla : public Unit<TYPE, NUM, DEN>
{
	Tesla() = default;
	Tesla(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
	inline Tesla operator*(Tesla u) { return Tesla(this->value * u.value); }
	inline Tesla operator/(Tesla u) { return Tesla(this->value / u.value); }
	inline Tesla operator+(Tesla u) { return Tesla(this->value + u.value); }
	inline Tesla operator-(Tesla u) { return Tesla(this->value - u.value); }
};

}

}

