/**
 * @file util.h
 *
 * @date Oct 14, 2016
 * @author hubert
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_UTIL_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A function that performs a specific operation.
 *
 * This function returns an integer value. The operation performed by
 * the function is predefined and determined at implementation.
 *
 * @return An integer value representing the result of the operation.
 */
int myFunc(void);

/**
 * @brief A constant that defines a specific integral value.
 *
 * This constant is assigned the value 42 and is used throughout the program
 * to represent a fixed, predefined value. Its purpose may vary depending on
 * the context of use in different parts of the codebase.
 */
extern int MY_CONST;

#ifdef __cplusplus
}
#endif

#endif /* ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_UTIL_H_ */
