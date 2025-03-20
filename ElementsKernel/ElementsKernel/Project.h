/**
 * @file ElementsKernel/Project.h
 * @brief Defines tools to describe the current project
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_

#include <algorithm>  // for for_each
#include <cstdint>    // for uint_least64_t
#include <iostream>   // for ostream
#include <string>     // for string
#include <vector>     // for vector

#include "ThisProject.h"  // local project constants generated
                          // by the build system

namespace Elements {

/**
 * @class Project
 * @brief Represents a project with basic attributes and functionality.
 *
 * The Project class is designed to encapsulate details about a project,
 * allowing for management and retrieval of its properties. This class
 * provides mechanisms to set and get project attributes such as name,
 * description, status, and other related attributes.
 *
 * This class can be extended to include more specialized project
 * management functionalities.
 *
 * Potential enhancements may include integration with collaborators,
 * advanced reporting, and dependency management between projects.
 */
struct Project {

  static std::string name();

  static std::string versionString();

  static std::uint_least64_t version();

  static std::uint_least64_t majorVersion();

  static std::uint_least64_t minorVersion();

  static std::uint_least64_t patchVersion();

  static std::string originalVersion();

  static std::string vcsVersion();

  static std::string installLocation();

  static bool useSoVersion();

  static std::string installPrefix();

  static std::vector<std::string> searchDirectories();
};

std::ostream& operator<<(std::ostream& stream, const Project& /* p */);

}  // namespace Elements

#define ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_IMPL_
#include "ElementsKernel/_impl/Project.tpp"  // IWYU pragma: export
#undef ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_IMPL_

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_H_

/**@}*/
