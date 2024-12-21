/**
 * @file ElementsKernel/_impl/FuncPtrCast.tpp
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

// IWYU pragma: private, include "ElementsKernel/FuncPtrCast.h"

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_IMPL_
#error "This file should not be included directly! Use ElementsKernel/FuncPtrCast.h instead"
#else

#include <cstring>      // for memcpy
#include <type_traits>  // for is_pointer

namespace Elements {

namespace System {

template <typename DESTPTR, typename SRC>
constexpr DESTPTR FuncPtrCast(SRC* const src_p) noexcept {
  static_assert(std::is_pointer_v<DESTPTR>, "must be a pointer");
  DESTPTR dst_p = nullptr;  // must initialize to be a valid constexpr...
  std::memcpy(&dst_p, &src_p, sizeof(dst_p));
  return dst_p;
}

}  // namespace System

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_IMPL_
