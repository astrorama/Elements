// Copyright (C) 2012-2024, UNIGE and contributors (for the Euclid Science Ground Segment)
// This file is part of Elements <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements>
// SPDX-License-Identifier: LGPL-3.0-or-later

/// @file EnableGMock.cpp
/// @brief Insantiation of the global fixture
/// @date 2024-06-28
/// @author Hubert Degaudenzi

#include "ElementsKernel/EnableGMock.h"

#include <boost/test/unit_test.hpp>

#if BOOST_VERSION >= 105900
BOOST_GLOBAL_FIXTURE(GoogleMockSetupFixture);
#else
BOOST_GLOBAL_FIXTURE(GoogleMockSetupFixture)
#endif
