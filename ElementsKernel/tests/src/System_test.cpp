/**
 * @file BackTrace_test.cpp
 *
 * @date Aug 27, 2015
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

#include "ElementsKernel/System.h"

#include <sys/utsname.h>  // for uname, utsname

#include <boost/test/unit_test.hpp>  // for BOOST_PP_IIF_1, BOOST_PP_IIF_0, BOOST_PP_BOOL_0, BOOST_PP_BOOL_1, BOOST_PP_EXPR_IIF_1, BOOST_PP_FOR_CHECK_BOOST_PP_NIL, BOOST_PP_DEC_1, BOOST_PP_TUPLE_ELEM_O_3, BOOST_PP_VARIADIC_ELEM_3, operator<<, BOOST_PP_BOOL_2, BOOST_PP_DEC_2, BOOST_PP_FOR_0, BOOST_PP_SEQ_ELEM_0, BOOST_PP_COMPL_0, BOOST_PP_NOT_EQUAL_1, BOOST_PP_NOT_EQUAL_CHECK_BOOST_PP_NOT_EQUAL_1, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_PP_DEC_128, BOOST_PP_DEC_16, BOOST_PP_DEC_3, BOOST_PP_DEC_32, BOOST_PP_DEC_4, BOOST_PP_DEC_64, BOOST_PP_DEC_8, BOOST_PP_FOR_1, BOOST_PP_FOR_127, BOOST_PP_FOR_15, BOOST_PP_FOR_3, BOOST_PP_FOR_31, BOOST_PP_FOR_63, BOOST_PP_FOR_7, BOOST_PP_NODE_ENTRY_256, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_2, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_3, BOOST_TEST_TOOL_PASS_ARGS0, BOOST_TEST_TOOL_PASS_PRED0, BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

#include <string>  // for string, operator!=

#include "ElementsKernel/Environment.h"  // for Environment

using std::string;

namespace Elements {

BOOST_AUTO_TEST_SUITE(System_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(HostName_test) {

  using std::string;

  Environment current;

  if (current["HOSTNAME"].exists() and string(current["HOSTNAME"]) != ".") {
    BOOST_CHECK_EQUAL(string(current["HOSTNAME"]), System::hostName());
  }
}

BOOST_AUTO_TEST_CASE(osName_test) {

  string         osname = "UNKNOWN";
  struct utsname ut;
  if (::uname(&ut) == 0) {
    osname = ut.sysname;
  }

  BOOST_CHECK_EQUAL(System::osName(), osname);
}

BOOST_AUTO_TEST_CASE(osVersion_test) {

  string         osver = "UNKNOWN";
  struct utsname ut;
  if (uname(&ut) == 0) {
    osver = ut.release;
  }

  BOOST_CHECK_EQUAL(System::osVersion(), osver);
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

}  // namespace Elements
