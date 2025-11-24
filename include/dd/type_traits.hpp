/**
 * @file type_traits.hpp
 * @copyright Copyright (c) 2025, Devin DeLong. All rights reserved.
 *
 * @license This code is distributed under the BSD 3-Clause License.
 *          See the LICENSE file for the full text of the license.
 *
 * @author Devin DeLong
 */

#pragma once

#include <type_traits>

namespace dd
{

// apply_const

template <typename From, typename To>
struct apply_const
    : std::type_identity<
        std::conditional_t<std::is_const_v<From>, std::add_const_t<To>, To>>
{
};

template <typename From, typename To>
using apply_const_t = apply_const<From, To>::type;

// copy_const

template <typename From, typename To>
struct copy_const : apply_const<From, std::remove_const_t<To>>
{
};

template <typename From, typename To>
using copy_const_t = copy_const<From, To>::type;

// apply_volatile

template <typename From, typename To>
struct apply_volatile
    : std::type_identity<
        std::conditional_t<std::is_volatile_v<From>, std::add_volatile_t<To>, To>>
{
};

template <typename From, typename To>
using apply_volatile_t = apply_volatile<From, To>::type;

// copy_volatile

template <typename From, typename To>
struct copy_volatile : apply_volatile<From, std::remove_volatile_t<To>>
{
};

template <typename From, typename To>
using copy_volatile_t = copy_volatile<From, To>::type;

// apply_cv

template <typename From, typename To>
struct apply_cv : std::type_identity<apply_const_t<From, apply_volatile_t<From, To>>>
{
};

template <typename From, typename To>
using apply_cv_t = apply_cv<From, To>::type;

// copy_cv

template <typename From, typename To>
struct copy_cv : std::type_identity<copy_const_t<From, copy_volatile_t<From, To>>>
{
};

template <typename From, typename To>
using copy_cv_t = copy_cv<From, To>::type;

// copy_reference

template <typename From, typename To>
class copy_reference
{
  static constexpr bool is_lvalue = std::is_lvalue_reference_v<From>;
  static constexpr bool is_rvalue = std::is_rvalue_reference_v<From>;
  using base_to = std::remove_reference_t<To>;

public:
  using type = std::conditional_t<
    is_lvalue,
    std::add_lvalue_reference_t<base_to>,
    std::conditional_t<is_rvalue, std::add_rvalue_reference_t<base_to>, base_to>>;
};

template <typename From, typename To>
using copy_reference_t = copy_reference<From, To>::type;

// copy_cvref_t (same as like_t)

template <typename From, typename To>
struct copy_cvref
    : std::type_identity<copy_reference_t<
        From,
        copy_cv_t<std::remove_reference_t<From>, std::remove_reference_t<To>>>>
{
};

template <typename From, typename To>
using copy_cvref_t = copy_cvref<From, To>::type;

// like_t (same as remove_cvref_t)

template <typename From, typename To>
struct like : std::type_identity<copy_reference_t<
                From,
                copy_cv_t<std::remove_reference_t<From>, std::remove_reference_t<To>>>>
{
};

template <typename From, typename To>
using like_t = like<From, To>::type;

} // namespace dd
