/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#define BOOST_TEST_IGNORE_NON_ZERO_CHILD_CODE
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <string>   // for allocator, basic_string, string, operator+, operator==
#include <utility>  // for pair
#include <vector>   // for vector

#include "ElementsKernel/Environment.h"               // for Environment
#include "ElementsKernel/Temporary.h"                 // for TempPath
#include "ElementsServices/DataSync/DataSyncUtils.h"  // for containsInThisOrder, localWorkspacePrefix, checkCall, getWorkdirVariable, DEFAULT_WORKDIR_VAR, WORKDIR_VAR_VAR, environmentVariable, lower, runCommandAndCaptureOutErr, DataSync

#include "fixtures/ConfigFilesFixture.h"  // for WorkspaceFixture

namespace DataSync = ElementsServices::DataSync;

using std::string;
using std::vector;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DataSyncUtils_test)

//-----------------------------------------------------------------------------

void checkLower(const vector<string>& casedList, const string& uncased) {
  for (const auto& cased : casedList) {
    BOOST_CHECK_EQUAL(DataSync::lower(cased), uncased);
  }
}

BOOST_AUTO_TEST_CASE(lower_test) {
  const vector<string> casedList{"webdav", "WebDAV", "WEBDAV"};
  checkLower(casedList, "webdav");
}

BOOST_AUTO_TEST_CASE(checkCall_test) {
  BOOST_CHECK(DataSync::checkCall("ls"));
  BOOST_CHECK(DataSync::checkCall("ls --help"));
  BOOST_CHECK(not DataSync::checkCall("This command should not work"));
}

BOOST_AUTO_TEST_CASE(runCommand_out_test) {
  const string msg("toto");
  auto [output, error] = DataSync::runCommandAndCaptureOutErr("echo " + msg);
  if (const string::size_type size = output.size(); output[size - 1] == '\n') {
    output.resize(size - 1);
  }
  BOOST_CHECK_EQUAL(output, msg);
}

BOOST_FIXTURE_TEST_CASE(localWorkspacePrefix_test, WorkspaceFixture) {
  if (const string localEv = DataSync::environmentVariable("NOPREFIX"); localEv.empty()) {
    BOOST_CHECK_NE(DataSync::localWorkspacePrefix(), "");
  } else {
    BOOST_CHECK_EQUAL(DataSync::localWorkspacePrefix(), "");
  }
}

// @TODO runCommand_err_test

BOOST_AUTO_TEST_CASE(containsInThisOrder_test) {

  using DataSync::containsInThisOrder;

  const string         input("0 one Two THREE");
  const vector<string> perfect = {"0", "one", "Two", "THREE"};
  const vector<string> partial = {"0", "one", "THR"};
  const vector<string> empty;
  const vector<string> unordered = {"0", "Two", "one", "THREE"};
  const vector<string> random    = {"R", "and", "OM"};
  const vector<string> wordcut   = {"0 one T", "wo THREE"};
  BOOST_CHECK(containsInThisOrder(input, perfect));
  BOOST_CHECK(containsInThisOrder(input, partial));
  BOOST_CHECK(containsInThisOrder(input, empty));
  BOOST_CHECK(containsInThisOrder(input, wordcut));
  BOOST_CHECK(not containsInThisOrder(input, unordered));
  BOOST_CHECK(not containsInThisOrder(input, random));
}

BOOST_AUTO_TEST_CASE(getWorkdirVariable_test) {

  using Elements::Environment;
  using Elements::TempPath;

  if (Environment current; current[DataSync::WORKDIR_VAR_VAR].empty()) {

    BOOST_CHECK_EQUAL(DataSync::getWorkdirVariable(), DataSync::DEFAULT_WORKDIR_VAR);
    if (not current[DataSync::DEFAULT_WORKDIR_VAR].empty()) {
      BOOST_CHECK_EQUAL(DataSync::localWorkspacePrefix(), string(current[DataSync::DEFAULT_WORKDIR_VAR]));
    }

    Environment local;
    local[DataSync::WORKDIR_VAR_VAR] = "THIS_WORKDIR";
    BOOST_CHECK_EQUAL(DataSync::getWorkdirVariable(), "THIS_WORKDIR");
    if (local["THIS_WORKDIR"].empty()) {
      const TempPath this_workdir;
      local["THIS_WORKDIR"] = this_workdir.path().string();
    }
    BOOST_CHECK_EQUAL(DataSync::localWorkspacePrefix(), string(local[local[DataSync::WORKDIR_VAR_VAR]]));
  } else {
    BOOST_CHECK_EQUAL(DataSync::getWorkdirVariable(), string(current[DataSync::WORKDIR_VAR_VAR]));
    BOOST_CHECK_EQUAL(DataSync::localWorkspacePrefix(), string(current[current[DataSync::DEFAULT_WORKDIR_VAR]]));
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
