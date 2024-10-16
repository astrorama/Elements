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

using std::size_t;
using std::string;

namespace Elements {
inline namespace Kernel {
namespace Threading {

const string ENV_VAR{"PIPELINE_CPU_CORES"};

const size_t DEFAULT_CPU_CORE{0};

}  // namespace Threading
}  // namespace Kernel
}  // namespace Elements
