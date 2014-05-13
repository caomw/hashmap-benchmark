cmake_minimum_required(VERSION 2.8)
project(hashmapbenchmark)

#add_definitions(-DBUILD_PARAM_MAPPED_TYPE_SIZE=100)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system)

if(Boost_FOUND)
	add_definitions(-DHAVE_BOOST -DBOOST_ALL_NO_LIB)
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedFiles.cmake)
embed_files(embedded_html_files RELATIVE html FILES html/linechart.html)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(APPEND srcs
	${CMAKE_CURRENT_BINARY_DIR}/embedded_html_files.cpp
	${CMAKE_CURRENT_BINARY_DIR}/embedded_html_files.h
	)

add_executable(hashmapbenchmark
	${srcs}
	${linechart_SOURCES})

foreach(i Boost)
	include_directories(${${i}_INCLUDE_DIRS})
	target_link_libraries(hashmapbenchmark ${${i}_LIBRARIES})
	add_definitions(${${i}_DEFINITIONS})
endforeach()
