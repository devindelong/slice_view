/**
 * @file slice_view.hpp
 * @copyright Copyright (c) 2025, Devin DeLong. All rights reserved.
 *
 * @license This code is distributed under the BSD 3-Clause License.
 *          See the LICENSE file for the full text of the license.
 *
 * @author Devin DeLong
 */

#pragma once

#include "dd/maybe_present.hpp"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <optional>
#include <ranges>
#include <type_traits>

namespace dd::ranges
{

namespace detail
{
template <typename T>
concept const_copyable_or_movable =
  (std::is_const_v<std::remove_reference_t<T>> && std::copy_constructible<T>) ||
  std::move_constructible<T>;
} // namespace detail

/**
 * @brief A ranges slice view.
 * @details Implements a slice view that uses integral indices for the beginning and
 * ending of a range.
 */
template <std::ranges::viewable_range R>
class slice_view : public std::ranges::view_interface<slice_view<R>>
{
  // Indicates if cached iterators should be used.
  // Cached iterators are used for ranges where std::ranges::next is not constant time.
  static constexpr bool use_cached_iterators_ =
    std::ranges::forward_range<R> &&
    !(std::ranges::random_access_range<R> && std::ranges::sized_range<R>);

  // The cached iterator type.
  using cached_iterator = std::optional<std::ranges::iterator_t<R>>;

public:
  /**
   * @brief Alias for the ranegs difference type.
   */
  using difference_type = std::ranges::range_difference_t<R>;

  /**
   * @brief Defaulted constructor.
   */
  constexpr slice_view()
    requires std::default_initializable<R>
  = default;

  /**
   * @brief Constructor.
   * @param start Starting index of the slice.
   * @param end Ending index of the slice.
   */
  constexpr slice_view(R base, difference_type start, difference_type end)
      : base_{std::move(base)}, start_index_{start}, end_index_{end}
  {
    assert(start >= 0);
    assert(end >= 0);
    assert(end >= start);
  }

  /**
   * @brief Gets the underlying view.
   * @param self Explicit object parameter (deducing this)
   * @return A copy-constructed or move-constructed instance of the underlying view.
   */
  [[nodiscard]] constexpr auto base(this auto&& self) -> R
    requires detail::const_copyable_or_movable<decltype(self.base_)>
  {
    return std::forward_like<decltype(self)>(self.base_);
  }

  /**
   * @brief Gets an iterator to the beginning of the slice view.
   * @param self Explicit object parameter (deducing this)
   * @return Iterator to the beginning of the slice view.
   */
  [[nodiscard]] constexpr auto begin(this auto&& self)
    requires std::ranges::range<decltype(self.base_)>
  {
    return self.next(self.begin_, self.start_index_);
  }

  /**
   * @brief Gets an iterator to the beginning of the slice view.
   * @param self Explicit object parameter (deducing this)
   * @return Iterator to the beginning of the slice view.
   */
  [[nodiscard]] constexpr auto end(this auto&& self)
    requires std::ranges::range<decltype(self.base_)>
  {
    return self.next(self.end_, self.end_index_);
  }

  /**
   * @brief Gets the size of the sliced range.
   * @param self Explicit object parameter (deducing this)
   * @return Size of the range.
   */
  [[nodiscard]] constexpr auto size(this auto&& self)
    requires std::ranges::sized_range<decltype(self.base_)>
  {
    constexpr auto zero = difference_type{0};
    const auto base_size = std::ranges::ssize(self.base_);
    auto slice_size = std::max(
      zero, std::clamp(self.end_index_, zero, base_size) -
              std::clamp(self.start_index_, zero, base_size));
    return static_cast<std::make_unsigned_t<decltype(slice_size)>>(slice_size);
  }

private:
  /**
   * @brief Get the next iterator advanced by n elements.
   * @details The begin iterator from the base range will be advanced by n elements. If
   * caching is used, the iterator will also be cached and reused for repeated calls to
   * next.
   * @param iter A cached iterator or an empty type.
   * @param n number of elements to advance.
   * @return The resulting iterator advanced by n elements.
   * @note The argument cached_iter is a template because it may be either
   * cached_iterator or an empty type. The empty types need to be distinct empty types
   * in order for [[no_unique_address]] to work properly. This is the reason for the
   * template: the cached begin and end iterators are distinct empty types when
   * use_cached_iterators_ is false.
   */
  [[nodiscard]] constexpr auto next(auto& iter, difference_type n) const
  {
    if constexpr (use_cached_iterators_)
    {
      if (iter.has_value())
      {
        return iter.value();
      }
    }

    auto next_iter =
      std::ranges::next(std::ranges::begin(base_), n, std::ranges::end(base_));
    if constexpr (use_cached_iterators_)
    {
      iter.emplace(next_iter);
    }
    return next_iter;
  }

  R base_{};
  difference_type start_index_{0};
  difference_type end_index_{0};

  [[no_unique_address]]
  mutable maybe_present_t<use_cached_iterators_, cached_iterator> begin_;
  [[no_unique_address]]
  mutable maybe_present_t<use_cached_iterators_, cached_iterator> end_;
};

/**
 * @brief Deduction guide for slice_view.
 * @details Wraps the input range type in std::views::all_t.
 */
template <typename R>
slice_view(R&&, std::ranges::range_difference_t<R>, std::ranges::range_difference_t<R>)
  -> slice_view<std::views::all_t<R>>;

namespace views
{
namespace detail
{
template <typename R>
concept can_slice_view = requires {
  slice_view(
    std::declval<R>(), std::declval<std::ranges::range_difference_t<R>>(),
    std::declval<std::ranges::range_difference_t<R>>());
};

/**
 * @brief Range adaptor closure for @c slice_view.
 */
template <std::integral DifferenceType>
class slice_range_adaptor
    : public std::ranges::range_adaptor_closure<slice_range_adaptor<DifferenceType>>
{
public:
  /*
   * @brief Constructor.
   * @param start Starting index of the slice.
   * @param end Ending index of the slice.
   */
  constexpr slice_range_adaptor(DifferenceType start, DifferenceType end)
      : start_{start}, end_{end}
  {
  }

  /**
   * @brief Required operator for range_adaptor_closure.
   * @param range The range to slice.
   * @return A slice view.
   */
  template <std::ranges::viewable_range R>
  [[nodiscard]] constexpr auto operator()(R&& r) const
    requires detail::can_slice_view<R>
  {
    return slice_view(
      std::forward<R>(r), static_cast<std::ranges::range_difference_t<R>>(start_),
      static_cast<std::ranges::range_difference_t<R>>(end_));
  }

private:
  DifferenceType start_;
  DifferenceType end_;
};

struct slice_fn
{
  /**
   * @brief Constructs and returns a slice range adaptor.
   * @param start Starting index of the slice.
   * @param end Ending index of the slice.
   * @return A slice range adaptor object.
   */
  template <std::integral DifferenceType>
  [[nodiscard]] constexpr auto operator()(DifferenceType start, DifferenceType end) const
    -> slice_range_adaptor<DifferenceType>
  {
    return slice_range_adaptor{start, end};
  }
};
} // namespace detail

inline constexpr auto slice = detail::slice_fn{};

} // namespace views
} // namespace dd::ranges

namespace dd::views
{
using dd::ranges::views::slice;
}
