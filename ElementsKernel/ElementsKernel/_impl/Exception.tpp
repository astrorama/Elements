/**
 * @file ElementsKernel/_impl/Exception.tpp
 * @brief implementation of the templates declared in ElementsKernel/Exception.h
 * @date 2023-12-11
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

// IWYU pragma: private, include "ElementsKernel/Exception.h"

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_EXCEPTION_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Exception.h instead"
#else

#include <cstdio>       // for snprintf, size_t
#include <sstream>      // for basic_ostream, stringstream
#include <string>       // for char_traits, allocator, operator<<, string
#include <type_traits>  // for enable_if, is_same
#include <utility>      // for forward

#include "ElementsKernel/Exit.h"  // for ExitCode

namespace Elements {

template <typename... Args>
Exception::Exception(const char* string_format, Args&&... args) : m_exit_code{ExitCodeHelper<Args...>{args...}.code} {
  std::size_t len     = snprintf(nullptr, 0, string_format, std::forward<Args>(args)...) + 1;
  char*       message = new char[len];
  snprintf(message, len, string_format, std::forward<Args>(args)...);
  m_error_msg = std::string(message);
  delete[] message;
}

template <typename T>
void Exception::appendMessage(const T& message) {
  std::stringstream new_message;
  new_message << m_error_msg << message;
  m_error_msg = new_message.str();
}

template <typename Last>
Exception::ExitCodeHelper<Last>::ExitCodeHelper(const Last& last) : code{getCode(last)} {}

template <typename Last>
template <typename T, typename std::enable_if<std::is_same<T, ExitCode>::value>::type*>
ExitCode Exception::ExitCodeHelper<Last>::getCode(const T& t) {
  return t;
}

template <typename Last>
template <typename T, typename std::enable_if<not std::is_same<T, ExitCode>::value>::type*>
ExitCode Exception::ExitCodeHelper<Last>::getCode(const T&) {
  return ExitCode::NOT_OK;
}

template <typename First, typename... Rest>
Exception::ExitCodeHelper<First, Rest...>::ExitCodeHelper(const First&, const Rest&... rest)
    : Exception::ExitCodeHelper<Rest...>::template ExitCodeHelper<Rest...>(rest...) {}

template <typename Ex, typename T, typename>
auto operator<<(Ex&& ex, const T& message) -> decltype(std::forward<Ex>(ex)) {
  ex.appendMessage(message);
  return std::forward<Ex>(ex);
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_EXCEPTION_IMPL_
