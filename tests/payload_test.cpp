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
	(void)argc; (void)argv;

	auto p = exo::msg::Payload<sizeof(SomeMsg)>{};
	auto msg_out = SomeMsg{
		"Planet Earth",
		{ 1, 33, 7 },
		10e6
	};

	p.put<SomeMsg>(msg_out);
	p.seek_set(0);

	SomeMsg msg_in;
	p.get<SomeMsg>(msg_in);

	int res = memcmp(&msg_out, &msg_in, sizeof(SomeMsg));

	return res;
}
