CMAKE_MINIMUM_REQUIRED(VERSION 3.20..4.0)

if (NOT NINJA_FOUND)

    find_program(NINJA_EXECUTABLE NAMES ninja-build ninja
                 HINTS ENV NINJA_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(NINJA_EXECUTABLE ${NINJA_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set NINJA_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Ninja DEFAULT_MSG NINJA_EXECUTABLE)

    mark_as_advanced(NINJA_FOUND NINJA_EXECUTABLE)


endif (NOT NINJA_FOUND)
