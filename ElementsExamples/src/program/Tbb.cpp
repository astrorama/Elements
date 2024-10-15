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

auto LOG = Logging::getLogger("Tbb");

namespace Examples {

void initTbb() {

  const auto processor_count = std::thread::hardware_concurrency();
  LOG.info() << "Number of native threads: " << processor_count;

  const auto default_concurrency = tbb::info::default_concurrency();
  LOG.info() << "TBB default concurrency: " << default_concurrency;

  tbb::global_control control(tbb::global_control::max_allowed_parallelism, processor_count);
}

int simpleSum(const int& first_number, const int& last_number) {

  int sum = 0;

  for (auto i = first_number; i < last_number; i++) {
    sum += i;
  }

  return (sum);
}

int parallelSum(const int& first_number, const int& last_number) {

  int sum = tbb::parallel_reduce(
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

  return (sum);
}

class Tbb : public Program {

public:
  ///
  ExitCode mainMethod(ELEMENTS_UNUSED std::map<std::string, VariableValue>& args) override {

    namespace chrono = std::chrono;

    initTbb();

    using clock = chrono::high_resolution_clock;
    using ms    = chrono::microseconds;

    const auto start = clock::now();

    const int first_number = 1;
    const int last_number  = 10001;

    const auto simple_sum = simpleSum(first_number, last_number);
    LOG.info() << "Simple sum: " << simple_sum;

    const auto simple_stop     = clock::now();
    const auto simple_duration = chrono::duration_cast<ms>(simple_stop - start);
    LOG.info() << "Simple duration: " << simple_duration.count();

    const auto parallel_sum = parallelSum(first_number, last_number);
    LOG.info() << "Parallel sum: " << parallel_sum;

    const auto parallel_stop     = clock::now();
    const auto parallel_duration = chrono::duration_cast<ms>(parallel_stop - simple_stop);
    LOG.info() << "Parallel duration: " << parallel_duration.count();

    const auto stop           = clock::now();
    const auto total_duration = chrono::duration_cast<ms>(stop - start);

    LOG.info() << "Total duration: " << total_duration.count();

    return (ExitCode::OK);
  }
};

}  // namespace Examples
}  // namespace Elements

MAIN_FOR(Elements::Examples::Tbb)
