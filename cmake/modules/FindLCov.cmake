CMAKE_MINIMUM_REQUIRED(VERSION 3.20..4.0)

if (NOT LCOV_FOUND)

    find_package(GCov)

    if(NOT GCOV_EXECUTABLE)
        message(WARNING "gcov not found!")
    endif() # no GCOV_EXECUTABLE

	find_program(LCOV_EXECUTABLE NAMES lcov
	             HINTS ENV LCOV_INSTALL_DIR GCOV_INSTALL_DIR
	             PATH_SUFFIXES bin)
	set(LCOV_EXECUTABLE ${LCOV_EXECUTABLE} CACHE STRING "")

  execute_process(COMMAND ${LCOV_EXECUTABLE} --version
                  OUTPUT_VARIABLE LCOV_VERSION_CALL_OUTPUT)
  string(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" LCOV_VERSION
         ${LCOV_VERSION_CALL_OUTPUT})

# handle the QUIETLY and REQUIRED arguments and set LCOV_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LCov DEFAULT_MSG LCOV_EXECUTABLE LCOV_VERSION)

    mark_as_advanced(LCOV_FOUND LCOV_EXECUTABLE LCOV_VERSION)


endif (NOT LCOV_FOUND)
