/**
 * @file FloatPrecision.cpp
 * @date October 30th, 2018
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
 */

#include <cmath>     // for nextafter, nexttoward
#include <complex>   // for cos, pow
#include <iomanip>   // for operator<<, setprecision
#include <iostream>  // for operator<<, endl, cout, basic_ostream, basic_ostream::operator<<, fixed
#include <limits>    // for numeric_limits
#include <map>       // for map
#include <string>    // for char_traits, string

#include <boost/math/constants/constants.hpp>     // for pi
#include <boost/math/special_functions/next.hpp>  // for float_next

#include "ElementsKernel/Main.h"     // for MAIN_FOR
#include "ElementsKernel/Program.h"  // for Program
#include "ElementsKernel/Unused.h"   // for ELEMENTS_UNUSED

using std::cout;
using std::endl;
using std::map;
using std::string;

namespace Elements {
namespace Examples {

constexpr int PRINT_PRECISION = 32;

template <typename T>
void printTitle() {
  cout << "================================================================================" << endl;
  cout << "Float:" << endl;
  cout << "--------------------------------------------------------------------------------" << endl;
}

template <>
void printTitle<double>() {
  cout << "================================================================================" << endl;
  cout << "Double:" << endl;
  cout << "--------------------------------------------------------------------------------" << endl;
}

template <>
void printTitle<long double>() {
  cout << "================================================================================" << endl;
  cout << "Long Double:" << endl;
  cout << "--------------------------------------------------------------------------------" << endl;
}

template <typename T>
constexpr T Zero() {
  return 0.0f;
}

template <>
constexpr double Zero<double>() {
  return 0.0;
}

template <>
constexpr long double Zero<long double>() {
  return 0.0l;
}

template <typename T>
constexpr T One() {
  return 1.0f;
}

template <>
constexpr double One<double>() {
  return 1.0;
}

template <>
constexpr long double One<long double>() {
  return 1.0l;
}

template <typename T>
constexpr T Two() {
  return 2.0f;
}

template <>
constexpr double Two<double>() {
  return 2.0;
}

template <>
constexpr long double Two<long double>() {
  return 2.0l;
}

template <typename T>
constexpr T Seven() {
  return 7.0f;
}

template <>
constexpr double Seven<double>() {
  return 7.0;
}

template <>
constexpr long double Seven<long double>() {
  return 7.0l;
}

template <typename T>
constexpr T Ten() {
  return 10.0f;
}

template <>
constexpr double Ten<double>() {
  return 10.0;
}

template <>
constexpr long double Ten<long double>() {
  return 10.0l;
}

template <typename T>
void printFloatPrecision() {

  using boost::math::float_next;
  using boost::math::constants::pi;
  using std::cos;
  using std::nextafter;
  using std::nexttoward;
  using std::numeric_limits;
  using std::pow;

  printTitle<T>();

  auto zero             = Zero<T>();
  auto zero_plus        = float_next(zero);
  auto next_after_zero  = nextafter(zero, Two<T>());
  auto next_zero_toward = nexttoward(zero, Two<long double>());

  cout << "zero: " << zero << endl;
  cout << "next to zero: " << zero_plus << endl;
  cout << "next after zero: " << next_after_zero << endl;
  cout << "next after zero toward two: " << next_zero_toward << endl;

  auto one                    = One<T>();
  auto one_plus               = float_next(one);
  auto next_after_one         = nextafter(one, Two<T>());
  auto next_one_toward        = nexttoward(one, Two<long double>());
  auto cos_pi_over_7          = cos(pi<T>() / Seven<T>());
  auto default_test_tolerance = pow(Ten<T>(), -numeric_limits<T>::digits10);

  cout << "one: " << one << endl;
  cout << "next to one: " << one_plus << endl;
  cout << "next after one: " << next_after_one << endl;
  cout << "next after one toward two: " << next_one_toward << endl;

  cout << "pi: " << pi<T>() << endl;
  cout << "the Cosine of pi/7: " << cos_pi_over_7 << endl;
  cout << "the default test tolerance: " << default_test_tolerance << endl;

  cout << "The mantissa digits: " << numeric_limits<T>::digits << endl;
  cout << "The decimal digits: " << numeric_limits<T>::digits10 << endl;
  cout << "The epsilon: " << numeric_limits<T>::epsilon() << endl;
  cout << "The minimum exponent: " << numeric_limits<T>::min_exponent << endl;
  cout << "The minimum decimal exponent: " << numeric_limits<T>::min_exponent10 << endl;
  cout << "The maximum exponent: " << numeric_limits<float>::max_exponent << endl;
  cout << "The maximum decimal exponent: " << numeric_limits<T>::max_exponent10 << endl;
  cout << "The minimum: " << numeric_limits<T>::min() << endl;
  cout << "The maximum: " << numeric_limits<T>::max() << endl;
  cout << "The sizeof: " << sizeof(T) << endl;
  cout << "The sizeof in bits: " << 8 * sizeof(T) << endl;
}

class FloatPrecision : public Program {

public:
  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    cout << std::fixed << std::setprecision(PRINT_PRECISION);

    printFloatPrecision<float>();

    printFloatPrecision<double>();

    printFloatPrecision<long double>();

    return ExitCode::OK;
  }
};

}  // namespace Examples
}  // namespace Elements

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::FloatPrecision)
