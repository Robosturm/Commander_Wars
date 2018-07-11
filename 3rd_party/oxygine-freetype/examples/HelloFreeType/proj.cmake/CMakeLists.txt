cmake_minimum_required (VERSION 2.6)
project (HelloFreeType)

add_subdirectory(../../../../oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(../../../../oxygine-freetype/ oxygine-freetype)
include_directories(${OXYGINE_FREETYPE_INCLUDE_DIRS})
link_directories(${OXYGINE_FREETYPE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS} ${OXYGINE_FREETYPE_CXX_FLAGS}")

add_executable(HelloFreeType ../src/main.cpp ../src/example.cpp  ../src/example.h  ../src/test.h   ../src/test.cpp )
target_link_libraries(HelloFreeType ${OXYGINE_CORE_LIBS} ${OXYGINE_FREETYPE_LIBS} oxygine-freetype)



if (WIN32) #disable console mode for VC++
	set_target_properties(HelloFreeType PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)



if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648 -s FORCE_FILESYSTEM=1")
	em_link_pre_js(HelloFreeType  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)