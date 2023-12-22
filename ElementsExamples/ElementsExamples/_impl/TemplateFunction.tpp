/**
 * @file ElementsExamples/_impl/TemplateFunction.tpp
 * @brief Implementation of the TemplateFunction
 * @date 2023-04-07
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
 *
 */

// IWYU pragma: private, include "ElementsExamples/TemplateFunction.h"

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_IMPL_
#error "This file should not be included directly! Use ElementsExamples/TemplateFunction.h instead"
#else

#include <iostream>

namespace Elements::Examples {

template <typename T>
void templateFunction() {
  std::cout << "foo!" << static_cast<T>(3.14f) << std::endl;
}

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_IMPL_
