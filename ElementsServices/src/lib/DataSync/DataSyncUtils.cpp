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

#include "ElementsServices/DataSync/DataSyncUtils.h"  // for path, checkCall, confFilePath, containsInThisOrder, createLocalDirOf, environmentVariable, getWorkdirVariable, localDirExists, localWorkspacePrefix, lower, runCommandAndCaptureOutErr

#include <algorithm>  // for transform
#include <array>      // for array
#include <cctype>     // for tolower
#include <cstdio>     // for fgets, popen, BUFSIZ, pclose, FILE
#include <cstdlib>    // for system
#include <memory>     // for allocator, shared_ptr
#include <stdexcept>  // for runtime_error
#include <string>     // for string, operator+, basic_string, char_traits
#include <utility>    // for make_pair, pair
#include <vector>     // for vector

#include <boost/filesystem/operations.hpp>  // for create_directories, is_directory

#include "ElementsKernel/Configuration.h"  // for getPath
#include "ElementsKernel/Environment.h"    // for Environment

namespace Elements {
inline namespace Services {
namespace DataSync {

using std::string;

const string DEFAULT_WORKDIR_VAR{"WORKSPACE"};

const string WORKDIR_VAR_VAR{"DATASYNC_WORKDIR_VAR"};

path confFilePath(const path& filename) {
  return Configuration::getPath(filename);
}

bool checkCall(const string& command) {
  const string silent_command = command + " > /dev/null";
  const int    status         = std::system(silent_command.c_str());
  return status == 0;
}

std::pair<string, string> runCommandAndCaptureOutErr(const string& command) {
  string                      out;
  string                      err;
  std::array<char, BUFSIZ>    buffer{};
  const std::shared_ptr<FILE> command_pipe(popen(command.c_str(), "r"), pclose);
  if (not command_pipe) {
    throw std::runtime_error(string("Unable to run command: ") + command);
  }
  if (fgets(buffer.data(), BUFSIZ, command_pipe.get()) != nullptr) {
    out += buffer.data();
  }
  // @TODO get standard error
  return std::make_pair(out, err);
}

bool localDirExists(const path& local_dir) {
  return is_directory(local_dir);
}

void createLocalDirOf(const path& local_file) {
  if (not local_file.has_parent_path()) {
    return;
  }
  if (const path dir = local_file.parent_path(); not localDirExists(dir)) {
    create_directories(dir);
  }
}

string environmentVariable(const string& name) {
  return Environment::get(name);  // Already returns "" if not found
}

string getWorkdirVariable() {

  string workdir_variable = DEFAULT_WORKDIR_VAR;

  if (Environment current; not current[WORKDIR_VAR_VAR].empty()) {
    workdir_variable = current[WORKDIR_VAR_VAR];
  }

  return workdir_variable;
}

path localWorkspacePrefix() {
  const string  workdir_variable = getWorkdirVariable();
  const string& codeen_prefix(workdir_variable);
  const string& prefix_env_variable(codeen_prefix);
  return path{environmentVariable(prefix_env_variable)};
}

string lower(string text) {
  string uncased(text);
  std::transform(text.begin(), text.end(), uncased.begin(), ::tolower);
  return uncased;
}

bool containsInThisOrder(const string& input, const std::vector<string>& substrings) {
  string::size_type offset(0);
  for (const auto& substr : substrings) {
    offset = input.find(substr, offset);
    if (offset == string::npos) {
      return false;
    }
  }
  return true;
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
