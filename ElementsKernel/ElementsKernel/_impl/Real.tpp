/**
 * @file ElementsKernel/_impl/Real.tpp
 * @brief implementation of the templates declared in ElementsKernel/Real.h
 * @date 2023-12-13
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

// IWYU pragma: private, include "ElementsKernel/Real.h"

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_REAL_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Real.h instead"
#else

#include "ElementsKernel/Unused.h"  // for ELEMENTS_UNUSED
#include <cstring>                  // for size_t, memcpy

namespace Elements {

template <typename RawType>
constexpr std::size_t defaultMaxUlps() {
  return FLT_DEFAULT_MAX_ULPS;
}

template <>
constexpr std::size_t defaultMaxUlps<float>() {
  return FLT_DEFAULT_MAX_ULPS;
}

template <>
constexpr std::size_t defaultMaxUlps<double>() {
  return DBL_DEFAULT_MAX_ULPS;
}

template <typename RawType>
FloatingPoint<RawType>::FloatingPoint(const RawType& x) {
  m_u.m_value = x;
}

template <typename RawType>
RawType FloatingPoint<RawType>::ReinterpretBits(const Bits& bits) {
  FloatingPoint fp(0);
  fp.m_u.m_bits = bits;
  return fp.m_u.m_value;
}

template <typename RawType>
RawType FloatingPoint<RawType>::Infinity() {
  return ReinterpretBits(s_exponent_bitmask);
}

template <typename RawType>
const typename FloatingPoint<RawType>::Bits& FloatingPoint<RawType>::bits() const {
  return m_u.m_bits;
}

template <typename RawType>
typename FloatingPoint<RawType>::Bits FloatingPoint<RawType>::exponentBits() const {
  return s_exponent_bitmask & m_u.m_bits;
}

template <typename RawType>
typename FloatingPoint<RawType>::Bits FloatingPoint<RawType>::fractionBits() const {
  return s_fraction_bitmask & m_u.m_bits;
}

template <typename RawType>
typename FloatingPoint<RawType>::Bits FloatingPoint<RawType>::signBit() const {
  return s_sign_bitmask & m_u.m_bits;
}

template <typename RawType>
bool FloatingPoint<RawType>::isNan() const {
  // It's a NAN if the exponent bits are all ones and the fraction
  // bits are not entirely zeros.
  return (exponentBits() == s_exponent_bitmask) && (fractionBits() != 0);
}

template <typename RawType>
bool FloatingPoint<RawType>::AlmostEquals(const FloatingPoint& rhs) const {
  // The IEEE standard says that any comparison operation involving
  // a NAN must return false.
  if (isNan() || rhs.isNan()) {
    return false;
  }
  return distanceBetweenSignAndMagnitudeNumbers(m_u.m_bits, rhs.m_u.m_bits) <= m_max_ulps;
}

template <typename RawType>
typename FloatingPoint<RawType>::Bits FloatingPoint<RawType>::signAndMagnitudeToBiased(const Bits& sam) {
  if (s_sign_bitmask & sam) {
    // sam represents a negative number.
    return ~sam + 1;
  } else {
    // sam represents a positive number.
    return s_sign_bitmask | sam;
  }
}

template <typename RawType>
typename FloatingPoint<RawType>::Bits FloatingPoint<RawType>::distanceBetweenSignAndMagnitudeNumbers(const Bits& sam1,
                                                                                                     const Bits& sam2) {
  const Bits biased1 = signAndMagnitudeToBiased(sam1);
  const Bits biased2 = signAndMagnitudeToBiased(sam2);
  return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
}

template <typename FloatType>
bool almostEqual2sComplement(ELEMENTS_UNUSED const FloatType& a, ELEMENTS_UNUSED const FloatType& b,
                             ELEMENTS_UNUSED const std::size_t& max_ulps) {
  return false;
}

template <typename RawType>
bool isNan(const RawType& x) {

  using Bits = typename TypeWithSize<sizeof(RawType)>::UInt;
  Bits x_bits;
  std::memcpy(&x_bits, &x, sizeof(x_bits));

  Bits x_exp_bits  = FloatingPoint<RawType>::s_exponent_bitmask & x_bits;
  Bits x_frac_bits = FloatingPoint<RawType>::s_fraction_bitmask & x_bits;

  return (x_exp_bits == FloatingPoint<RawType>::s_exponent_bitmask) && (x_frac_bits != 0);
}

template <typename RawType, std::size_t max_ulps>
bool isEqual(const RawType& left, const RawType& right) {

  bool is_equal{false};

  if (not(isNan<RawType>(left) or isNan<RawType>(right))) {
    using Bits = typename TypeWithSize<sizeof(RawType)>::UInt;
    Bits l_bits;
    Bits r_bits;
    std::memcpy(&l_bits, &left, sizeof(l_bits));
    std::memcpy(&r_bits, &right, sizeof(r_bits));
    is_equal = (FloatingPoint<RawType>::distanceBetweenSignAndMagnitudeNumbers(l_bits, r_bits) <= max_ulps);
  }

  return is_equal;
}

template <std::size_t max_ulps>
inline bool isEqual(const float& left, const float& right) {
  return (isEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isEqual(const double& left, const double& right) {
  return (isEqual<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps>
inline bool isNotEqual(const RawType& left, const RawType& right) {
  return (not isEqual<RawType, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isNotEqual(const float& left, const float& right) {
  return (isNotEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isNotEqual(const double& left, const double& right) {
  return (isNotEqual<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps>
bool isLess(const RawType& left, const RawType& right) {
  bool is_less{false};

  if (left < right && (not isEqual<RawType, max_ulps>(left, right))) {
    is_less = true;
  }

  return is_less;
}

template <std::size_t max_ulps>
inline bool isLess(const float& left, const float& right) {
  return (isLess<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isLess(const double& left, const double& right) {
  return (isLess<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps>
bool isGreater(const RawType& left, const RawType& right) {
  bool is_greater{false};

  if (left > right && (not isEqual<RawType, max_ulps>(left, right))) {
    is_greater = true;
  }

  return is_greater;
}

template <std::size_t max_ulps>
inline bool isGreater(const float& left, const float& right) {
  return (isGreater<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isGreater(const double& left, const double& right) {
  return (isGreater<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps>
bool isLessOrEqual(const RawType& left, const RawType& right) {
  bool is_loe{false};

  if (not isGreater<RawType, max_ulps>(left, right)) {
    is_loe = true;
  }

  return is_loe;
}

template <std::size_t max_ulps>
inline bool isLessOrEqual(const float& left, const float& right) {
  return (isLessOrEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isLessOrEqual(const double& left, const double& right) {
  return (isLessOrEqual<double, max_ulps>(left, right));
}

template <typename RawType, std::size_t max_ulps>
bool isGreaterOrEqual(const RawType& left, const RawType& right) {
  bool is_goe{false};

  if (not isLess<RawType, max_ulps>(left, right)) {
    is_goe = true;
  }

  return is_goe;
}

template <std::size_t max_ulps>
inline bool isGreaterOrEqual(const float& left, const float& right) {
  return (isGreaterOrEqual<float, max_ulps>(left, right));
}

template <std::size_t max_ulps>
inline bool isGreaterOrEqual(const double& left, const double& right) {
  return (isGreaterOrEqual<double, max_ulps>(left, right));
}

template <typename RawType>
bool realBitWiseEqual(const RawType& left, const RawType& right) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
  return (left == right);
#pragma GCC diagnostic pop
}

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_REAL_IMPL_
