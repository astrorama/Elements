/**
 * @file ElementsKernel/EnableGMock.h
 * @brief Google Mock helper classes
 * @date December 16, 2014
 * @author Nikolaos Apostolakos
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
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_

#include <boost/test/unit_test.hpp>
#include <boost/version.hpp>  // for the BOOST_VERSION define
#include <gmock/gmock.h>      // IWYU pragma: export
#include <gtest/gtest.h>      // IWYU pragma: export

#include "ElementsKernel/Export.h"  // ELEMENTS_API

/**
 * @class BoostTestAdapter
 * @brief A listener class that integrates Google Test results with Boost Test.
 *
 * This class extends `testing::EmptyTestEventListener` to intercept test result
 * events in Google Test and adapt them for use in Boost Unit Test Framework.
 * It aids in relaying Google Test's test result failures into Boost Test's
 * reporting mechanism.
 *
 * Functional specifics:
 * - Overrides the `OnTestPartResult` method from `testing::EmptyTestEventListener`.
 * - Converts Google Test failures into Boost Test errors using the `BOOST_ERROR` macro.
 */
class BoostTestAdapter final : public testing::EmptyTestEventListener {

  void OnTestPartResult(const testing::TestPartResult& testPartResult) override {
    if (testPartResult.failed()) {
      BOOST_ERROR(testPartResult.summary());
    }
  }
};

/**
 * @class GoogleMockSetupFixture
 * @brief A fixture class that integrates Google Mock with Boost Test.
 *
 * This class is responsible for initializing Google Mock within the context of
 * a Boost Test framework. It sets up necessary command-line arguments for Google Mock
 * and replaces the default result printer with a Boost Test adapter.
 *
 * The fixture is typically used to provide seamless integration of Google Mock's
 * functionality into tests that utilize Boost Unit Test Framework.
 *
 * Functional specifics:
 * - Initializes Google Mock using the command-line arguments from Boost Test's
 *   master test suite.
 * - Replaces the default Google Mock result printer to properly route test
 *   results into the Boost Test reporting mechanism.
 */
class GoogleMockSetupFixture {

public:
  GoogleMockSetupFixture() {
    testing::InitGoogleMock(&boost::unit_test::framework::master_test_suite().argc,
                            boost::unit_test::framework::master_test_suite().argv);
    auto& listeners = testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new BoostTestAdapter);
  }
};

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_

/**
 * \example ElementsExamples/tests/src/GMock/TemplatedDataSourceUser_test.cpp
 * Example that shows the usage of the EnableGMock.h file.
 */

/**@}*/
