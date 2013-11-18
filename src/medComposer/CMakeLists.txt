### CMakeLists.txt ---
## 
## Author: 
## Created: Mon Nov 18 10:44:24 2013 (+0100)
## Version: 
## Last-Updated: Mon Nov 18 12:33:14 2013 (+0100)
##           By: hacker
##     Update #: 34
######################################################################
## 
### Change Log:
## 
######################################################################

project(medComposer)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS_MOC
  medComposerFactory.h)

set(${PROJECT_NAME}_HEADERS
  medComposerFactory.h
  medComposerNodeView.h)

set(${PROJECT_NAME}_SOURCES
  medComposerFactory.cpp
  medComposerNodeView.cpp)

## ###################################################################
## Build rules
## ###################################################################

add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_PCH}
  ${${PROJECT_NAME}_SOURCES_MOC}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_WRAP})

target_link_libraries(${PROJECT_NAME}
    ${QT_LIBRARIES}
    dtkCore
    dtkComposer
    dtkLog)

## ###################################################################
## Source tree layout
## ###################################################################

source_group("Header Files" REGULAR_EXPRESSION .*\\.h\$)
source_group("Generated Files" FILES ${${PROJECT_NAME}_SOURCES_MOC})

## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
