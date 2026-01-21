/**
 * @file ElementsKernel/_impl/Module.tpp
 * @brief implementation of the templates declared in ElementsKernel/Module.h
 * @date 2023-12-13
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

// IWYU pragma: private, include "ElementsKernel/Module.h"
#pragma once

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Module.h instead"
#else

#include <cstdint>  // for uint_least64_t
#include <ostream>  // for operator<<, basic_ostream, endl, ostream
#include <string>   // for char_traits, operator<<, string

#include "ThisElementsModule.h"  // for THIS_MODULE_MAJOR_VERSION, ...

namespace Elements {
struct Module;
}

namespace Elements {

inline std::string Module::name() {
  return THIS_MODULE_NAME_STRING;
}

inline std::string Module::versionString() {
  return THIS_MODULE_VERSION_STRING;
}
inline std::uint_least64_t Module::version() {
  return THIS_MODULE_VERSION;
}
inline std::uint_least64_t Module::majorVersion() {
  return THIS_MODULE_MAJOR_VERSION;
}
inline std::uint_least64_t Module::minorVersion() {
  return THIS_MODULE_MINOR_VERSION;
}

inline std::uint_least64_t Module::patchVersion() {
  return THIS_MODULE_PATCH_VERSION;
}

inline std::ostream& operator<<(std::ostream& stream, const Module& m) {

  stream << "Name: " << m.name() << std::endl;
  stream << "Version String: " << m.versionString() << std::endl;
  stream << "Version: " << m.version() << std::endl;
  stream << "Major Version: " << m.majorVersion() << std::endl;
  stream << "Minor Version: " << m.minorVersion() << std::endl;
  stream << "Patch Version: " << m.patchVersion() << std::endl;

  return stream;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_MODULE_IMPL_
