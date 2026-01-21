/**
 * @file ElementsKernel/_impl/Logging.tpp
 * @brief implementation of the templates declared in ElementsKernel/Logging.h
 * @date 2023-12-13
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public Licence as published by the Free Software Foundation; either version 3.0 of the Licence, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public Licence for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public Licence along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

// IWYU pragma: private, include "ElementsKernel/Logging.h"
#pragma once

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_LOGGING_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Logging.h instead"
#else

#include <utility>  // for forward

#include <log4cpp/Priority.hh>  // for Priority

namespace Elements {

template <typename... Args>
void Logging::debug(const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.debug(stringFormat, std::forward<Args>(args)...);
}

template <typename... Args>
void Logging::info(const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.info(stringFormat, std::forward<Args>(args)...);
}

template <typename... Args>
void Logging::warn(const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.warn(stringFormat, std::forward<Args>(args)...);
}

template <typename... Args>
void Logging::error(const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.error(stringFormat, std::forward<Args>(args)...);
}

template <typename... Args>
void Logging::fatal(const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.fatal(stringFormat, std::forward<Args>(args)...);
}

template <typename... Args>
void Logging::log(log4cpp::Priority::Value level, const char* stringFormat, Args&&... args) const {
  m_log4cppLogger.log(level, stringFormat, std::forward<Args>(args)...);
}

template <typename T>
Logging::LogMessageStream& Logging::LogMessageStream::operator<<(const T& m) {
  m_message << m;
  return *this;
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_LOGGING_IMPL_
