/**
 * @file ElementsKernel/FuncPtrCast.h
 * @brief defines a Small helper function that allows the cast
 *   from void * to function pointer
 * @date 2014-12-01
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2023 Euclid Science Ground Segment
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_

#include <cstring>      // for memcpy
#include <type_traits>  // for is_pointer

namespace Elements {
namespace System {

/**
 * @brief Cast from void * to function pointer
 * @ingroup ElementsKernel
 * @details
 *   Small helper function that performs the cast from void * to function pointer
 *   in a standards compliant way. For more information on this type of 'type punning, see
 *   https://github.com/CppCon/CppCon2017/blob/master/Presentations/Type%20Punning%20In%20C%2B%2B17%20-%20Avoiding%20Pun-defined%20Behavior/Type%20Punning%20In%20C%2B%2B17%20-%20Avoiding%20Pun-defined%20Behavior%20-%20Scott%20Schurr%20-%20CppCon%202017.pdf
 *   and https://www.youtube.com/watch?v=sCjZuvtJd-k
 * @param src_p pointer address to convert
 * @tparam SRC source pointer type
 * @tparam DESTPTR destination pointer type
 * @return destination pointer
 */
template <typename DESTPTR, typename SRC>
constexpr DESTPTR FuncPtrCast(SRC* const src_p) noexcept {
  static_assert(std::is_pointer<DESTPTR>::value, "must be a pointer");
  DESTPTR dst_p = nullptr;  // must initialize to be a valid constexpr...
  std::memcpy(&dst_p, &src_p, sizeof(dst_p));
  return dst_p;
}

}  // namespace System
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_FUNCPTRCAST_H_

/**@}*/
