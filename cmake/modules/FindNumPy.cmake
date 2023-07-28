# Find the Python NumPy package
# PYTHON_NUMPY_INCLUDE_DIR
# PYTHON_NUMPY_FOUND
# will be set by this script

cmake_minimum_required(VERSION 3.5)



if(NOT Python_NumPy_FOUND)
  if(NumPy_FIND_QUIETLY)
    find_package(Python ${PYTHON_EXPLICIT_VERSION} COMPONENTS Interpreter Development NumPy QUIET)
  else()
    find_package(Python ${PYTHON_EXPLICIT_VERSION} COMPONENTS Interpreter Development NumPy)
    set(__numpy_out 1)
  endif()
endif()

set(NUMPY_INCLUDE_DIRS ${Python_NumPy_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy REQUIRED_VARS Python_NumPy_INCLUDE_DIRS NUMPY_INCLUDE_DIRS
                                        VERSION_VAR Python_NumPy_VERSION)

mark_as_advanced(NumPy_FOUND NUMPY_INCLUDE_DIRS)
