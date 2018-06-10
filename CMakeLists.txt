cmake_minimum_required (VERSION 2.6)
project (Commander_Wars)

# add oxygen as our engine
add_subdirectory(../Oxygen/oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

set(${PROJECT_NAME}_SRCS
	main.cpp
)

# create executable
add_executable(
	${PROJECT_NAME}
	${PROJECT_NAME}_SRCS
)
# link libraries
target_link_libraries(
	${PROJECT_NAME} 
	${OXYGINE_CORE_LIBS}
)