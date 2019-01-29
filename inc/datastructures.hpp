#pragma once

#include "exo.hpp"

namespace exo
{
    namespace ds
    {

        /**
         * @brief Simple, very low overhead datastructure for storing unordered data.
         * @tparam T Type whose instances will be stored in the BoundedList.
         * @tparam CAP Maximum number of elements that may be stored in the list.
         */
        template<typename T, size_t CAP>
        struct BoundedList {
            BoundedList()
            {
                _size = 0;
            }

            BoundedList(std::initializer_list<T> items)
            {
                _size = 0;
                for (auto item : items)
                {
                    push_back(item);
                }
            }

            /**
             * @brief Append element to the end of the list.
             * @param e Element to add to the BoundedList
             * @return True if the BoundedList has enough space, False if there
             *         wasn't enough space.
             */
            bool push_back(T e)
            {
                if (_size >= CAP) { return false; }

                _list[_size++] = e;

                return true;
            }

            /**
             * @brief Remove the last element from the list.
             * @param e Set to the value of the element removed.
             * @return False if the list is empty, True otherwise.
             */
            bool pop_back(T& e)
            {
                if (_size <= 0) { return false; }

                e = _list[--_size];

                return true;
            }

            /**
             * @brief Remove the last element from the list.
             * @return False if the list is empty, True otherwise.
             */
            bool pop_back()
            {
                if (_size <= 0) { return false; }

                --_size;

                return true;
            }

            /**
             * @brief Get the last element of the list.
             * @param e Set to the value of the last element.
             * @return False if the list is empty, True otherwise.
             */
            bool peek_back(T& e)
            {
                if (_size <= 0) { return false; }

                e = _list[_size - 1];

                return true;
            }

            /**
             * @brief Get the last element of the list.
             * @return Reference to the last element in the list.
             * @note Invokes undefined behavior for empty lists.
             */
            T& peek_back() { return _list[_size - 1]; }

            /**
             * @brief Removes an element at a specific index.
             * @param idx Index to element to remove from the list.
             * @return False if the list is empty, or idx is outside of the
             *         bounds True otherwise.
             */
            bool remove_at(size_t idx)
            {
                if (idx >= _size) { return false; }
                if (_size == 0) { return false; }

                _list[idx] = _list[_size - 1];
                _size--;

                return true;
            }

            /**
             * @brief Removes an element from the list.
             * @return True if a matching element is found in the list
             *         otherwise False.
             * @note In the event of duplicate entries, only the first occurance
             *       is removed.
             */
            bool remove(T e)
            {
                for (int i = 0; i < _size; i++)
                {
                    if (e == _list[i]) { return remove_at(i); }
                }

                return false;
            }

            /**
             * @return Number of elements contained by the list.
             */
            size_t size() { return _size; }


            inline T& operator[](int idx) { return _list[idx]; }
        private:
            T _list[CAP];
            size_t _size;
        };
    }
}
