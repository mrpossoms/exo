#pragma once

#include "linalg.hpp"

namespace exo
{
namespace math
{

template <class T>
struct pid
{
	pid() = default;

	pid(T p, T i, T d) : k_p(p), k_i(i), k_d(d)
	{
		// no-op
	}

	inline T p(T e) { return k_p * e; }

	inline T i(T e, T dt)
	{
		int_i += e * dt;
		return k_i * int_i;
	}

	inline T d(T e, T dt)
	{
		auto _d = (e - e_t_1) / dt;
		d_filtered = _d * (1 - filter_power) + d_filtered * filter_power;
		return k_d * d_filtered;
	}

	inline T step(T e, T dt)
	{
		auto u = p(e) + i(e, dt) + d(e, dt);
		e_t_1 = e;
		return u; 
	}

	T k_p = {}, k_i = {}, k_d = {}; // tuning parameters

	T filter_power = {};
private:
	T int_i = {};
	T e_t_1 = {};
	T d_filtered = {};
};

} // math

} // exo
