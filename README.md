# cpp-circular-list

A barebones, header-only C++ implementation of a container `circular_list` with STL-like iterator support. This container is suitable for performance non-critical situations in which a circular linked list data structure simplifies application logic. For example, using the provided iterators, delicate indexing required to handle all cases of inserting or erasing sequences of elements in `std::vector` or `std::forward_list` can be avoided.

This code is provided as-is, without guarantees of correctness or efficiency. In particular, no effort has been made to optimize for performance.

## License - Public domain

This code is dedicated to the public domain under the [Creative Commons CC0 license](https://creativecommons.org/publicdomain/zero/1.0/). You can copy, modify, distribute and perform the work, even for commercial purposes, all without asking permission.

Full legal text is available in the included file COPYING.txt or at [https://creativecommons.org/publicdomain/zero/1.0/legalcode](https://creativecommons.org/publicdomain/zero/1.0/legalcode).

## Circular iterators

Iterators over `circular_list` provide a "multi-sheeted" view of the list: each iterator tracks the current position in the list along with a sheet index k, indicating the "winding number" of the iterator. The sheets are related by crossing the boundary between first/last element in the canonical order. Advancing an iterator in the kth sheet just past the last element produces an iterator pointing to the first element in the (k+1)th sheet. Likewise decrementing an iterator in the kth sheet to just before the first element produces an iterator pointing to the last element in the (k-1)th sheet.

In this language, `circular_list::begin()` produces an iterator pointing to the first element of the list in the 0th sheet. On the other hand, `circular_list::end()` points to the first element on the 1st sheet. The range from `circular_list::begin()` (inclusive) to `circular_list::end()` (exclusive) then includes each element of the list once, as desired. Multiple traversals over the circular list can also be expressed this way, by simply advancing an iterator by more than the size of the list.

