#include "exo.hpp"
#include "datastructures.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the RingBuffer data structure implementation."

#include "test.h"
{
    (void)argc; (void)argv;

    {
        int rand_ints[] = { rand(), rand(), rand() };
        exo::ds::RingBuffer<int, 3> buf;

        // add 3 elements, all must succeed
        for (int i = 0; i < 3; ++i)
        {
            buf.push_back(rand_ints[i]);
        }

        EXPECT_EQ(buf.size(), 3);

        // ensure that every int is in the right place
        for (int i = 0; i < 3; ++i)
        {
            EXPECT_EQ(*buf[i], rand_ints[i]);
        }

        // remove all 3 elements
        for (int i = 0; i < 3; ++i)
        {
            assert(buf.pop_front() == rand_ints[i]);
        }

        // the buf should now be empty
        EXPECT_EQ(buf.size(), 0);

        // add the same 3 elements twice, first 3 should be overwritten
        for (int i = 0; i < 6; ++i)
        {
            buf.push_back(rand_ints[i % 3]);
        }

        // ensure that every int is in the right place
        for (int i = 0; i < 3; ++i)
        {
            assert(*buf[i] == rand_ints[i]);
        }
    }

    return 0;
}
