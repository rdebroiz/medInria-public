################################################################################
#
# medInria
#
# Copyright (c) INRIA 2013 - 2014. All rights reserved.
# See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

macro(set_lib_install_rules
  target_name
  headers_list
  )


################################################################################
#
# Usage: set_lib_install_rules(target, header1, header2, header3 ...)
# set rules for the library designed by the target, and add all the additional
# header to {CMAKE_PREFIX_INSTALL}/include during install step.
#
################################################################################

set(${PROJECT_NAME}_BINARY_DIR
  ${${target_name}_BINARY_DIR}/lib
  ${${PROJECT_NAME}_BINARY_DIR}
  )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

install(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )


## #############################################################################
## Add header wich have to be exposed in the include dir of the install tree
## #############################################################################

if(${ARGC} GREATER 1)
  set(headers ${ARGV})
  list(REMOVE_ITEM headers ${target_name})
  install(FILES ${headers}
    DESTINATION include/${target_name}
    )
endif()

endmacro()

