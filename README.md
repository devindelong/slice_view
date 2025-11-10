# dd::slice_view

A modern **C++23 Ranges view** that provides slicing over arbitrary range types — similar
in spirit to Python’s slicing or a combination of `std::views::drop` and
`std::views::take`, but implemented as a single unified view.

This project was developed as an exploration of **iterator caching optimization**,
**conditional member storage**, and **safe caching semantics** for range adaptors.

---

## 🧩 Overview

`dd::slice_view` allows creating lightweight, composable subviews of ranges:

```cpp
#include <vector>
#include <iostream>
#include "dd/slice_view.hpp"

auto v = std::vector{1, 2, 3, 4, 5, 6};
auto sliced = dd::slice_view(v, 2, 5);

for (int x : sliced)
  std::cout << x << ' '; // prints: 3 4 5
```
