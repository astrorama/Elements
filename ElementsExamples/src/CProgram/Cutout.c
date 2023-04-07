/**
 * @file ElementsExamples/src/CProgram/Cutout.c
 * @date 2015-01-09
 * @author Pierre Dubath
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

#include <stdio.h>

#include "ElementsExamples/Util.h"

int main(int argc, char* argv[]) {

  printf("This is a constant: %d\n", MY_CONST);
  printf("This is the function result: %d\n", myFunc());

  return 0;
}
