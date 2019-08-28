#include "exo.hpp"
#include "datastructures.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the BoundedList data structure implementation."

#include "test.h"
{
    (void)argc; (void)argv;

    {
        int rand_ints[] = { rand(), rand(), rand() };
        exo::ds::BoundedList<int, 3> list;

        // add 3 elements, all must succeed
        for (int i = 0; i < 3; ++i)
        {
            assert(list.push_back(rand_ints[i]) == true);
        }

        assert(list.size() == 3);

        // add one more, this should fail
        assert(list.push_back(rand()) == false);

        // ensure that every int is in the right place
        for (int i = 0; i < 3; ++i)
        {
            assert(list[i] == rand_ints[i]);
        }

        // remove all 3 elements
        for (int i = 0; i < 3; ++i)
        {
            assert(list.remove(list[0]) == true);
        }

        // the list should now be empty
        assert(list.size() == 0);

        int rand_int = rand();
        int popped_int = 0;
        list.push_back(rand_int);
        assert(list.peek_back(popped_int) == true);
        assert(popped_int == rand_int);
        assert(list.pop_back(popped_int) == true);
        assert(popped_int == rand_int);
    }

    return 0;
}
