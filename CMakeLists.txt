project(qt_vtk)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

set(VTK_DIR "C:/ProgramData/swissmeda/VTK-7.1.0-build")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set( CMAKE_AUTOMOC ON)
set( CMAKE_AUTOUIC ON )
find_package( Qt5Widgets REQUIRED ON )

include_directories( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

SET( PSDK_PATH "C:/Program Files/Microsoft Platform SDK/Bin" )
	SET( PSDK_INC "C:/Program Files/Microsoft Platform SDK/Include" )

	FIND_PATH(DX9_INCLUDE_PATH d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK (June 2010)/Include"
		DOC "The directory where D3D9.h resides")


	FIND_LIBRARY(D3D9_LIBRARY d3d9.lib
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK (June 2010)/Lib/x86"
		DOC "The directory where d3d9.lib resides")

	FIND_LIBRARY(D3DX9_LIBRARY d3dx9.lib
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK (June 2010)/Lib/x86"
		DOC "The directory where d3dx9.lib resides")

	SET( DX9_LIBRARIES ${D3D9_LIBRARY} ${D3DX9_LIBRARY} )



set( SOURCES main.cpp
    mainwindow.cpp
)
set( HEADERS mainwindow.h )
set( UI mainwindow.ui )


add_executable(${PROJECT_NAME} ${SOURCES} ${UI} ${HEADERS} )

qt5_use_modules( ${PROJECT_NAME} Core Gui )
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${DX9_LIBRARIES} wbemuuid.lib)

# To run use arguments
# -DCMAKE_PREFIX_PATH=/home/oleh/Qt5.5.1/5.5/gcc_64 -DVTK_DIR=/home/oleh/VTK/build
