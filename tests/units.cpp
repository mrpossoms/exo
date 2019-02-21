#include "exo.hpp"
#include "units.hpp"

#define DESCRIPTION "Tests exo unit representation"

using namespace exo::units;

#include "test.h"
{
	M one_meter  = {1};
	M two_meters = {2};

	assert(NEAR(one_meter.in<Km>(), 0.001));
	assert(NEAR(one_meter.in<M>(), 1));
	assert(NEAR(one_meter.in<Dm>(), 10));
	assert(NEAR(one_meter.in<Cm>(), 100));
	assert(NEAR(one_meter.in<Mm>(), 1000));

	assert(NEAR(two_meters * two_meters, 4));
	assert(NEAR(two_meters + one_meter, 3));
	assert(NEAR(two_meters - one_meter, 1));
	assert(NEAR(two_meters / two_meters, 1));

	return 0;
}
