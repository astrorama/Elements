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

#include "ElementsServices/DataSync/DataSynchronizer.h"  // for DataSynchronizer, DownloadFailed

#include <map>      // for map, operator!=, _Rb_tree_const_iterator
#include <string>   // for string
#include <utility>  // for pair

#include <boost/filesystem/operations.hpp>  // for is_regular_file, file_size

#include "ElementsKernel/Unused.h"  // for ELEMENTS_UNUSED

#include "ElementsServices/DataSync/ConnectionConfiguration.h"  // for ConnectionConfiguration
#include "ElementsServices/DataSync/DataSyncUtils.h"  // for path, createLocalDirOf, runCommandAndCaptureOutErr
#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration

namespace Elements {
inline namespace Services {
namespace DataSync {

DataSynchronizer::DataSynchronizer(const ConnectionConfiguration& connection, const DependencyConfiguration& dependency)
    : m_connection(connection), m_fileMap(dependency.fileMap()) {}

void DataSynchronizer::downloadAllFiles() const {
  for (const auto& [fst, snd] : m_fileMap) {
    const auto& localFile   = fst;
    const auto& distantFile = snd;
    if (fileShouldBeWritten(localFile)) {
      downloadOneFile(distantFile, localFile);
    }
  }
}

bool DataSynchronizer::fileShouldBeWritten(const path& localFile) const {
  if (not fileAlreadyExists(localFile)) {
    return true;
  }
  return m_connection.overwritingAllowed();
}

bool DataSynchronizer::fileAlreadyExists(const path& localFile) {
  return is_regular_file(localFile);
}

void DataSynchronizer::downloadOneFile(const path& distantFile, const path& localFile) const {
  const std::string command = createDownloadCommand(distantFile, localFile);
  createLocalDirOf(localFile);
  const auto outErr = runCommandAndCaptureOutErr(command);
  if (not hasBeenDownloaded(distantFile, localFile)) {
    throw DownloadFailed(distantFile, localFile);
  }
}

bool DataSynchronizer::hasBeenDownloaded(ELEMENTS_UNUSED const path& distantFile, const path& localFile) {
  if (not is_regular_file(localFile)) {
    return false;
  }
  return file_size(localFile) > 0;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
