/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include "MockDataSynchronizer.h"

#include <map>     // for map
#include <string>  // for allocator, operator+, char_traits, string

#include "ElementsServices/DataSync/ConnectionConfiguration.h"  // for ConnectionConfiguration
#include "ElementsServices/DataSync/DataSynchronizer.h"         // for DataSynchronizer
#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration
#include "tests/src/DataSync/fixtures/ConfigFilesFixture.h"     // for theLocalWorkspace, theWebdavFrDistantWorkspace

namespace DataSync = ElementsServices::DataSync;

using DataSync::path;
using std::string;

MockDataSynchronizer::MockDataSynchronizer(const path& connection, const path& dependency)
    : DataSynchronizer(
          DataSync::ConnectionConfiguration(connection),
          DataSync::DependencyConfiguration(theWebdavFrDistantWorkspace(), theLocalWorkspace(), dependency)) {}

string MockDataSynchronizer::createDownloadCommand(const path distant_file, const path local_file) const {
  string cmd = string("echo ") + distant_file.string() + "\t" + local_file.string();
  return cmd;
}

std::map<path, path> MockDataSynchronizer::fileMap() {
  return m_fileMap;
}
