cmake_minimum_required(VERSION 3.7.1)

project(BuildLibrary)

if(MSVC)
	# enable unicode in windows platform
	add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

add_subdirectory(MyTools)
add_subdirectory(Lua)
