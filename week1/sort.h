// Sorting algorithms.

#include <algorithm>
#include <iterator>
#include <sstream>
#include <cstdio>

#pragma once

namespace rtl {
    
template <typename Iter, typename Comp>
void mergesort(Iter first, Iter last, Comp comp)
{
    // 0 or 1 elements is already sorted.
    if ((first + 1) >= last)
        return;

    Iter middle = (first + (last - first) / 2);

    // Sort the left & right sides.
    mergesort(first, middle, comp);
    mergesort(middle, last, comp);

    vector<typename std::iterator_traits<Iter>::value_type> temp;
    temp.reserve(last - first);

    Iter leftIter = first;
    Iter leftEnd = middle;
    Iter rightIter = middle;
    Iter rightEnd = last;

    while (leftIter != leftEnd || rightIter != rightEnd) {

        if (leftIter == leftEnd) {
            // Left list is exhausted.
            temp.push_back(*(rightIter++));
        } else if (rightIter == rightEnd) {
            // Right list is exhausted.
            temp.push_back(*(leftIter++));
        } else if (comp(*leftIter, *rightIter)) {
            temp.push_back(*(leftIter++));
        } else {
            temp.push_back(*(rightIter++));
        }
    }

    // Insert sorted list.
    for (Iter insertIter = first, tempIter = temp.begin(); insertIter != last; ++insertIter, ++tempIter)
        std::swap(*insertIter, *tempIter);
}

template <typename Iter, typename Comp>
void quicksort(Iter first, Iter last, Comp comp)
{
    // 0 or 1 elements is already sorted.
    if ((first + 1) >= last)
        return;

    Iter pivot = (first + (last - first) / 2);

    // Move elements according to pivot.
    Iter insertLeft = first;
    Iter insertRight = last - 1;

    while (insertLeft != insertRight) {
        if (comp(*insertLeft, *pivot)) {
            // Left side is on the correct side
            ++insertLeft;
            continue;
        }

        if (comp(*pivot, *insertRight)) {
            // Right side is on the correct side
            --insertRight;
            continue;
        }

        // Both items are on wrong side, fix it.
        std::swap(*insertLeft, *insertRight);

        // Follow the pivot value, if it just moved.
        if (pivot == insertLeft)
            pivot = insertRight;
        else if (pivot == insertRight)
            pivot = insertLeft;

        ++insertLeft;
    }

    // Recurse
    quicksort(first, pivot, comp);
    quicksort(pivot, last, comp);
}

}  // namespace rtl
