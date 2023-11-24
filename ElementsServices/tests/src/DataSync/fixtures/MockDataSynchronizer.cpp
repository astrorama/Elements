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

MockDataSynchronizer::MockDataSynchronizer(DataSync::path connection, path dependency)
    : DataSync::DataSynchronizer(
          DataSync::ConnectionConfiguration(connection),
          DataSync::DependencyConfiguration(theWebdavFrDistantWorkspace(), theLocalWorkspace(), dependency)) {}

string MockDataSynchronizer::createDownloadCommand(path distantFile, path localFile) const {
  string cmd = string("echo ") + distantFile.string() + "\t" + localFile.string();
  return cmd;
}

std::map<path, path> MockDataSynchronizer::fileMap() {
  return m_fileMap;
}
