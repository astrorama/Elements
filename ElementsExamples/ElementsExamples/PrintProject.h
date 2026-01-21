/**
 * @file PrintProject.h
 * @brief function that displays the current project properties
 * @date 2020-05-14
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PRINTPROJECT_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PRINTPROJECT_H_

#include "ElementsKernel/Export.h"

namespace Elements::Examples {

/**
 * @brief Prints the project information to the standard output.
 *
 * This method retrieves information about the current project, such as its
 * name or version, and outputs it to the console. It is a basic utility
 * function designed to display project details in a human-readable format.
 *
 * Usage of this method may be helpful for logging or debugging purposes to
 * provide contextual information about the project execution environment.
 */
ELEMENTS_API void printProject();

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PRINTPROJECT_H_

/**@}*/
