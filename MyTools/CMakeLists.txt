
set(MYTOOLS_SOURCE_FILES
	RandomToolNeedLib/MTRandom.cpp
	RandomToolNeedLib/PrimeSearch.cpp)
	
add_library(MyTool ${MYTOOLS_SOURCE_FILES})

set_target_properties(MyTool
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/MyTools/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/MyTools/lib")
