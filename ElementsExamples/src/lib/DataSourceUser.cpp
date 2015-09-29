/*
 * DataSourceUser.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fdubath
 */

#include "ElementsExamples/DataSourceUser.h"

namespace Elements {
namespace ElementsExamples {

double DataSourceUser::sumRecords (const DataSourceInterface& data_source) {
  double sum = 0.;
  size_t records_number = data_source.countRecords();
  for (size_t index = 0; index < records_number; ++index) {
    sum += data_source.getRecordValue(index);
  }

  return sum;
}

} // namespace ElementsExamples
} // namespace Elements

