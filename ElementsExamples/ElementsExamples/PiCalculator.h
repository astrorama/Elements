/**
 * @file ElementsExamples/PiCalculator.h
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_

#include "ElementsKernel/Export.h"

namespace Elements::Examples {

/**
 * @brief Provides functionality to calculate the mathematical constant Pi.
 *
 * This class uses the Leibniz formula to calculate the value of Pi based
 * on a specified number of terms. Additionally, it allows the user
 * to provide a callback function to handle the result of the calculation.
 */
class ELEMENTS_API PiCalculator {
public:
  /// Default constructor
  PiCalculator() = default;

  /**
   * @brief Calculates the value of Pi using the Leibniz formula.
   *
   * This method computes the mathematical constant Pi by iterating
   * through the Leibniz formula for the number of terms specified.
   * The calculated value is then passed to a callback function that
   * has been set earlier using the setShowResultCallback method.
   *
   * @param terms The number of terms to use in the approximation of Pi.
   * A higher value results in a more accurate calculation.
   */
  void calculate(unsigned int terms) const;

  /// Type definition for the callback function that displays the result of the Pi calculation.
  using show_result_callback_type = int (*)(double pi);

  /**
   * @brief Sets the callback function to handle the result of the Pi calculation.
   *
   * This method assigns a user-defined function that will be invoked upon
   * completing the Pi calculation. The callback function takes a single argument
   * of type 'double' representing the calculated value of Pi.
   *
   * @param f A function of type `show_result_callback_type` to handle the calculated Pi value.
   */
  void setShowResultCallback(show_result_callback_type f);

private:
  show_result_callback_type m_show_result_callback{};
};

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_PICALCULATOR_H_

/**@}*/
