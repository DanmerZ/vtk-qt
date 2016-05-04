project(qt_vtk)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_AUTOMOC ON)
set( CMAKE_AUTOUIC ON )
find_package( Qt5Widgets REQUIRED ON )

include_directories( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )



set( SOURCES main.cpp
    mainwindow.cpp
)
set( HEADERS mainwindow.h )
set( UI mainwindow.ui )


add_executable(${PROJECT_NAME} ${SOURCES} ${UI} ${HEADERS} )

qt5_use_modules( ${PROJECT_NAME} Core Gui )
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )


# To run use arguments
# -DCMAKE_PREFIX_PATH=/home/oleh/Qt5.5.1/5.5/gcc_64 -DVTK_DIR=/home/oleh/VTK/build
