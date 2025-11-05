/**
 * @file ModuleInfo.cpp
 * @brief OS specific details to access at run-time the module
 * configuration of the process.
 * @date 2014-12-01
 * @author hubert
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
 */

#include "ElementsKernel/ModuleInfo.h"

#include <climits>   // for PATH_MAX
#include <dlfcn.h>   // for dladdr, Dl_info, (anonymous), dlopen, dlsym, RTLD_LAZY
#include <libgen.h>  // for __xpg_basename, basename
#include <unistd.h>  // for getpid

#include <array>       // for array
#include <cstdint>     // for int64_t
#include <filesystem>  // for canonical, exists, path
#include <fstream>     // IWYU pragma: keep
#include <memory>
#include <sstream>  // for basic_istream, basic_ostream, basic_ostream::operator<<, operator<<, basic_ios, ifstream, istringstream, stringstream
#include <string>   // for char_traits, basic_string, string, operator>>, getline, operator==
#include <vector>   // for vector

#include "ElementsKernel/FuncPtrCast.h"  // for FuncPtrCast
#include "ElementsKernel/Path.h"         // for Item

#ifdef __APPLE__
#include <climits>        // for PATH_MAX
#include <mach-o/dyld.h>  // for _NSGetExecutablePath
#endif

using std::string;
using std::vector;

namespace {
vector<string> s_linkedModules;
}

namespace Elements::System {

ModuleInfo::ModuleInfo() : m_dlinfo{nullptr} {}

ModuleInfo::ModuleInfo(void* funct) {
  m_dlinfo = std::make_unique<Dl_info>();
  dladdr(FuncPtrCast<void*>(funct), m_dlinfo.get());
}

string ModuleInfo::name() const {
  return ::basename(const_cast<char*>(m_dlinfo->dli_fname));
}

string ModuleInfo::libraryName() const {
  return const_cast<char*>(m_dlinfo->dli_fname);
}

const void* ModuleInfo::addresse() const {
  return m_dlinfo->dli_saddr;
}

bool ModuleInfo::isEmpty() const {
  return m_dlinfo == nullptr;
}

ModuleInfo::operator const Dl_info&() const {
  return *m_dlinfo;
}

namespace {
ImageHandle s_module_handle = nullptr;
}
/// Retrieve base name of module
const string& moduleName() {
  static string module{};
  if (module.empty()) {
    if (processHandle() != nullptr && moduleHandle() != nullptr) {
      string mod = ::basename(const_cast<char*>(static_cast<Dl_info*>(moduleHandle())->dli_fname));
      module     = mod.substr(0, mod.find('.'));
    }
  }
  return module;
}

/// Retrieve full name of module
const string& moduleNameFull() {
  static string module{};
  if (module.empty()) {
    if (processHandle() and moduleHandle()) {
      std::array<char, PATH_MAX> name{"Unknown.module"};
      name[0] = 0;
      if (const char* path = static_cast<Dl_info*>(moduleHandle())->dli_fname; realpath(path, name.data())) {
        module = string(name.data());
      }
    }
  }
  return module;
}

/// Get type of the module
ModuleType moduleType() {
  static auto type = ModuleType::UNKNOWN;
  if (type == ModuleType::UNKNOWN) {
    const string& module = moduleNameFull();
    if (const std::size_t loc = module.rfind('.') + 1; loc == 0) {
      type = ModuleType::EXECUTABLE;
    } else if (module[loc] == 'e' or module[loc] == 'E') {
      type = ModuleType::EXECUTABLE;
    } else if (module[loc] == 's' and module[loc + 1] == 'o') {
      type = ModuleType::SHAREDLIB;
    } else {
      type = ModuleType::UNKNOWN;
    }
  }
  return type;
}

/// Retrieve process handle
void* processHandle() {
  static std::int64_t pid = getpid();
  static auto         hP  = reinterpret_cast<void*>(pid);
  return hP;
}

void setModuleHandle(const void* const handle) {
  s_module_handle = const_cast<ImageHandle>(handle);
}

ImageHandle moduleHandle() {
  if (nullptr == s_module_handle) {
    if (processHandle() != nullptr) {
      static Dl_info info;
      if (0 != dladdr(FuncPtrCast<void*>(moduleHandle), &info)) {
        return &info;
      }
    }
  }
  return s_module_handle;
}

ImageHandle exeHandle() {
  // This does NOT work!
  static Dl_info  infoBuf;
  static Dl_info* info;

  if (nullptr == info) {
    if (void* handle = dlopen(nullptr, RTLD_LAZY); nullptr != handle) {
      if (const void* func = dlsym(handle, "main"); nullptr != func) {
        if (0 != dladdr(func, &infoBuf)) {
          info = &infoBuf;
        }
      }
    }
  }
  return info;
}

const string& exeName() {
  static string module;
  if (module.empty()) {
    module = getExecutablePath().string();
  }
  return module;
}

Path::Item getSelfProc() {

  Path::Item self_proc{"/proc/self"};

  if (const Path::Item exe = self_proc / "exe"; not exists(exe)) {
    std::stringstream self_str{};
    self_str << "/proc/" << getpid();
    self_proc = Path::Item(self_str.str());
  }

  return self_proc;
}

vector<Path::Item> linkedModulePaths() {

  vector<Path::Item> linked_modules;

  Path::Item    self_maps = getSelfProc() / "maps";
  std::ifstream maps_str(self_maps.string());

  string line;
  while (std::getline(maps_str, line)) {
    string             address;
    string             perms;
    string             offset;
    string             dev;
    string             pathname;
    unsigned           inode;
    std::istringstream iss(line);
    if (not(iss >> address >> perms >> offset >> dev >> inode >> pathname)) {
      continue;
    }
    if (perms == "r-xp" and std::filesystem::exists(pathname)) {
      linked_modules.emplace_back(pathname);
    }
  }

  maps_str.close();

  return linked_modules;
}

vector<string> linkedModules() {

  if (s_linkedModules.empty()) {

    for (const auto& m : linkedModulePaths()) {
      s_linkedModules.emplace_back(m.string());
    }
  }
  return s_linkedModules;
}

Path::Item getExecutablePath() {

#ifdef __APPLE__
  path         self_proc{};
  char         pathbuf[PATH_MAX + 1];
  unsigned int bufsize = sizeof(pathbuf);
  _NSGetExecutablePath(pathbuf, &bufsize);
  path self_exe = path(string(pathbuf));
#else

  Path::Item self_exe = getSelfProc() / "exe";

#endif

  return canonical(self_exe);
}

}  // namespace Elements::System
