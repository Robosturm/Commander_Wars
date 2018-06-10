cmake_minimum_required (VERSION 2.6)
project (Commander_Wars)

# add oxygen as our engine
add_subdirectory(../../Oxygen/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

# source list we start slow here with the main cpp
set(${PROJECT_NAME}_SRCS
	main.cpp
)

# create executable
add_executable(
	${PROJECT_NAME}
        ${${PROJECT_NAME}_SRCS}
)
# link libraries
target_link_libraries(
        ${PROJECT_NAME}
	${OXYGINE_CORE_LIBS}
)


# install section
set(CMAKE_INSTALL_PREFIX "../install/${CMAKE_BUILD_TYPE}")

# select libs to install
set(${PROJECT_NAME}_INSTALL_LIBS
    Libs/libcurl.dll
    Libs/libeay32.dll
    Libs/libgcc_s_dw2-1.dll
    Libs/libpng16.dll
    Libs/libssh2.dll
    Libs/libstdc++-6.dll
    Libs/libwinpthread-1.dll
    Libs/libzlib.dll
    Libs/pthreadVCE2.dll
    Libs/SDL2.dll
    Libs/ssleay32.dll
    Libs/zlib.dll
    Libs/libSDL2.dll
)
# install libs
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION ".")
# install exe
install(TARGETS ${PROJECT_NAME} DESTINATION ".")
