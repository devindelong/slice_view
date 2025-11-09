/**
 * @file basic.cpp
 * @copyright Copyright (c) 2025, Devin DeLong. All rights reserved.
 *
 * @license This code is distributed under the BSD 3-Clause License.
 *          See the LICENSE file for the full text of the license.
 *
 * @author Devin DeLong
 */

#include "dd/slice_view.hpp"

#include <iostream>
#include <list>
#include <print>
#include <ranges>
#include <vector>

int main()
{
  auto vec = std::vector{10, 20, 30, 40, 50, 60};

  // Use with l-value container.
  const auto slice1 = dd::ranges::slice_view(vec, 2, 5);
  std::println("sizeof non-cached slice_view: {}", sizeof(slice1));
  std::println("size: {}", slice1.size());
  for (const auto x : slice1)
  {
    std::print("{} ", x);
  }
  std::println();

  // Use with r-value generator
  auto slice2 = dd::ranges::slice_view(std::views::iota(0, 10), 2, 5);
  for (auto x : slice2)
  {
    std::print("{} ", x);
  }
  std::println();

  // Use the range adapter with pipe operator
  for (auto x : vec | dd::views::slice(2, 5))
  {
    std::print("{} ", x);
  }
  std::println();

  // Use an r-value container.
  for (auto x : dd::ranges::slice_view(std::vector{10, 20, 30, 40, 50, 60}, 2, 5))
  {
    std::print("{} ", x);
  }
  std::println();

  // Use a generating view
  for (auto x : std::views::iota(50, 60) | dd::views::slice(2, 5))
  {
    std::print("{} ", x);
  }
  std::println();

  // list should use cached iterators.
  std::list<int> lst = {10, 20, 30, 40, 50};
  const auto slice_cached = dd::ranges::slice_view(lst, 2, 5);
  std::cout << "\nsizeof cached slice_view: " << sizeof(slice_cached) << "\n";
  std::println("size: {}", slice1.size());

  return 0;
}
