/**
 * @file ElementsKernel/_impl/Number.tpp
 * @brief implementation of the templates declared in ElementsKernel/Auxiliary.h
 * @date 2023-12-12
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

// IWYU pragma: private, include "ElementsKernel/Number.h"
#pragma once

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_NUMBER_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Number.h instead"
#else

#include <cmath>        // for round
#include <type_traits>  // for is_floating_point, is_integral

namespace Elements {

template <typename TargetType, typename SourceType>
TargetType numberCast(const SourceType& s) {

  using std::is_floating_point;
  using std::is_integral;
  using std::round;

  TargetType t = static_cast<TargetType>(s);

  if (is_floating_point<SourceType>::value && is_integral<TargetType>::value) {
    t = static_cast<TargetType>(round(s));
  }

  return t;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_NUMBER_IMPL_
