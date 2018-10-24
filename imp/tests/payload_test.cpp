#include "exo.hpp"

#define DESCRIPTION "Tests packing and unpacking with the Payload class"

struct SomeMsg
{
	char name[16];
	double v[3];
	float mass;
};

#include "test.h"
{

	auto p = exo::msg::Payload<sizeof(SomeMsg) + sizeof(uint32_t)>{};
	auto msg_out = SomeMsg{
		"Planet Earth",
		{ 1, 33, 7 },
		10e6
	};
	uint32_t uint_out = 1337;

	p.put<SomeMsg>(msg_out);
	p << uint_out;
	p.seek_set(0);

	SomeMsg msg_in;
	uint32_t uint_in = 0;
	p.get<SomeMsg>(msg_in);
	p >> uint_in;

	int res = memcmp(&msg_out, &msg_in, sizeof(SomeMsg));
	res += uint_in != uint_out;

	return res;
}