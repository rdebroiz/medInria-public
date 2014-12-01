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
  target
  headers_list
  )

################################################################################
#
# Usage: set_lib_install_rules(target, header1, header2, header3 ...)
# set rules for the library designed by the target, and add all the additional
# header to {CMAKE_PREFIX_INSTALL}/include during install step.
#
################################################################################

qt5_use_modules(${target} Declarative)
qt5_use_modules(${target} Core)
qt5_use_modules(${target} Gui)
qt5_use_modules(${target} Quick)
qt5_use_modules(${target} Widgets)
qt5_use_modules(${target} Sql)
qt5_use_modules(${target} Network)
qt5_use_modules(${target} Xml)
qt5_use_modules(${target} OpenGL)


set(MEDINRIA_BINARY_DIR
  ${${target}_BINARY_DIR}/lib
  ${MEDINRIA_BINARY_DIR}
  )

install(TARGETS ${target}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )


## #############################################################################
## Add header wich have to be exposed in the include dir of the install tree
## #############################################################################

if(${ARGC} GREATER 1)
  set(headers ${ARGV})
  list(REMOVE_ITEM headers ${target})
  install(FILES ${headers}
    DESTINATION include/${target}
    )
endif()

endmacro()

