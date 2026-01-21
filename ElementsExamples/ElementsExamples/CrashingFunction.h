/**
 * @file CrashingFunction.h
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

/**
 * @addtogroup ElementsExamples ElementsExamples
 * @{
 */

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CRASHINGFUNCTION_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CRASHINGFUNCTION_H_

#include "ElementsKernel/Export.h"

namespace Elements::Examples {

/**
 * @brief A function that demonstrates intentional throwing of an exception.
 *
 * This method logs an entry message and then throws a logic_error exception
 * with a specified error message. It is primarily used for testing and
 * demonstrating error handling or crash scenarios.
 *
 * @throws std::logic_error Always throws an exception with the message "arrg!".
 */
ELEMENTS_API void crashingFunction();

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CRASHINGFUNCTION_H_

/**@}*/
