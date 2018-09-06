cmake_minimum_required(VERSION 3.7.1)

project(BuildLibrary)

if(MSVC)
	# enable unicode in windows platform
	add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

set(MYTOOLS_SOURCE_FILES
	RandomToolNeedLib/MTRandom.cpp
	RandomToolNeedLib/PrimeSearch.cpp)
	
add_library(MyTool ${MYTOOLS_SOURCE_FILES})

set_target_properties(MyTool
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
