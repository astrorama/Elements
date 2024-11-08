// Copyright (C) 2012-2024, UNIGE and contributors (for the Euclid Science Ground Segment)
// This file is part of Elements <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements>
// SPDX-License-Identifier: LGPL-3.0-or-later

/// @file Threading.cpp
/// @brief Add a description
/// @date 2024-10-15
/// @author Hubert Degaudenzi

#include "ElementsKernel/Threading.h"

#include <cstddef>  // for size_t
#include <string>

#include <oneapi/tbb.h>  // for the Threading Building Blocks

#include "ElementsKernel/Environment.h"  // for Environment
#include "ElementsKernel/Logging.h"      // for getLogger

using std::size_t;
using std::string;

namespace Elements {

namespace {
auto LOG = Logging::getLogger("Elements");
}

inline namespace Kernel {
namespace Threading {

namespace tbb = oneapi::tbb;

const string ENV_VAR{"PIPELINE_CPU_CORES"};

const size_t DEFAULT_CPU_CORES{0};

tbb::global_control initBuildingBlocks(const std::string& var, const std::size_t& default_cpu_cores) {

  Environment env;

  size_t concurrency{0};

  if (env[var].exists()) {
    auto var_concurrency = std::stoi(env[var]);
    if (var_concurrency >= 0) {
      concurrency = static_cast<size_t>(var_concurrency);
    }
  }

  if (concurrency == 0) {
    concurrency = default_cpu_cores;
  }

  if (concurrency == 0) {
    const auto default_concurrency = tbb::info::default_concurrency();
    if (default_concurrency >= 0) {
      concurrency = static_cast<size_t>(default_concurrency);
    }
  }

  tbb::global_control control(tbb::global_control::max_allowed_parallelism, concurrency);
  LOG.debug() << "Set the TBB max allowed parallelism to" << concurrency;

  return (control);
}

}  // namespace Threading
}  // namespace Kernel
}  // namespace Elements
