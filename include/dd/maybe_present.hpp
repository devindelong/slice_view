/**
 * @file maybe_present.hpp
 * @copyright Copyright (c) 2025, Devin DeLong. All rights reserved.
 *
 * @license This code is distributed under the BSD 3-Clause License.
 *          See the LICENSE file for the full text of the license.
 *
 * @author Devin DeLong
 */

#pragma once

#include <type_traits>

/**
 * @brief Creates a unique empty type.
 * @detail Relies on the fact that every lanbda is a unique type.
 */
#define DD_UNIQUE_EMPTY_TYPE ::dd::empty_type<decltype([]() {})>

namespace dd
{

/**
 * @brief An empty type.
 * @tparam T Type parameter used to generate unique empty types.
 */
template <typename T>
struct empty_type
{
  /**
   * @brief Variadiv constructor to construct from anything.
   * @details Since this might be used in conditional type expressions, where the
   * resulting type is either an empty type or a user-defined type, a variadic constructor
   * is needed so that both types can be constructed from the same arguments. The empty
   * type must be constructible from an unknown number of type arguments.
   */
  constexpr empty_type(auto&&...) {}
};

/**
 * @brief Conditionally keeps or "removes" a variable.
 * @detail Use this with [[no_unique_address]] to avoid storage space for empty types.
 * @tparam Present Indicates if the type evaluates to T or an empty type.
 * @tparam T The desired type if Present is true.
 * @tparam E The empty type if Present is false.
 * @note Be careful when creating an alias to this type. If an alias is created, the empty
 * type will not be unique between instances of each alias and [[no_unique_address]] will
 * result in an extra added byte for each instance of the same type.
 */
template <bool Present, typename T, typename E = DD_UNIQUE_EMPTY_TYPE>
using maybe_present_t = std::conditional_t<Present, T, E>;

} // namespace dd
