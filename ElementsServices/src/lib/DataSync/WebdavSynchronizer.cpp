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

#include "ElementsServices/DataSync/WebdavSynchronizer.h"  // for WebdavSynchronizer, webdavIsInstalled

#include <stdexcept>  // for runtime_error
#include <string>     // for allocator, operator+, char_traits, to_string, string

#include "ElementsServices/DataSync/ConnectionConfiguration.h"  // for ConnectionConfiguration
#include "ElementsServices/DataSync/DataSyncUtils.h"            // for checkCall, path
#include "ElementsServices/DataSync/DataSynchronizer.h"         // for DataSynchronizer
#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration

namespace Elements {
inline namespace Services {
namespace DataSync {

bool webdavIsInstalled() {
  return checkCall("wget -h");
}

WebdavSynchronizer::WebdavSynchronizer(const ConnectionConfiguration& connection,
                                       const DependencyConfiguration& dependency)
    : DataSynchronizer(connection, dependency) {
  if (not webdavIsInstalled()) {
    throw std::runtime_error("You are trying to use WebDAV, "
                             "but it does not seem to be installed.");
  }
}

std::string WebdavSynchronizer::createDownloadCommand(path distantFile, path localFile) const {
  std::string cmd = "wget --no-check-certificate ";
  cmd += " --user=" + m_connection.user;
  cmd += " --password=" + m_connection.password;
  cmd += " -O " + localFile.string();
  cmd += " " + m_connection.hostUrl + "/" + distantFile.string();
  cmd += " --tries " + std::to_string(m_connection.tries);
  return cmd;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
