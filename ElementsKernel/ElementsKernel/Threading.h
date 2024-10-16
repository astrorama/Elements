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

namespace Elements {
inline namespace Kernel {
namespace Threading {

/// @brief Environment variable containing the default number of threads
/// @ingroup ElementsKernel
ELEMENTS_API extern const std::string ENV_VAR;

/// @brief Default number of cores.
///        - 0: get the number from the system
/// @ingroup ElementsKernel
ELEMENTS_API extern const std::size_t DEFAULT_CPU_CORE;

}  // namespace Threading
}  // namespace Kernel
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_THREADING_H_

/// @}
