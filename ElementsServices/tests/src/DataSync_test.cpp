/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <exception>   // for exception
#include <filesystem>  // for is_regular_file

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "ElementsServices/DataSync.h"                     // for DataSync
#include "ElementsServices/DataSync/DataSyncUtils.h"       // for path, DataSync
#include "ElementsServices/DataSync/IrodsSynchronizer.h"   // for irodsIsInstalled
#include "ElementsServices/DataSync/WebdavSynchronizer.h"  // for webdavIsInstalled

#include "DataSync/fixtures/ConfigFilesFixture.h"  // for theDependencyConfig, theIrodsFrConfig, theLocalFiles, theWebdavFrConfig, aBadConnectionConfig

namespace DataSync = ElementsServices::DataSync;

using DataSync::path;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DataSync_test)

//-----------------------------------------------------------------------------

void checkDownload(const path& connection_config) {
  const auto sync = DataSync::DataSync(connection_config, theDependencyConfig());
  sync.download();
  for (const auto& file : theLocalFiles()) {
    const path abs_path = sync.absolutePath(file);
    BOOST_CHECK(std::filesystem::is_regular_file(abs_path));
    std::filesystem::remove(abs_path);
  }
}

void checkFallback(const path& fallback_config) {
  auto sync = DataSync::DataSync(aBadConnectionConfig(), theDependencyConfig());
  BOOST_CHECK_THROW(sync.download(), std::exception);
  sync.downloadWithFallback(fallback_config);
  for (const auto& file : theLocalFiles()) {
    const path abs_path = sync.absolutePath(file);
    BOOST_CHECK(std::filesystem::is_regular_file(abs_path));
    std::filesystem::remove(abs_path);
  }
}

BOOST_AUTO_TEST_CASE(dataSyncWebdavFr_test) {
  if (DataSync::webdavIsInstalled()) {
    checkDownload(theWebdavFrConfig());
    checkFallback(theWebdavFrConfig());
  }
}

BOOST_AUTO_TEST_CASE(dataSyncIrodsFr_test) {
  if (DataSync::irodsIsInstalled()) {
    checkDownload(theIrodsFrConfig());
    checkFallback(theIrodsFrConfig());
  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
