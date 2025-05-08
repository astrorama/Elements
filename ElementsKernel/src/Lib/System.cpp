/**
 * @file System.cpp
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2023 Euclid Science Ground Segment
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

#include "ElementsKernel/System.h"

#include <cxxabi.h>       // for __cxa_demangle
#include <dlfcn.h>        // for dladdr, dlclose, dlerror, dlopen, dlsym, Dl_info, RTLD_GLOBAL, RTLD_LAZY
#include <execinfo.h>     // for backtrace
#include <sys/utsname.h>  // for uname, utsname
#include <unistd.h>       // for gethostname, environ

#include <array>     // for array
#include <cerrno>    // for errno
#include <cstdlib>   // for free, getenv, setenv, unsetenv, size_t
#include <cstring>   // for strerror, strnlen, size_t
#include <iomanip>   // for operator<<, setiosflags, setw
#include <iostream>  // for basic_ostream, operator<<, basic_ostream::operator<<, dec, hex, ios, ostringstream
#include <new>       // for nothrow
#include <sstream>   // for basic_ostringstream
#include <string>    // for string, char_traits, basic_string, operator<<, operator+
#include <typeinfo>  // for type_info
#include <vector>    // for vector

#include "ElementsKernel/FuncPtrCast.h"  // for FuncPtrCast
#include "ElementsKernel/Unused.h"       // for ELEMENTS_UNUSED

#include <map>

using std::size_t;
using std::string;
using std::vector;

namespace Elements::System {

// --------------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------------

namespace {

unsigned long doLoad(const string& name, ImageHandle* handle) {
  void* mh = dlopen(name.empty() ? nullptr : name.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  *handle  = mh;
  if (nullptr == *handle) {
    return getLastError();
  }
  return 1;
}

unsigned long loadWithoutEnvironment(const string& name, ImageHandle* handle) {

  string       dll_name = name;
  const size_t dll_len  = dll_name.size();
  const size_t suf_len  = SHLIB_SUFFIX.size();

  // Add the suffix at the end of the library name only if necessary
  if (dll_len >= suf_len && dll_name.compare(dll_len - suf_len, suf_len, SHLIB_SUFFIX) != 0) {
    dll_name += SHLIB_SUFFIX;
  }

  // Load the library
  return doLoad(dll_name, handle);
}

}  // anonymous namespace
// --------------------------------------------------------------------------------------

/// Load dynamic link library
unsigned long loadDynamicLib(const string& name, ImageHandle* handle) {
  unsigned long res;
  // if name is empty, just load it
  if (name.empty()) {
    res = loadWithoutEnvironment(name, handle);
  } else {
    // If the name is a logical name (environment variable), the try
    // to load the corresponding library from there.
    string imgName;
    if (getEnv(name, imgName)) {
      res = loadWithoutEnvironment(imgName, handle);
    } else {
      // build the dll name
      string dllName = name;
      dllName        = "lib" + dllName;
      dllName += SHLIB_SUFFIX;
      // try to locate the dll using the standard PATH
      res = loadWithoutEnvironment(dllName, handle);
    }
    if (res != 1) {
      errno = static_cast<int>(0xAFFEDEAD);
    }
  }
  return res;
}

/// unload dynamic link library
unsigned long unloadDynamicLib(void* const handle) {
  dlclose(handle);
  return 1;
}

/// Get a specific function defined in the DLL
unsigned long getProcedureByName(ImageHandle handle, const string& name, EntryPoint* pFunction) {
#if defined(__linux__)
  *pFunction = FuncPtrCast<EntryPoint>(dlsym(handle, name.c_str()));
  if (nullptr == *pFunction) {
    errno = static_cast<int>(0xAFFEDEAD);
    return 0;
  }
#elif defined(__APPLE__)
  *pFunction = (EntryPoint)::dlsym(handle, name.c_str());
  if (not*pFunction) {
    // Try with an underscore :
    string sname = "_" + name;
    *pFunction   = (EntryPoint)::dlsym(handle, sname.c_str());
  }
  if (0 == *pFunction) {
    errno = static_cast<int>(0xAFFEDEAD);
    std::cout << "Elements::System::getProcedureByName>" << getLastErrorString() << std::endl;
    return 0;
  }
#endif
  return 1;
}

/// Get a specific function defined in the DLL
unsigned long getProcedureByName(void* const handle, const string& name, Creator* pFunction) {
  return getProcedureByName(handle, name, reinterpret_cast<EntryPoint*>(pFunction));
}

/// Retrieve last error code
unsigned long getLastError() {
  // convert errno (int) to unsigned long
  return static_cast<unsigned int>(errno);
}

/// Retrieve last error code as string
string getLastErrorString() {
  const string errString = getErrorString(getLastError());
  return errString;
}

/// Retrieve error code as string for a given error
string getErrorString(const unsigned long error) {
  string errString;
  char*  cerrString(nullptr);
  // Remember: for linux dl* routines must be handled differently!
  if (error == 0xAFFEDEAD) {
    cerrString = dlerror();
    if (nullptr == cerrString) {
      cerrString = std::strerror(static_cast<int>(error));
    }
    if (nullptr == cerrString) {
      cerrString = const_cast<char*>("Unknown error. No information found in strerror()!");
    }
    errString = string(cerrString);
    errno     = 0;
  } else {
    cerrString = std::strerror(static_cast<int>(error));
    errString  = string(cerrString);
  }
  return errString;
}

string typeinfoName(const std::type_info& tinfo) {
  return typeinfoName(tinfo.name());
}

string typeinfoName(const char* class_name) {
  string                       result{class_name};
  const std::map<char, string> type_initials = {{'v', "void"},        {'w', "wchar_t"},
                                                {'b', "bool"},        {'c', "char"},
                                                {'a', "signed char"}, {'h', "unsigned char"},
                                                {'s', "short"},       {'t', "unsigned short"},
                                                {'i', "int"},         {'j', "unsigned int"},
                                                {'l', "long"},        {'m', "unsigned long"},
                                                {'x', "long long"},   {'y', "unsigned long long"},
                                                {'n', "__int128"},    {'o', "unsigned __int128"},
                                                {'f', "float"},       {'d', "double"},
                                                {'e', "long double"}, {'g', Float128_str},
                                                {'z', "ellipsis"}};
  if (strnlen(class_name, 1024) == 1) {
    // See http://www.realitydiluted.com/mirrors/reality.sgi.com/dehnert_engr/cxx/abi.pdf
    // for details

    if (const auto key = class_name[0]; type_initials.find(key) != type_initials.end()) {
      result = type_initials.at(key);
    }

  } else {
    int                                          status;
    const std::unique_ptr<char, decltype(free)*> real_name(abi::__cxa_demangle(class_name, nullptr, nullptr, &status),
                                                           free);
    if (real_name == nullptr) {
      return class_name;
    }
    result = real_name.get();
    /// substitute ', ' with ','
    string::size_type pos = result.find(", ");
    while (string::npos != pos) {
      result.replace(pos, 2, ",");
      pos = result.find(", ");
    }
  }
  return result;
}

/// Host name
const string& hostName() {
  static string host{};
  if (host.empty()) {
    std::array<char, HOST_NAME_MAX + 1> buffer{};
    gethostname(buffer.data(), HOST_NAME_MAX);
    host = string(buffer.data());
  }
  return host;
}

/// OS name
const string& osName() {
  static string osname;
  utsname       ut{};
  if (uname(&ut) == 0) {
    osname = ut.sysname;
  } else {
    osname = "UNKNOWN";
  }
  return osname;
}

/// OS version
const string& osVersion() {
  static string osver = "UNKNOWN";
  utsname       ut{};

  if (uname(&ut) == 0) {
    osver = ut.release;
  }

  return osver;
}

/// Machine type
const string& machineType() {
  static string mach = "UNKNOWN";
  utsname       ut{};

  if (uname(&ut) == 0) {
    mach = ut.machine;
  }

  return mach;
}

string getEnv(const string& var) {

  string env_str{};

  getEnv(var, env_str);

  return env_str;
}

/// get a particular env var, storing the value in the passed string (if set)
bool getEnv(const string& variable_name, string& variable_value) {
  bool found     = false;
  variable_value = "";

  if (const char* env = getenv(variable_name.c_str()); env != nullptr) {
    found          = true;
    variable_value = env;
  }

  return found;
}

bool isEnvSet(const string& variable_name) {
  string result;
  return getEnv(variable_name, result);
}

/// get all defined environment vars
#if defined(__APPLE__)
// Needed for _NSGetEnviron(void)
#include "crt_externs.h"
#endif
vector<string> getEnv() {
#if defined(__APPLE__)
  static char** environ = *_NSGetEnviron();
#endif
  vector<string> vars;
  for (int i = 0; environ[i] != nullptr; ++i) {
    vars.emplace_back(environ[i]);
  }
  return vars;
}

/// set an environment variables. @return 0 if successful, -1 if not
int setEnv(const string& name, const string& value, const bool overwrite) {

  int over = 1;
  if (not overwrite) {
    over = 0;
  }

  return setenv(name.c_str(), value.c_str(), over);
}

int unSetEnv(const string& name) {
  return unsetenv(name.c_str());
}

// -----------------------------------------------------------------------------
// backtrace utilities
// -----------------------------------------------------------------------------
__attribute__((noinline)) int backTrace(ELEMENTS_UNUSED const std::shared_ptr<void*>& addresses,
                                        ELEMENTS_UNUSED const int                     depth) {

  if (const int count = backtrace(addresses.get(), depth); count > 0) {
    return count;
  } else {
    return 0;
  }
}

vector<string> backTrace(const int depth, const int offset) {

  // Always hide the first two levels of the stack trace (that's us)
  const int      total_offset = offset + STACK_OFFSET;
  const int      total_depth  = depth + total_offset;
  vector<string> trace{};

  const std::shared_ptr<void*> addresses{new (std::nothrow) void*[static_cast<std::size_t>(total_depth)],
                                         std::default_delete<void*[]>()};

  if (addresses != nullptr) {

    const int count = backTrace(addresses, total_depth);

    for (int i = total_offset; i < count; ++i) {
      string fnc;
      string lib;
      if (void* addr = nullptr; getStackLevel(addresses.get()[i], addr, fnc, lib)) {
        std::ostringstream ost;
        ost << "#" << std::setw(3) << std::setiosflags(std::ios::left) << i - total_offset + 1;
        ost << std::hex << addr << std::dec << " " << fnc << "  [" << lib << "]";
        trace.emplace_back(ost.str());
      }
    }
  }

  return trace;
}

bool getStackLevel(const void* addresses ELEMENTS_UNUSED, void*& addr ELEMENTS_UNUSED, string& fnc ELEMENTS_UNUSED,
                   string& lib ELEMENTS_UNUSED) {

  Dl_info info;

  if (dladdr(addresses, &info) && info.dli_fname && info.dli_fname[0] != '\0') {
    const char* symbol = info.dli_sname && info.dli_sname[0] != '\0' ? info.dli_sname : nullptr;

    lib  = info.dli_fname;
    addr = info.dli_saddr;

    if (symbol != nullptr) {
      int                                          stat;
      const std::unique_ptr<char, decltype(free)*> dmg(abi::__cxa_demangle(symbol, nullptr, nullptr, &stat), free);
      fnc = string(stat == 0 ? dmg.get() : symbol);
    } else {
      fnc = "local";
    }
    return true;
  } else {
    return false;
  }
}

}  // namespace Elements::System
