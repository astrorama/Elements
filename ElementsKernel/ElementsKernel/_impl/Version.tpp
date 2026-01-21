/**
 * @file ElementsKernel/_impl/Version.tpp
 * @brief implementation of the templates declared in ElementsKernel/Version.h
 * @date 2023-12-11
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

// IWYU pragma: private, include "ElementsKernel/Version.h"
#pragma once

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_VERSION_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Version.h instead"
#else

#include <cstdint>  // for uint_least64_t

constexpr std::uint_least64_t CALC_PROJECT_VERSION(const std::uint_least64_t maj, const std::uint_least64_t min,
                                                   const std::uint_least64_t pat) {
  return (((maj) << 32) + ((min) << 16) + (pat));
}

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_VERSION_IMPL_
