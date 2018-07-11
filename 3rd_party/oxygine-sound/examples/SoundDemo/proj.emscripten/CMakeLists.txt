cmake_minimum_required (VERSION 2.6)
project (SoundDemo)

add_subdirectory(../../../../oxygine-framework oxygine-framework)

add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(../../../ oxygine-sound)
include_directories(${OXYGINESOUND_INCLUDE_DIRS})



SET(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(SoundDemo
	../src/main.cpp
	../src/example.h 
	../src/example.cpp
	../src/test.cpp
	../src/test.h
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648 -s FORCE_FILESYSTEM=1")


target_link_libraries(SoundDemo ${OXYGINE_CORE_LIBS} ${OXYGINESOUND_LIBS})
em_link_pre_js(SoundDemo ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
#	${CMAKE_CURRENT_SOURCE_DIR}/sound.js
#	${CMAKE_CURRENT_SOURCE_DIR}/js2cpp.js
#	${CMAKE_CURRENT_SOURCE_DIR}/preloading.js)