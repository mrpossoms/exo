#pragma once

#include "exo.hpp"

namespace exo
{
    namespace ds
    {
        template<typename T, size_t CAP>
        struct BoundedList {
            bool push_back(T e)
            {
                if (_size >= CAP) { return false; }

                _list[_size++] = e;

                return true;
            }


            bool pop_back(T& e)
            {
                if (_size <= 0) { return false; }

                e = _list[--_size];

                return true;
            }


            bool pop_back()
            {
                if (_size <= 0) { return false; }

                --_size;

                return true;
            }


            bool peek_back(T& e)
            {
                if (_size <= 0) { return false; }

                e = _list[_size - 1];

                return true;
            }


            bool remove_at(size_t idx)
            {
                if (idx >= _size) { return false; }
                if (_size == 0) { return false; }

                _list[idx] = _list[_size - 1];
                _size--;

                return true;
            }


            bool remove(T e)
            {
                for (int i = 0; i < _size; i++)
                {
                    if (e == _list[i]) { return remove_at(i); }
                }

                return false;
            }


            size_t size() { return _size; }


            inline T& operator[](int idx) { return _list[idx]; }
        private:
            T _list[CAP];
            size_t _size;
        };
    }
}
