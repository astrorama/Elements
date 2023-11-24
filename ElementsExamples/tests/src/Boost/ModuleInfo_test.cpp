/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013Jun 20, 2013
 *     Author: Pierre Dubath
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

#include "ElementsExamples/ModuleInfo.h"

#include <string>  // for allocator, operator+, char_traits, string

#include <boost/test/unit_test.hpp>  // for BOOST_PP_IIF_1, BOOST_PP_IIF_0, BOOST_PP_BOOL_0, BOOST_PP_BOOL_1, BOOST_PP_EXPR_IIF_1, BOOST_PP_FOR_CHECK_BOOST_PP_NIL, BOOST_PP_DEC_1, BOOST_PP_TUPLE_ELEM_O_3, BOOST_PP_VARIADIC_ELEM_3, operator<<, BOOST_PP_BOOL_2, BOOST_PP_DEC_2, BOOST_PP_FOR_0, BOOST_PP_SEQ_ELEM_0, BOOST_CHECK_EQUAL, BOOST_PP_COMPL_0, BOOST_PP_DEC_128, BOOST_PP_DEC_16, BOOST_PP_DEC_3, BOOST_PP_DEC_32, BOOST_PP_DEC_4, BOOST_PP_DEC_64, BOOST_PP_DEC_8, BOOST_PP_FOR_1, BOOST_PP_FOR_127, BOOST_PP_FOR_15, BOOST_PP_FOR_3, BOOST_PP_FOR_31, BOOST_PP_FOR_63, BOOST_PP_FOR_7, BOOST_PP_NODE_ENTRY_256, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_2, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_3, BOOST_TEST_TOOL_PASS_ARGS0, BOOST_TEST_TOOL_PASS_PRED0, BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_FIXTURE_TEST_CASE, BOOST_PP_NOT_EQUAL_1, BOOST_PP_NOT_EQUAL_2, BOOST_PP_NOT_EQUAL_CHECK_BOOST_PP_NOT_EQUAL_1, BOOST_PP_NOT_EQUAL_CHECK_BOOST_PP_NOT_EQUAL_2

#include "ElementsKernel/ModuleInfo.h"  // for ModuleInfo
#include "ElementsKernel/System.h"      // for LIB_PREFIX, LIB_SUFFIX

#include "ThisProject.h"  // for THIS_PROJECT_ORIGINAL_VERSION, THIS_PROJECT_USE_SOVERSION

//-----------------------------------------------------------------------------

/*
 * Fixture to compare the test result against reference values
 */
struct ModuleInfoFixture {

  ModuleInfoFixture() {
    // call the constructor
  }

  ~ModuleInfoFixture() {
    // delete fixture object
  }
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(ModuleInfo_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(ModuleName_test, ModuleInfoFixture) {
  if (THIS_PROJECT_USE_SOVERSION) {
    BOOST_CHECK_EQUAL(Elements::Examples::getModuleInfo().name(),
                      std::string(Elements::System::LIB_PREFIX + "ElementsExamples" + Elements::System::LIB_SUFFIX +
                                  "." + THIS_PROJECT_ORIGINAL_VERSION));
  } else {
    BOOST_CHECK_EQUAL(Elements::Examples::getModuleInfo().name(),
                      std::string(Elements::System::LIB_PREFIX + "ElementsExamples" + Elements::System::LIB_SUFFIX));
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
