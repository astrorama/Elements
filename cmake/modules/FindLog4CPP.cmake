# -*- cmake -*-
# FindLog4CPP.cmake
#
# Created on: Jul 26, 2013
#     Author: hubert
#
# - Locate Log4CPP library
# Defines:
#
#  LOG4CPP_FOUND
#  LOG4CPP_INCLUDE_DIR
#  LOG4CPP_INCLUDE_DIRS (not cached)
#  LOG4CPP_LIBRARY
#  LOG4CPP_LIBRARIES (not cached)
#
# Imports:
#
#  Log4CPP::log4cpp
#

# Find quietly if already found before
if(DEFINED CACHE{LOG4CPP_INCLUDE_DIR})
  set(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY YES)
endif()



find_path(LOG4CPP_INCLUDE_DIR log4cpp/Category.hh
          HINTS ENV LOG4CPP_INSTALL_DIR
          PATH_SUFFIXES include)
find_library(LOG4CPP_LIBRARY log4cpp
             HINTS ENV LOG4CPP_INSTALL_DIR
             PATH_SUFFIXES lib)

# handle the QUIETLY and REQUIRED arguments and set LOG4CPP_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Log4CPP DEFAULT_MSG LOG4CPP_INCLUDE_DIR LOG4CPP_LIBRARY)
mark_as_advanced(LOG4CPP_FOUND LOG4CPP_INCLUDE_DIR LOG4CPP_LIBRARY)

set(LOG4CPP_LIBRARIES ${LOG4CPP_LIBRARY})
set(LOG4CPP_INCLUDE_DIRS ${LOG4CPP_INCLUDE_DIR})


# Modernisation: create an interface target to link against
if(TARGET Log4CPP::log4cpp)
    return()
endif()
if(LOG4CPP_FOUND)
  add_library(Log4CPP::log4cpp IMPORTED INTERFACE)
  target_include_directories(Log4CPP::log4cpp SYSTEM INTERFACE "${LOG4CPP_INCLUDE_DIRS}")
  target_link_libraries(Log4CPP::log4cpp INTERFACE "${LOG4CPP_LIBRARIES}")
  # Display the imported target for the user to know
  if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message(STATUS "  Import target: Log4CPP::log4cpp")
  endif()
endif()
