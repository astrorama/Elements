/**
 * @file ElementsKernel/Module.h
 * @brief Defines tools to describe the current Elmeents module
 * @date May 14, 2020
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
 *
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_

#include <algorithm>  // for for_each
#include <cstdint>    // for uint_least64_t
#include <iostream>   // for ostream
#include <string>     // for string
#include <vector>     // for vector

#include "ThisElementsModule.h"  // local module constants generated
                                 // by the build system

namespace Elements {

struct Module {
  static std::string name();

  static std::string versionString();

  static std::uint_least64_t version();

  static std::uint_least64_t majorVersion();

  static std::uint_least64_t minorVersion();

  static std::uint_least64_t patchVersion();
};

std::ostream& operator<<(std::ostream& stream, const Module& m);

}  // namespace Elements

#define ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_IMPL_
#include "ElementsKernel/_impl/Module.tpp"  // IWYU pragma: export
#undef ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_IMPL_

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_H_

/**@}*/
