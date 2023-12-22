/**
 * @file Auxiliary_test.cpp
 *
 * Created on: Dec 4, 2013
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

#include "ElementsKernel/Auxiliary.h"

#include <algorithm>  // for copy_if, for_each
#include <iterator>   // for distance
#include <string>     // for allocator, string
#include <vector>     // for vector

#include <boost/filesystem/operations.hpp>  // for exists, create_directory, is_regular
#include <boost/filesystem/path.hpp>        // for operator/, path
#include <boost/test/unit_test.hpp>  // for BOOST_PP_IIF_1, BOOST_PP_IIF_0, BOOST_PP_EXPR_IIF_1, BOOST_PP_BOOL_1, BOOST_PP_BOOL_0, BOOST_PP_FOR_CHECK_BOOST_PP_NIL, BOOST_PP_TUPLE_ELEM_O_3, BOOST_PP_VARIADIC_ELEM_3, operator<<, BOOST_PP_SEQ_ELEM_0, BOOST_PP_DEC_1, BOOST_PP_BOOL_2, BOOST_PP_DEC_2, BOOST_PP_FOR_0, BOOST_PP_COMPL_0, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_4, BOOST_PP_DEC_4, BOOST_PP_BOOL_3, BOOST_PP_BOOL_4, BOOST_PP_NOT_EQUAL_1, BOOST_PP_NOT_EQUAL_CHECK_BOOST_PP_NOT_EQUAL_1, BOOST_AUTO_TEST_CASE, BOOST_PP_DEC_128, BOOST_PP_DEC_16, BOOST_PP_DEC_3, BOOST_PP_DEC_32, BOOST_PP_DEC_64, BOOST_PP_DEC_8, BOOST_PP_FOR_1, BOOST_PP_FOR_127, BOOST_PP_FOR_15, BOOST_PP_FOR_3, BOOST_PP_FOR_31, BOOST_PP_FOR_63, BOOST_PP_FOR_7, BOOST_PP_NODE_ENTRY_256, BOOST_CHECK_EQUAL_COLLECTIONS, BOOST_FIXTURE_TEST_CASE, BOOST_PP_DEC_5, BOOST_PP_NOT_EQUAL_2, BOOST_PP_NOT_EQUAL_CHECK_BOOST_PP_NOT_EQUAL_2, BOOST_PP_SEQ_ENUM_4, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_5, BOOST_TEST_TOOL_PASS_ARGS1, BOOST_TEST_TOOL_PASS_PRED1, BOOST_CHECK_EQUAL, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_2, BOOST_PP_SEQ_SIZE_BOOST_PP_SEQ_SIZE_3, BOOST_TEST_TOOL_PASS_ARGS0, BOOST_TEST_TOOL_PASS_ARGS2, BOOST_TEST_TOOL_PASS_PRED0, BOOST_TEST_TOOL_PASS_PRED2, BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_THROW

#include "ElementsKernel/Exception.h"  // for Exception
#include "ElementsKernel/Path.h"       // for Item, join
#include "ElementsKernel/System.h"     // for DEFAULT_INSTALL_PREFIX
#include "ElementsKernel/Temporary.h"  // for TempDir, TempEnv

using std::string;
using std::vector;

using boost::filesystem::exists;
using boost::filesystem::is_regular;

namespace Elements {

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Auxiliary_Fixture {

  TempDir            m_top_dir;
  vector<Path::Item> m_item_list;
  vector<Path::Item> m_target_item_list;
  vector<Path::Item> m_real_item_list;
  vector<Path::Item> m_target_real_item_list;

  Auxiliary_Fixture() : m_top_dir{"Auxiliary_test-%%%%%%%"} {

    using std::copy_if;
    using std::distance;
    using std::for_each;

    m_item_list.emplace_back(m_top_dir.path() / "test1");
    m_item_list.emplace_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.emplace_back(m_top_dir.path() / "test2");
    m_item_list.emplace_back(m_top_dir.path() / "test3");

    for_each(m_item_list.cbegin(), m_item_list.cend(), [](const Path::Item& p) {
      boost::filesystem::create_directory(p);
    });

    m_item_list.emplace_back(m_top_dir.path() / "test4");

    m_target_item_list = m_item_list;

    m_target_item_list.emplace_back(Path::Item(System::DEFAULT_INSTALL_PREFIX) / "share" / "auxdir");
    m_target_item_list.emplace_back(Path::Item(System::DEFAULT_INSTALL_PREFIX) / "share" / "aux");

    m_real_item_list.resize(m_item_list.size());
    auto it = copy_if(m_item_list.begin(), m_item_list.end(), m_real_item_list.begin(), [](const Path::Item& p) {
      return exists(p);
    });
    m_real_item_list.erase(it, m_real_item_list.end());

    m_target_real_item_list.resize(m_target_item_list.size());
    auto it2 = copy_if(m_target_item_list.begin(), m_target_item_list.end(), m_target_real_item_list.begin(),
                       [](const Path::Item& p) {
                         return exists(p);
                       });
    m_target_real_item_list.erase(it2, m_target_real_item_list.end());
  }

  ~Auxiliary_Fixture() = default;
};

BOOST_AUTO_TEST_SUITE(Auxiliary_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(AuxiliaryConstructor_test) {

  Path::Item make_template = getAuxiliaryPath("ElementsKernel/templates/Makefile.in");
}

BOOST_AUTO_TEST_CASE(AuxiliaryConstructor2_test) {

  const string make_template_stem{"ElementsKernel/templates/Makefile.in"};

  Path::Item make_template = getAuxiliaryPath(make_template_stem);
}

BOOST_AUTO_TEST_CASE(AuxiliaryConstructor3_test) {

  const Path::Item make_template_stem{"ElementsKernel/templates/Makefile.in"};

  Path::Item make_template = getAuxiliaryPath(make_template_stem);
}

BOOST_AUTO_TEST_CASE(AuxiliaryVariableName_test) {

  BOOST_CHECK_EQUAL(getAuxiliaryVariableName(), "ELEMENTS_AUX_PATH");
}

BOOST_AUTO_TEST_CASE(AuxiliaryException_test) {

  BOOST_CHECK_THROW(getAuxiliaryPath("NonExistingFile.txt"), Exception);
}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Auxiliary_Fixture) {

  auto env = TempEnv();

  env["ELEMENTS_AUX_PATH"] = Path::join(m_item_list);

  auto locations = getAuxiliaryLocations();

  BOOST_CHECK_EQUAL_COLLECTIONS(locations.begin(), locations.end(), m_target_item_list.begin(),
                                m_target_item_list.end());
}

BOOST_FIXTURE_TEST_CASE(getFromLocationsExist_test, Auxiliary_Fixture) {

  auto env = TempEnv();

  env["ELEMENTS_AUX_PATH"] = Path::join(m_real_item_list);

  auto locations = getAuxiliaryLocations(true);

  BOOST_CHECK_EQUAL_COLLECTIONS(locations.begin(), locations.end(), m_target_real_item_list.begin(),
                                m_target_real_item_list.end());
}

BOOST_FIXTURE_TEST_CASE(NamespaceAlias_test, Auxiliary_Fixture) {

  {
    auto env = TempEnv();

    env["ELEMENTS_AUX_PATH"] = Path::join(m_real_item_list);

    auto locations = Auxiliary::getLocations(true);

    BOOST_CHECK_EQUAL_COLLECTIONS(locations.begin(), locations.end(), m_target_real_item_list.begin(),
                                  m_target_real_item_list.end());
  }

  BOOST_CHECK_EQUAL(Auxiliary::getVariableName(), "ELEMENTS_AUX_PATH");

  Path::Item make_template = Auxiliary::getPath("ElementsKernel/templates/Makefile.in");

  BOOST_CHECK_THROW(Auxiliary::getPath("NonExistingFile.txt"), Exception);
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------

}  // namespace Elements
