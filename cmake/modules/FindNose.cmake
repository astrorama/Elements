CMAKE_MINIMUM_REQUIRED(VERSION 3.20..4.0)

if (NOT NOSE_FOUND)

    find_package(Python ${PYTHON_EXPLICIT_VERSION} COMPONENTS Interpreter)

    set(explicit_nose)
    if(PYTHON_EXPLICIT_VERSION)
      set(explicit_nose nosetests-${PYTHON_EXPLICIT_VERSION})
    endif()


    if(Python_Interpreter_FOUND)
        get_filename_component(_python_path ${Python_EXECUTABLE} PATH)
        find_program(NOSE_EXECUTABLE
                     NAMES ${explicit_nose} nosetests
                     HINTS ${_python_path})
    else()
	    find_program(NOSE_EXECUTABLE NAMES ${explicit_nose} nosetests
	                 HINTS ENV NOSE_INSTALL_DIR
                     PATH_SUFFIXES bin)
    endif()

	set(NOSE_EXECUTABLE ${NOSE_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set NOSE_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Nose DEFAULT_MSG NOSE_EXECUTABLE)

    mark_as_advanced(NOSE_FOUND NOSE_EXECUTABLE)


endif (NOT NOSE_FOUND)
