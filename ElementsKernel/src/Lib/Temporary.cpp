/**
 * @file Temporary.cpp
 * @brief Implementation of the Temporary classes
 * @date May 27, 2014
 * @author hubert degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "ElementsKernel/Temporary.h"

#include <filesystem>  // for operator/, ofstream, path>
#include <fstream>     // for ofstream>
#include <string>      // for string

#include <utility>

#include <boost/filesystem/operations.hpp>  // for remove_all, create_directory

#include "ElementsKernel/Environment.h"  // for Environment
#include "ElementsKernel/Logging.h"      // for Logging
#include "ElementsKernel/Path.h"         // for Item

using std::string;
using std::filesystem::temp_directory_path;

namespace Elements {

namespace {
auto log = Logging::getLogger();
}

TempPath::TempPath(string motif, string keep_var)
    : m_motif(std::move(motif)), m_path(temp_directory_path()), m_keep_var(std::move(keep_var)) {

  using boost::filesystem::unique_path;

  if (m_motif.find('%') == string::npos) {
    log.error() << "The '" << m_motif << "' motif is not random";
  }

  auto pattern = m_motif;

  if (pattern.empty()) {
    log.warn() << "The motif has been replaced by \"" << DEFAULT_TMP_MOTIF << "\"";
    pattern = DEFAULT_TMP_MOTIF;
  }

  m_path /= Path::uniquePath(pattern);
}

TempPath::~TempPath() {

  if (Environment current; not Environment::hasKey(m_keep_var)) {
    log.debug() << "Automatic destruction of the " << path() << " temporary path";
    const auto file_number = remove_all(m_path);
    log.debug() << "Number of files removed: " << file_number;
  } else {
    log.info() << m_keep_var << " set: I do not remove the " << m_path.string() << " temporary path";
  }
}

Path::Item TempPath::path() const {
  return m_path;
}

string TempPath::motif() const {
  return m_motif;
}

TempDir::TempDir(const string& motif, const string& keep_var) : TempPath(motif, keep_var) {

  log.debug() << "Creation of the " << path() << " temporary directory";

  create_directory(path());
}

TempDir::~TempDir() = default;

TempFile::TempFile(const string& motif, const string& keep_var) : TempPath(motif, keep_var) {

  log.debug() << "Creation of the " << path() << " temporary file";

  std::ofstream ofs(path());
  ofs.close();
}

TempFile::~TempFile() = default;

}  // namespace Elements
