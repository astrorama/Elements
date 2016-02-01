  # Add Doxygen generation
  find_package(Doxygen)
  if(DOXYGEN_FOUND)

    find_file(doxygen_file_template
              NAMES Doxyfile.in
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)


    if(doxygen_file_template)
      configure_file(
        "${doxygen_file_template}"
        "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile"
        @ONLY
      )
      message(STATUS "Generated Doxygen configuration file: ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile")
      message(STATUS "From the Doxygen.in template file: ${doxygen_file_template}")

    endif()

    add_custom_target(doc
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

  endif()
  
  
  
  find_package(Sphinx)
  if(SPHINX_FOUND)
  
    find_file(sphinx_conf_template
              NAMES Sphinx_conf.py.in
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)


    if(sphinx_conf_template)
      configure_file(
        "${sphinx_conf_template}"
        "${PROJECT_BINARY_DIR}/doc/sphinx/conf.py"
        @ONLY
      )
      message(STATUS "Generated Sphinx configuration file: ${PROJECT_BINARY_DIR}/doc/sphinx/conf.py")
      message(STATUS "From the Doxygen.in template file: ${sphinx_conf_template}")

    endif()
  
  endif()
  