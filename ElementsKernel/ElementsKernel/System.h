/**
 * @file ElementsKernel/System.h
 *
 * @brief This file is intended to iron out all the
 *   differences between systems (currently Linux and MacOSX)
 *
 * @details All the compilation system dependent parts (\#if, \#ifdef etc)
 *   should be located in this file. This should clear the need of these
 *   entities in all other C++ files.
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
#ifndef ELEMENTSKERNEL_SYSTEM_H
#define ELEMENTSKERNEL_SYSTEM_H

// STL include files
#include <vector>
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>

// Framework include files
#include "ElementsKernel/Export.h" // ELEMENTS_API
#include "ElementsKernel/Unused.h" // ELEMENTS_UNUSED

namespace Elements {
namespace System {

/**
 * @brief name of the shared dynamic library path
 */
ELEMENTS_API extern const std::string SHLIB_VAR_NAME;

/**
 * @brief constant that represent the common prefix of the libraries
 */
ELEMENTS_API extern const std::string LIB_PREFIX;

/**
 * @brief constant that represent the common extension of the libraries
 */
ELEMENTS_API extern const std::string LIB_EXTENSION;

/**
 * @brief  constant that represents the standard suffix of
 *   the libraries: usually "."+LIB_EXTENSION
 */
ELEMENTS_API extern const std::string LIB_SUFFIX;

/**
 * @brief alias for LIB_SUFFIX
 */
ELEMENTS_API extern const std::string SHLIB_SUFFIX;

#if defined(__linux) || defined(__APPLE__)
#define TEMPLATE_SPECIALIZATION
#endif


/// Definition of an image handle
using ImageHandle = void*;
/// Definition of the process handle
using ProcessHandle = void*;
/// Definition of the "generic" DLL entry point function
using EntryPoint = unsigned long (*)(const unsigned long iid, void** ppvObject);
/// Definition of the "generic" DLL entry point function
using Creator = void* (*)();


/// Load dynamic link library
ELEMENTS_API unsigned long loadDynamicLib(const std::string& name,
    ImageHandle* handle);
/// unload dynamic link library
ELEMENTS_API unsigned long unloadDynamicLib(ImageHandle handle);
/// Get a specific function defined in the DLL
ELEMENTS_API unsigned long getProcedureByName(ImageHandle handle,
    const std::string& name, EntryPoint* pFunction);
/// Get a specific function defined in the DLL
ELEMENTS_API unsigned long getProcedureByName(ImageHandle handle,
    const std::string& name, Creator* pFunction);
/// Get last system known error
ELEMENTS_API unsigned long getLastError();
/// Get last system error as string
ELEMENTS_API const std::string getLastErrorString();
/// Retrieve error code as string for a given error
ELEMENTS_API const std::string getErrorString(unsigned long error);
/// Get platform independent information about the class type
ELEMENTS_API const std::string typeinfoName(const std::type_info&);
ELEMENTS_API const std::string typeinfoName(const char*);
/// Host name
ELEMENTS_API const std::string& hostName();
/// OS name
ELEMENTS_API const std::string& osName();
/// OS version
ELEMENTS_API const std::string& osVersion();
/// Machine type
ELEMENTS_API const std::string& machineType();
/// User login name
ELEMENTS_API std::string accountName();
///get a particular environment variable
ELEMENTS_API std::string getEnv(const std::string& var);
/// get a particular environment variable, storing the value in the passed string if the
/// variable is set. Returns true if the variable is set, false otherwise.
ELEMENTS_API bool getEnv(const std::string& var, std::string &value);
///get all environment variables
ELEMENTS_API std::vector<std::string> getEnv();
///Set an environment variables.
///If value is empty, the variable is removed from the environment.
///When overwrite is 0, the variable is not set if already present.
///Returns 0 on success, -1 on failure.
///See man 3 setenv.
ELEMENTS_API int setEnv(const std::string &name, const std::string &value,
                         bool overwrite = true);
/// Simple wrap around unsetenv for strings
ELEMENTS_API int unSetEnv(const std::string& name);
/// Check if an environment variable is set or not.
ELEMENTS_API bool isEnvSet(const std::string& var);

ELEMENTS_API int backTrace(ELEMENTS_UNUSED std::shared_ptr<void*> addresses, ELEMENTS_UNUSED const int depth);
ELEMENTS_API bool backTrace(std::string& btrace, const int depth, const int offset = 0);
ELEMENTS_API const std::vector<std::string> backTrace(const int depth, const int offset = 0);

ELEMENTS_API bool getStackLevel(ELEMENTS_UNUSED void* addresses, ELEMENTS_UNUSED void*& addr,
                                ELEMENTS_UNUSED std::string& fnc, ELEMENTS_UNUSED std::string& lib);

} // end of namespace System
} // end of namespace Elements

#endif    // SYSTEM_SYSTEM_H
