cmake_minimum_required(VERSION 2.8)
project(hashmapbenchmark)

#add_definitions(-DBUILD_PARAM_MAPPED_TYPE_SIZE=100)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system)

if(Boost_FOUND)
	add_definitions(-DHAVE_BOOST -DBOOST_ALL_NO_LIB)
endif()

find_package(Poco COMPONENTS Foundation)
if(POCO_FOUND)
	add_definitions(-DHAVE_POCO)
endif()

include(CheckIncludeFile)
include(CheckTypeSize)
include(CheckSymbolExists)

# detect config values for sparsehash config

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_type_size("long long" LONG_LONG) # also checks HAVE_SYS_TYPES_H, HAVE_STDINT_H, HAVE_STDDEF_H
check_type_size(uint16_t UINT16_T)
check_type_size(u_int16_t U_INT16_T)
check_type_size(__uint16 __UINT16)
check_symbol_exists(memcpy string.h HAVE_MEMCPY)

configure_file(sparsehash/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/sparsehash/internal/sparseconfig.h)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR} # for embedded files and sparsehash config
	)
	
# embed html files into .h/.cpp
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmbedFiles.cmake)
embed_files(embedded_html_files
	RELATIVE html
	FILES html/linechart.html html/barchart.html
	)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(APPEND srcs
	${CMAKE_CURRENT_BINARY_DIR}/embedded_html_files.cpp
	${CMAKE_CURRENT_BINARY_DIR}/embedded_html_files.h
	)

add_executable(hashmapbenchmark
	${srcs}
	${linechart_SOURCES})

foreach(i Boost POCO)
	include_directories(${${i}_INCLUDE_DIRS})
	target_link_libraries(hashmapbenchmark ${${i}_LIBRARIES})
	add_definitions(${${i}_DEFINITIONS})
endforeach()


