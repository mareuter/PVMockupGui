cmake_minimum_required(VERSION 2.8)

project(PVMockupGui)

# Locate and import ParaView enviroment.
find_package(ParaView REQUIRED)
include(${PARAVIEW_USE_FILE})
include(${ParaView_CMAKE_DIR}/ParaViewPlugins.cmake)

# Setup the include locations.
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
  ${CMAKE_CURRENT_BINARY_DIR})

# These are the C++ files to be compiled.
set (INCLUDE_FILES
  inc/mpmainwindow.h
  inc/standardview.h
  inc/threesliceview.h
  inc/timecontrolwidget.h
)
set (SOURCE_FILES
  src/main.cpp
  src/mpmainwindow.cpp
  src/standardview.cpp
  src/threesliceview.cpp
  src/timecontrolwidget.cpp
)

# These are the headers to be preprocessed using
# Qt's moc preprocessor.
qt4_wrap_cpp(MOC_SRCS
  inc/mpmainwindow.h
  inc/standardview.h
  inc/threesliceview.h
  inc/timecontrolwidget.h
)

# These are the ui files to be processed using
# Qt's ui file processor.
qt4_wrap_ui(UI_BUILT_SOURCES
  ui/mpmainwindow.ui
  ui/standardview.ui
  ui/threesliceview.ui
  ui/timecontrolwidget.ui
)

add_executable(${CMAKE_PROJECT_NAME}
${INCLUDE_FILES} ${SOURCE_FILES} ${MOC_SRCS} ${UI_BUILT_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME}
pqCore pqComponents pqApplicationComponents
${QT_LIBRARIES})
