/**
 * @file SwigClassExample.h
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_SWIGCLASSEXAMPLE_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_SWIGCLASSEXAMPLE_H_

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

namespace Elements::Examples {

/**
 * @class SwigClassExample
 *
 * @brief A class representing a simplified example for RA and DEC coordinates.
 *
 * This class provides access to RA (Right Ascension) and DEC (Declination) properties,
 * initialized through the constructor and accessible through getter methods.
 */
class ELEMENTS_API SwigClassExample final {

public:
  /**
   * @brief Constructs a SwigClassExample object with specified RA and DEC coordinates.
   *
   * This constructor initializes the RA (Right Ascension) and DEC (Declination) properties
   * with the provided values. If no values are provided, the default is 0.0 for both RA and DEC.
   *
   * @param ra The Right Ascension coordinate to initialize (default is 0.0).
   * @param dec The Declination coordinate to initialize (default is 0.0).
   */
  explicit SwigClassExample(const double ra = 0.0, const double dec = 0.0) : m_ra(ra), m_dec(dec) {}

  ~SwigClassExample() = default;

  /**
   * @brief Retrieves the Right Ascension (RA) value.
   *
   * This method returns the RA value stored in the instance of SwigClassExample.
   *
   * @return The RA (Right Ascension) value as a double.
   */
  double getRa() const;

  /**
   * @brief Retrieves the Declination (DEC) value of the object.
   *
   * This method provides access to the DEC property of the SwigClassExample instance.
   *
   * @return The DEC value as a double.
   */
  double getDec() const;

private:
  double m_ra;
  double m_dec;
};

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_SWIGCLASSEXAMPLE_H_

/**@}*/
