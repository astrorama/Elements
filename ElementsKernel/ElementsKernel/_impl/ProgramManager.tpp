/**
 * @file ElementsKernel/_impl/ProgramManager.tpp
 * @brief implementation of the templates declared in ElementsKernel/Auxiliary.h
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

// IWYU pragma: private, include "ElementsKernel/ProgramManager.h"

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAM_MANAGER_IMPL_
#error "This file should not be included directly! Use ElementsKernel/ProgramManager.h instead"
#else

#include <boost/filesystem.hpp>  // for exists, operator<<
#include <iostream>              // for operator<<, basic_ostream, char_traits, endl, cerr
#include <stdlib.h>              // for exit

#include "ElementsKernel/Exit.h"  // for ExitCode
#include "ElementsKernel/Path.h"  // for Item

namespace boost::program_options {
template <class charT>
class basic_parsed_options;
}

namespace Elements {

template <class charT>
void ProgramManager::checkCommandLineOptions(
    const boost::program_options::basic_parsed_options<charT>& cmd_parsed_options) {

  for (const auto& o : cmd_parsed_options.options) {
    if (o.string_key == "config-file") {
      if (o.value.size() != 1) {
        std::cerr << "Wrong usage of the --config-file option" << std::endl;
        exit(static_cast<int>(ExitCode::USAGE));
      } else {
        auto conf_file = Path::Item{o.value[0]};
        if (not boost::filesystem::exists(conf_file)) {
          std::cerr << "The " << conf_file << " configuration file doesn't exist!" << std::endl;
          exit(static_cast<int>(ExitCode::CONFIG));
        }
      }
    }
  }
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PROGRAM_MANAGER_IMPL_
