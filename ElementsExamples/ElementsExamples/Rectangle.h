/**
 * @file ElementsExamples/Rectangle.h
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

namespace Elements::Examples {

/**
 * @class Rectangle
 * @brief Represents a rectangle defined by four corner coordinates.
 *
 * This class provides methods to calculate rectangle dimensions,
 * area, and perform coordinate transformations such as movement.
 */
class ELEMENTS_API Rectangle {
public:
  /**
   * @var x0
   * @brief Represents the x-coordinate of the top-left corner of a rectangle.
   *
   * This integer variable is used to define the horizontal starting position
   * of a rectangular shape in a Cartesian coordinate system.
   */
  int x0;
  /**
   * @var y0
   * @brief Specifies the vertical coordinate of the bottom-left corner of a rectangle.
   *
   * Represents the y-coordinate used to define a rectangle's position or dimensions
   * within a two-dimensional space.
   */
  int y0;
  /**
   * @var int x1
   * @brief Represents the x-coordinate of the top-right corner of a rectangle.
   *
   * Used to define the horizontal boundary of the rectangle, as part
   * of its diagonal coordinates. This coordinate helps in calculating
   * properties such as length, area, and in determining rectangle transformations.
   */
  int x1;
  /**
   * @variable y1
   * @brief Represents the upper or lower y-coordinate of a rectangle's boundary.
   *
   * The variable y1 defines the second y-coordinate used to compute the
   * height and area of a rectangle, as well as other operations
   * related to its positioning and dimensions.
   */
  int y1;
  /// Default constructor
  Rectangle(int xl, int yl, int xu, int yu);
  /// Destructor
  ~Rectangle();
  /**
   * @brief Retrieves the length of the rectangle.
   *
   * This method calculates and returns the horizontal dimension
   * of the rectangle based on its coordinates.
   *
   * @return The length of the rectangle.
   */
  int getLength() const;
  /**
   * @brief Retrieves the height of the rectangle.
   *
   * Calculates the vertical dimension of the rectangle
   * based on its corner coordinates.
   *
   * @return The height of the rectangle as an integer.
   */
  int getHeight() const;
  /**
   * @brief Calculates and returns the area of the rectangle.
   *
   * This method computes the area by multiplying the length and height
   * of the rectangle, which are derived from its corner coordinates.
   *
   * @return The area of the rectangle as an integer.
   */
  int getArea() const;
  /**
   * @brief Moves the rectangle by the specified offsets in the x and y directions.
   *
   * This method adjusts the rectangle's corner coordinates by adding the given
   * horizontal (dx) and vertical (dy) offsets, effectively shifting its position
   * while retaining its size and shape.
   *
   * @param dx The offset to move along the x-axis.
   * @param dy The offset to move along the y-axis.
   */
  void move(int dx, int dy);
};

}  // namespace Elements::Examples

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_RECTANGLE_H_

/**@}*/
