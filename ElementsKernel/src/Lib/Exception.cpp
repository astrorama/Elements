/**
 * @file Auxiliary.cpp
 * @brief Implementation of the Exception class
 * @date Feb 8, 2017
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
 *
 *
 */

#include <utility>

#include "ElementsKernel/Exception.h"

namespace Elements {

Exception::Exception(const ExitCode e) : m_exit_code{e} {}

Exception::Exception(const char* message, const ExitCode e) : m_error_msg(message), m_exit_code{e} {}

Exception::Exception(std::string message, const ExitCode e) : m_error_msg(std::move(message)), m_exit_code{e} {}

Exception::Exception(const Exception& rhs) noexcept = default;

Exception::~Exception() noexcept = default;

const char* Exception::what() const noexcept {
  return m_error_msg.c_str();
}

ExitCode Exception::exitCode() const noexcept {
  return m_exit_code;
}

}  // namespace Elements
