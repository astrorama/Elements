/**
 * @file ElementsExamples/TemplateFunction.h
 * @brief Example of a template function
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

/**
 * @addtogroup ElementsExamples ElementsExamples
 * @{
 */

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_H_

#include "ElementsKernel/Export.h"

namespace Elements::Examples {

template <typename T>
ELEMENTS_API void templateFunction();

// template instance declaration
extern template ELEMENTS_API void templateFunction<int>();
extern template ELEMENTS_API void templateFunction<double>();

}  // namespace Elements::Examples

/**
 * @def ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_IMPL_
 * @brief Macro to prevent the direct inclusion of the implementation file
 */
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_IMPL_
#include "ElementsExamples/_impl/TemplateFunction.tpp"  // IWYU pragma: export
#undef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_IMPL_

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEFUNCTION_H_

/**@}*/
