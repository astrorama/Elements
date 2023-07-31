# Find the Python NumPy package
# PYTHON_NUMPY_INCLUDE_DIR
# PYTHON_NUMPY_FOUND
# will be set by this script

cmake_minimum_required(VERSION 3.5)

if(NOT NumPy_FOUND)
  if(NumPy_FIND_QUIETLY)
    find_package(Python ${PYTHON_EXPLICIT_VERSION} COMPONENTS Interpreter Development NumPy QUIET)
  else()
    find_package(Python ${PYTHON_EXPLICIT_VERSION} COMPONENTS Interpreter Development NumPy)
    set(__numpy_out 1)
  endif()
endif()

if(NOT Python_NumPy_INCLUDE_DIRS)

  if (PYTHON_EXECUTABLE)
    # Find out the include path
    execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" -c
            "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
            OUTPUT_VARIABLE __numpy_path)
    # And the version
    execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" -c
            "from __future__ import print_function\ntry: import numpy; print(numpy.__version__, end='')\nexcept:pass\n"
            OUTPUT_VARIABLE Python_NumPy_VERSION)
  elseif(__numpy_out)
    message(STATUS "Python executable not found.")
  endif(PYTHON_EXECUTABLE)

  find_path(Python_NumPy_INCLUDE_DIRS numpy/arrayobject.h
            HINTS "${__numpy_path}" "${PYTHON_INCLUDE_PATH}" NO_DEFAULT_PATH)

endif()

set(NUMPY_INCLUDE_DIRS ${Python_NumPy_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy REQUIRED_VARS Python_NumPy_INCLUDE_DIRS NUMPY_INCLUDE_DIRS
                                        VERSION_VAR Python_NumPy_VERSION)

mark_as_advanced(NumPy_FOUND NUMPY_INCLUDE_DIRS)
