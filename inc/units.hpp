#pragma once

namespace exo
{

template <class TYPE, unsigned int NUM, unsigned int DEN>
struct Unit
{
	TYPE value;

	Unit(TYPE v) { value = v; }
	inline operator TYPE() const  { return value; }
	// inline TYPE operator*(Unit u) { return (value * u.value); }
	inline Unit operator*(Unit u) { return Unit(value * u.value); }
	inline Unit operator/(Unit u) { return Unit(value / u.value); }
	inline Unit operator+(Unit u) { return Unit(value + u.value); }
	inline Unit operator-(Unit u) { return Unit(value - u.value); }

	template<class OTHER_TYPE>
	inline TYPE as() { return static_cast<OTHER_TYPE>(value); }
};

template <class TYPE>
struct Meter : public Unit<TYPE, 1, 1>
{
	Meter(TYPE v) : Unit<TYPE, 1, 1>(v) { }
};

/*
template <class TYPE>
struct Centimeter : public Meter<TYPE>
{
	Meter(TYPE v) : Meter<TYPE>(v) { }
};
*/


}

