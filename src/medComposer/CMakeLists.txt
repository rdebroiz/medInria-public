### CMakeLists.txt ---
## 
## Author: Thibaud Kloczko
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
  medComposerNodeImage.h
  medComposerFactory.h)

set(${PROJECT_NAME}_HEADERS
  medComposerFactory.h
  medComposerNodeFiltering.h
  medComposerNodeImage.h
  medComposerNodeImageWriter.h
  medComposerNodeView.h)

set(${PROJECT_NAME}_SOURCES
  medComposerFactory.cpp
  medComposerNodeFiltering.cpp
  medComposerNodeImage.cpp
  medComposerNodeImageWriter.cpp
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
    ${QT_LIBRARIES})

target_link_libraries(${PROJECT_NAME}
    dtkCore
    dtkComposer
    dtkGui
    dtkLog)

target_link_libraries(${PROJECT_NAME}
    medCore)

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
