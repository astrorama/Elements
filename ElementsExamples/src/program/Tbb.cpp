// Copyright (C) 2012-2024, UNIGE and contributors (for the Euclid Science Ground Segment)
// This file is part of Elements <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements>
// SPDX-License-Identifier: LGPL-3.0-or-later

/// @file Tbb.cpp
/// @brief Example using the Threading Building Blocks
/// @date 2024-09-27
/// @author Hubert Degaudenzi

#include <chrono>  // for high_resolution_clock
#include <iostream>
#include <map>     // for maps
#include <string>  // for strings
#include <thread>  // for std::thread

#include <oneapi/tbb.h>  // for the Threading Building Blocks

#include "ElementsKernel/Main.h"     // for MAIN_FOR
#include "ElementsKernel/Program.h"  // for Program
#include "ElementsKernel/Unused.h"   // for ELEMENTS_UNUSED

namespace tbb = oneapi::tbb;

namespace Elements {
namespace Examples {

class Tbb : public Program {

public:
  ///
  ExitCode mainMethod(ELEMENTS_UNUSED std::map<std::string, VariableValue>& args) override {

    using namespace std::chrono;

    auto log = Logging::getLogger("Tbb");

    const auto processor_count = std::thread::hardware_concurrency();
    log.info() << "Number of native threads: " << processor_count;
    const auto default_concurrency = tbb::info::default_concurrency();
    log.info() << "TBB default concurrency: " << default_concurrency;

    const auto          start = high_resolution_clock::now();
    tbb::global_control control(tbb::global_control::max_allowed_parallelism, processor_count);

    const int first_number = 1;
    const int last_number  = 10001;

    int simple_sum = 0;
    for (auto i = first_number; i < last_number; i++) {
      simple_sum += i;
    }

    log.info() << "Simple sum: " << simple_sum;

    const auto simple_stop     = high_resolution_clock::now();
    const auto simple_duration = duration_cast<microseconds>(simple_stop - start);

    log.info() << "Simple duration: " << simple_duration.count();

    int parallel_sum = tbb::parallel_reduce(
        tbb::blocked_range<int>(first_number, last_number), 0,
        [](tbb::blocked_range<int> const& r, int init) -> int {
          for (int v = r.begin(); v != r.end(); v++) {
            init += v;
          }
          return (init);
        },
        [](int lhs, int rhs) -> int {
          return (lhs + rhs);
        });

    log.info() << "Parallel sum: " << parallel_sum;

    const auto parallel_stop     = high_resolution_clock::now();
    const auto parallel_duration = duration_cast<microseconds>(parallel_stop - simple_stop);

    log.info() << "Parallel duration: " << parallel_duration.count();

    const auto stop           = high_resolution_clock::now();
    const auto total_duration = duration_cast<microseconds>(stop - start);

    log.info() << "Total duration: " << total_duration.count();

    return (ExitCode::OK);
  }
};

}  // namespace Examples
}  // namespace Elements

MAIN_FOR(Elements::Examples::Tbb)
