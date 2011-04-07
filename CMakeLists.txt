cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build)

project(PVMockupGui)

# Locate and import ParaView enviroment.
find_package(ParaView REQUIRED)
include(${PARAVIEW_USE_FILE})
include(${ParaView_CMAKE_DIR}/ParaViewPlugins.cmake)

find_package(Qwt)

# Setup the include locations.
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QWT_INCLUDE_DIR})

# These are the C++ files to be compiled.
set (INCLUDE_FILES
  inc/iview.h
  inc/modecontrolwidget.h
  inc/mpmainwindow.h
  inc/standardview.h
  inc/threesliceview.h
  inc/timecontrolwidget.h
)
set (SOURCE_FILES
  src/iview.cpp
  src/main.cpp
  src/modecontrolwidget.cpp
  src/mpmainwindow.cpp
  src/standardview.cpp
  src/threesliceview.cpp
  src/timecontrolwidget.cpp
)

# These are the headers to be preprocessed using
# Qt's moc preprocessor.
qt4_wrap_cpp(MOC_SRCS
  inc/modecontrolwidget.h
  inc/mpmainwindow.h
  inc/standardview.h
  inc/threesliceview.h
  inc/timecontrolwidget.h
)

# These are the ui files to be processed using
# Qt's ui file processor.
qt4_wrap_ui(UI_BUILT_SOURCES
  ui/modecontrolwidget.ui
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

set (TEST_INCLUDE_FILES
  inc/axisinteractor.h
)

set (TEST_SOURCE_FILES
  src/axisinteractor.cpp
  src/test.cpp
)

qt4_wrap_cpp(TEST_MOC_SOURCES
  inc/axisinteractor.h
)

qt4_wrap_ui(TEST_UI_BUILT_SOURCES
  ui/axisinteractor.ui
)

add_executable(test
${TEST_INCLUDE_FILES} ${TEST_SOURCE_FILES} ${TEST_MOC_SOURCES} 
${TEST_UI_BUILT_SOURCES})

target_link_libraries(test ${QT_LIBRARIES} ${QWT_LIBRARIES})
