// Copyright (C) 2012-2024, UNIGE and contributors (for the Euclid Science Ground Segment)
// This file is part of Elements <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements>
// SPDX-License-Identifier: LGPL-3.0-or-later

/// @file ElementsKernel/Threading.h
/// @brief Utilities for the Threading Building Blocks (TBB)
/// @date 2024-10-15
/// @author Hubert Degaudenzi

/// @addtogroup ElementsKernel ElementsKernel
/// @{

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_THREADING_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_THREADING_H_

#include "ElementsKernel/Export.h"  // for ELEMENTS_API
#include <cstddef>                  // for size_t
#include <string>                   // for string

#include <oneapi/tbb.h>  // for the Threading Building Blocks

namespace Elements {
inline namespace Kernel {
namespace Threading {

/// @brief Environment variable containing the default number of threads
/// @ingroup ElementsKernel
ELEMENTS_API extern const std::string ENV_VAR;

/// @brief Default number of cores.
/// @details 0: get the number from the system
/// @ingroup ElementsKernel
ELEMENTS_API extern const std::size_t DEFAULT_CPU_CORES;

/// @brief Function to initialize the OneAPI Threading Building Blocks
///
/// @param var Environment variable to look for the thread numbers. Its default value is
///        stored in #ENV_VAR.
/// @param default_cpu_cores default number of cores if var cannot be found in the environment.
///        Its default value is stored in #DEFAULT_CPU_CORES
///
/// @details to initialize the number of threads, this function looks first in the number
///          stored in the var environment variable. If it can't be found, it falls
///          back on the default_cpu_cores value. Finally, if that value is 0, it gets
///          the default number of cores from the system.
///
/// @ingroup ElementsKernel
ELEMENTS_API oneapi::tbb::global_control initBuildingBlocks(const std::string& var               = ENV_VAR,
                                                            const std::size_t& default_cpu_cores = DEFAULT_CPU_CORES);

/// @example ElementsExamples/src/program/Tbb.cpp
/// This is an example of how to use the ElementsKernel/Threading.h module.

}  // namespace Threading
}  // namespace Kernel
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_THREADING_H_

/// @}
