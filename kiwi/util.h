/*-----------------------------------------------------------------------------
| Copyright (c) 2013-2017, Nucleic Development Team.
|
| Distributed under the terms of the Modified BSD License.
|
| The full license is in the file LICENSE, distributed with this software.
|----------------------------------------------------------------------------*/
#pragma once

#include <type_traits>
#include <limits>

namespace kiwi
{

namespace impl
{

template <typename T, typename std::enable_if<std::is_floating_point<T>::value, bool>::type = true>
inline bool nearZero(T value)
{
    constexpr T eps = std::numeric_limits<T>::epsilon();
    return value < T{0} ? -value < eps : value < eps;
}

template <typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
inline bool nearZero(T value)
{
    return value == T{0};
}

} // namespace impl

} // namespace kiwi
