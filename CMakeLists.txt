cmake_minimum_required(VERSION 2.8)
project(hashmapbenchmark)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS chrono system)

if(Boost_FOUND)
	add_definitions(-DHAVE_BOOST -DBOOST_ALL_NO_LIB)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB srcs ${CMAKE_SOURCE_DIR}/*.h ${CMAKE_SOURCE_DIR}/*.cpp)
add_executable(hashmapbenchmark
	${srcs})

foreach(i Boost)
	include_directories(${${i}_INCLUDE_DIRS})
	target_link_libraries(hashmapbenchmark ${${i}_LIBRARIES})
	add_definitions(${${i}_DEFINITIONS})
endforeach()