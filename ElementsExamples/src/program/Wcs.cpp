/**
 * @file Wcs.cpp
 * @date January 6th, 2015
 * @author Pierre Dubath
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

#include <map>     // for map
#include <string>  // for allocator, string

#include <wcslib/wcs.h>  // for wcsnps

#include "ElementsKernel/Main.h"     // for MAIN_FOR
#include "ElementsKernel/Program.h"  // for Program
#include "ElementsKernel/Unused.h"   // for ELEMENTS_UNUSED

using std::map;
using std::string;

namespace Elements {
namespace Examples {

class Wcs : public Program {

public:
  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    auto log = Logging::getLogger("WcsExample");

    int nps_max = wcsnps(-1);

    log.info() << "NPSMAX: " << nps_max;

    log.info() << "done with test program! ";

    return ExitCode::OK;
  }
};

}  // namespace Examples
}  // namespace Elements

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::Wcs)
