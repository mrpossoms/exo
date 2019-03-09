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

template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Meter : public Unit<TYPE, NUM, DEN>
{
	Meter() = default;
	Meter(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
	inline Meter operator*(Meter u) { return { *this * u }; }
	inline Meter operator/(Meter u) { return { *this / u }; }
	inline Meter operator+(Meter u) { return { *this + u }; }
	inline Meter operator-(Meter u) { return { *this - u }; }
};

template <class TYPE>
struct Millimeter : public Meter<TYPE, 1, 1000> { Millimeter(TYPE v) : Meter<TYPE, 1, 1000>(v) { } };

template <class TYPE>
struct Centimeter : public Meter<TYPE, 1, 100> { Centimeter(TYPE v) : Meter<TYPE, 1, 100>(v) { } };

template <class TYPE>
struct Decimeter : public Meter<TYPE, 1, 10> { Decimeter(TYPE v) : Meter<TYPE, 1, 10>(v) { } };

template <class TYPE>
struct Kilometer : public Meter<TYPE, 1000, 1> { Kilometer(TYPE v) : Meter<TYPE, 1000, 1>(v) { } };

Kilometer<float>  operator"" _km(long double v) { return { static_cast<float>(v) }; }
Meter<float>      operator"" _m(long double v)  { return { static_cast<float>(v) }; }
Decimeter<float>  operator"" _dm(long double v) { return { static_cast<float>(v) }; }
Centimeter<float> operator"" _cm(long double v) { return { static_cast<float>(v) }; }
Millimeter<float> operator"" _mm(long double v) { return { static_cast<float>(v) }; }

using M = Meter<float>;
using Mm = Millimeter<float>;
using Cm = Centimeter<float>;
using Dm = Decimeter<float>;
using Km = Kilometer<float>;


template <class TYPE, unsigned int NUM=1, unsigned int DEN=1>
struct Degree : public Unit<TYPE, NUM, DEN>
{
	Degree() = default;
	Degree(TYPE v) : Unit<TYPE, NUM, DEN>(v) { }
};

}

}

