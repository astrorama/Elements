/**
 * @file crashingFunction.cpp
 *
 * @date Nov 15, 2016
 * @author hubert
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

#include "ElementsExamples/crashingFunction.h"

#include <stdexcept>                           // for the logic_error

#include "ElementsKernel/Logging.h"            // for the Logging

namespace Elements {
namespace Examples {

void crashingFunction() {
  Logging logger = Logging::getLogger("ElementsExamples");
  logger.info() << "Entering Crashing Function";

  throw std::logic_error("arrg!");
}

}  // namespace Examples
}  // namespace Elements
