/**
 * @file ElementsKernel/_impl/Project.tpp
 * @brief implementation of the templates declared in ElementsKernel/Project.h
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

// IWYU pragma: private, include "ElementsKernel/Project.h"

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Project.h instead"
#else

#include <cstdint>  // for uint_least64_t
#include <ostream>  // for operator<<, basic_ostream, endl, ostream
#include <string>   // for char_traits, operator<<, basic_string, string
#include <vector>   // for vector

#include "ThisProject.h"  // for CMAKE_INSTALL_PREFIX_STRING, ...

namespace Elements {
struct Project;
}

namespace Elements {

inline std::string Project::name() {
  return THIS_PROJECT_NAME_STRING;
}

inline std::string Project::versionString() {
  return THIS_PROJECT_VERSION_STRING;
}

inline std::uint_least64_t Project::version() {
  return THIS_PROJECT_VERSION;
}

inline std::uint_least64_t Project::majorVersion() {
  return THIS_PROJECT_MAJOR_VERSION;
}

inline std::uint_least64_t Project::minorVersion() {
  return THIS_PROJECT_MINOR_VERSION;
}

inline std::uint_least64_t Project::patchVersion() {
  return THIS_PROJECT_PATCH_VERSION;
}

inline std::string Project::originalVersion() {
  return THIS_PROJECT_ORIGINAL_VERSION;
}

inline std::string Project::vcsVersion() {
  return THIS_PROJECT_VCS_VERSION;
}

inline std::string Project::installLocation() {
  return THIS_PROJECT_INSTALL_LOCATION_STRING;
}

inline bool Project::useSoVersion() {
  return THIS_PROJECT_USE_SOVERSION;
}

inline std::string Project::installPrefix() {
  return CMAKE_INSTALL_PREFIX_STRING;
}

inline std::vector<std::string> Project::searchDirectories() {
  return THIS_PROJECT_SEARCH_DIRS;
}

inline std::ostream& operator<<(std::ostream& stream, const Project& /* p */) {
  stream << "Name: " << Elements::Project::name() << std::endl;
  stream << "Version String: " << Elements::Project::versionString() << std::endl;
  stream << "Version: " << Elements::Project::version() << std::endl;
  stream << "Major Version: " << Elements::Project::majorVersion() << std::endl;
  stream << "Minor Version: " << Elements::Project::minorVersion() << std::endl;
  stream << "Patch Version: " << Elements::Project::patchVersion() << std::endl;
  stream << "Original Version: " << Elements::Project::originalVersion() << std::endl;
  stream << "VCS Version: " << Elements::Project::vcsVersion() << std::endl;
  stream << "Install Location: " << Elements::Project::installLocation() << std::endl;
  stream << "Use So Version: " << Elements::Project::useSoVersion() << std::endl;
  stream << "Install Prefix: " << Elements::Project::installPrefix() << std::endl;
  stream << "Search Directories: ";

  for (const auto& d : Project::searchDirectories()) {
    stream << d << " ";
  }

  return stream;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROJECT_IMPL_
