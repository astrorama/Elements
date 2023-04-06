/**
 * @file ElementsKernel/TemplateFunctionAlias.h
 * @brief Macro that creates an alias for a templated function
 * @date 2023-04-06
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

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPLATEFUNCTIONALIAS_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPLATEFUNCTIONALIAS_H_

#include <utility>  // for forward

/** @def TEMPLATE_FUNCTION_ALIAS(Target, Source)
 * @brief Macro that creates an alias for a templated function. It conserves the
 * type deduction. Please have a look at https://www.fluentcpp.com/2017/10/27/function-aliases-cpp/
 * @param Source the original function
 * @param Target the new alias
 */
#define TEMPLATE_FUNCTION_ALIAS(Target, Source)                                                                        \
  template <typename... Args>                                                                                          \
  inline auto Target(Args&&... args)->decltype(Source(std::forward<Args>(args)...)) {                                  \
    return Source(std::forward<Args>(args)...);                                                                        \
  }

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_TEMPLATEFUNCTIONALIAS_H_
