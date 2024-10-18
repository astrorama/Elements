/**
 * @file Temporary.cpp
 *
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

#include <string>  // for string

#include <boost/filesystem/fstream.hpp>     // for fstream
#include <boost/filesystem/operations.hpp>  // for create_directory, remove_all, temp_directory_path, unique_path
#include <boost/filesystem/path.hpp>        // for operator<<

#include "ElementsKernel/Environment.h"  // for Environment
#include "ElementsKernel/Logging.h"      // for Logging
#include "ElementsKernel/Path.h"         // for Item

using boost::filesystem::temp_directory_path;
using std::string;

namespace Elements {

namespace {
auto LOG = Logging::getLogger("Elements");
}

TempPath::TempPath(const string& arg_motif, const string& keep_var)
    : m_motif(arg_motif), m_path(temp_directory_path()), m_keep_var(keep_var) {

  using boost::filesystem::unique_path;

  if (m_motif.find('%') == string::npos) {
    LOG.error() << "The '" << m_motif << "' motif is not random";
  }

  auto pattern = m_motif;

  if (pattern.empty()) {
    LOG.warn() << "The motif has been replaced by \"" << DEFAULT_TMP_MOTIF << "\"";
    pattern = DEFAULT_TMP_MOTIF;
  }

  m_path /= unique_path(pattern);
}

TempPath::~TempPath() {

  Environment current;

  if (not current.hasKey(m_keep_var)) {
    LOG.debug() << "Automatic destruction of the " << path() << " temporary path";
    const auto file_number = boost::filesystem::remove_all(m_path);
    LOG.debug() << "Number of files removed: " << file_number;
  } else {
    LOG.info() << m_keep_var << " set: I do not remove the " << m_path.string() << " temporary path";
  }
}

Path::Item TempPath::path() const {
  return m_path;
}

string TempPath::motif() const {
  return m_motif;
}

TempDir::TempDir(const string& arg_motif, const string& keep_var) : TempPath(arg_motif, keep_var) {

  LOG.debug() << "Creation of the " << path() << " temporary directory";

  boost::filesystem::create_directory(path());
}

TempDir::~TempDir() = default;

TempFile::TempFile(const string& arg_motif, const string& keep_var) : TempPath(arg_motif, keep_var) {

  LOG.debug() << "Creation of the " << path() << " temporary file";

  boost::filesystem::ofstream ofs(path());
  ofs.close();
}

TempFile::~TempFile() = default;

}  // namespace Elements
