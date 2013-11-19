### CMakeLists.txt ---
##
## Author: Nicolas Niclausse
######################################################################
##
### Commentary:
##
######################################################################
##
### Change log:
##
######################################################################

project(medSlave)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_SOURCES
  main.cpp)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  medCore
  medComposer
  dtkCore
  dtkComposer
  dtkDistributed)

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)

## #################################################################
## Installation
## #################################################################

install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)
