if (NOT IWYU_FOUND)

    find_program(IWYU_EXECUTABLE NAMES include-what-you-use iwyu
                 HINTS ENV IWYU_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(IWYU_EXECUTABLE ${IWYU_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set IWYU_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(IWYU DEFAULT_MSG IWYU_EXECUTABLE)

    mark_as_advanced(IWYU_FOUND IWYU_EXECUTABLE)


endif (NOT IWYU_FOUND)

