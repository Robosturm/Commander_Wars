cmake_minimum_required (VERSION 2.6)
project (EXAMPLE1)

add_subdirectory(../../../../oxygine-framework oxygine-framework)

add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(../../../ oxygine-sound)
include_directories(${OXYGINESOUND_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(example1 
	../src/main.cpp
	../src/example.h 
	../src/example.cpp
	../src/test.cpp
	../src/test.h
	../src/TestSoundFormats.h
)

target_link_libraries(example1 ${OXYGINESOUND_LIBS} ${OXYGINE_CORE_LIBS})