/*
 * DataSourceUser_test.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: Florian Dubath
 */

#include "ElementsKernel/EnableGMock.h"
#include "ElementsKernel/Real.h" // isEqual

#include "ElementsExamples/TemplatedDataSourceUser.h" // Access the objects you want to test
#include "DataSourceUserTemplatedTypeMock.h" // Access the needed mock objects.

using testing::Return;
using Elements::Examples::DataSourceUserTemplatedTypeMock;

BOOST_AUTO_TEST_SUITE (TemplatedDataSourceUser_test_suite)

BOOST_AUTO_TEST_CASE(sumRecords_test) {
  // Setup mock
  DataSourceUserTemplatedTypeMock data_source_mock;
  EXPECT_CALL(data_source_mock, countRecords()).Times(1).WillOnce(testing::Return(5));
  for (size_t index=0; index<5;++index){
    EXPECT_CALL(data_source_mock, getRecordValue(index)).Times(1).WillOnce(Return(index+1.));
  }

  // object to test
  Elements::Examples::TemplatedDataSourceUser user{};
  double result = user.sumRecords<DataSourceUserTemplatedTypeMock>(data_source_mock);

  BOOST_CHECK_MESSAGE(Elements::isEqual(result,15.), "Expected value :"<<15<<" Actual value :"<<result);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()

