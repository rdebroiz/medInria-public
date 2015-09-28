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

project(medProcess VERSION ${MEDINRIA_VERSION})

## #############################################################################
## List Sources
## #############################################################################

list_source_files(${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}
  arithmetic_operation
  )


## #############################################################################
## include directories.
## #############################################################################

list_header_directories_to_include(${PROJECT_NAME}
  ${${PROJECT_NAME}_HEADERS}
  )

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS}
  ${medCore_INCLUDE_DIRS}
  ${medCoreLegacy_INCLUDE_DIRS}
  )


## #############################################################################
## add library
## #############################################################################

qt5_add_resources(${PROJECT_NAME}_QRC
  ${${PROJECT_NAME}_QRC}
  )

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_CFILES}
  ${${PROJECT_NAME}_QRC}
  )


## #############################################################################
## Link
## #############################################################################

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    dtkCore
    medCore
    medCoreLegacy
    )

## #############################################################################
## install
## #############################################################################

set_lib_install_rules(${PROJECT_NAME}
  ${${PROJECT_NAME}_HEADERS}
  )
