CMAKE_MINIMUM_REQUIRED(VERSION 3.5)


include(ElementsUtils)

include_guard(GLOBAL)

find_file_to_configure(common.imp.in
                       FILETYPE "IWYU Mapping file"
                       PATH_SUFFIXES "check"
                       OUTPUTDIR "${PROJECT_BINARY_DIR}/check"
                       OUTPUTNAME common.imp
                       PATHS ${CMAKE_MODULE_PATH})


if(NOT IWYU_MAPPING_FILE)
  set(MAPPING_FILE "${PROJECT_BINARY_DIR}/check/common.imp" CACHE STRING "List of mapping files for IWYU")
else()
  set(MAPPING_FILE ${IWYU_MAPPING_FILE})
endif()
