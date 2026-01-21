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

#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration

#include <cstddef>  // for size_t
#include <fstream>  // for basic_istream, ifstream
#include <map>      // for map, operator!=, _Rb_tree_const_iterator
#include <string>   // for char_traits, string, getline
#include <utility>  // for pair
#include <vector>   // for vector

#include "ElementsServices/DataSync/DataSyncUtils.h"  // for path, confFilePath

namespace Elements {
inline namespace Services {
namespace DataSync {

using std::string;
using std::vector;

DependencyConfiguration::DependencyConfiguration(path distantRoot, path localRoot, const path& configFile)
    : m_aliasSeparator('\t'), m_distantRoot(std::move(distantRoot)), m_localRoot(std::move(localRoot)) {
  parseConfigurationFile(configFile);
}

std::map<path, path> DependencyConfiguration::fileMap() const {
  return m_fileMap;
}

path DependencyConfiguration::distantPathOf(const path& localFile) const {
  return m_fileMap.at(localFile);  // @TODO error handling
}

std::size_t DependencyConfiguration::dependencyCount() const {
  return m_fileMap.size();
}

vector<path> DependencyConfiguration::distantPaths() const {
  vector<path> distant_paths;
  for (const auto& [local, distant] : m_fileMap) {
    distant_paths.emplace_back(distant);
  }
  return distant_paths;
}

vector<path> DependencyConfiguration::localPaths() const {
  vector<path> local_paths;
  for (const auto& [local, distant] : m_fileMap) {
    local_paths.emplace_back(local);
  }
  return local_paths;
}

void DependencyConfiguration::parseConfigurationFile(const path& filename) {
  const path    abs_path = confFilePath(filename);
  std::ifstream inputStream(abs_path.c_str());
  string        line;
  while (std::getline(inputStream, line)) {
    parseConfigurationLine(line);
  }
}

void DependencyConfiguration::parseConfigurationLine(const string& line) {
  if (lineHasAlias(line)) {
    parseLineWithAlias(line);
  } else {
    parseLineWithoutAlias(line);
  }
}

char DependencyConfiguration::aliasSeparator() const {
  return m_aliasSeparator;
}

bool DependencyConfiguration::lineHasAlias(const string& line) const {
  const string::size_type offset = line.find(m_aliasSeparator);
  return offset != string::npos;
}

void DependencyConfiguration::parseLineWithAlias(const string& line) {
  const string::size_type offset          = line.find(m_aliasSeparator);
  const string            distantFilename = line.substr(0, offset);
  const string            localFilename   = line.substr(offset + 1);
  const path              distantPath     = m_distantRoot / distantFilename;
  const path              localPath       = m_localRoot / localFilename;
  m_fileMap[localPath]                    = distantPath;
}

void DependencyConfiguration::parseLineWithoutAlias(const string& line) {
  const path distantPath = m_distantRoot / line;
  const path localPath   = m_localRoot / line;
  m_fileMap[localPath]   = distantPath;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
