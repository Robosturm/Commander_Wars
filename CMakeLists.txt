cmake_minimum_required (VERSION 2.8.11)
project (Commander_Wars)

# add oxygen as our engine
add_subdirectory(../../Oxygen/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# add qt to our project
find_package(Qt5Core REQUIRED)
# Qt5LinguistTools
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5QML REQUIRED)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(
        -DQT_DECLARATIVE_DEBUG
        -DQT_QML_DEBUG
        -DGAMEDEBUG
    )
else("Release")
    add_definitions(
        -DGAMERELEASE
    )
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

# source list we start slow here with the main cpp
set(${PROJECT_NAME}_SRCS
        main.cpp
        mainwindow.cpp
        resource_management/unitspritemanager.cpp
        resource_management/fontmanager.cpp
        coreengine/mainapp.cpp
        coreengine/settings.cpp
        coreengine/interpreter.cpp
        coreengine/console.cpp

)




# create translation here :)
qt5_create_translation(${CMAKE_CURRENT_SOURCE_DIR}/translation/language.qm ${${PROJECT_NAME}_SRCS})

# create executable
add_executable(
	${PROJECT_NAME}
        ${${PROJECT_NAME}_SRCS}
)
# link libraries
target_link_libraries(
        ${PROJECT_NAME}
	${OXYGINE_CORE_LIBS}
        Qt5::Core
        Qt5::Qml
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

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${_qmake_executable}" DIRECTORY)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
         ${QT_BIN_DIR}/Qt5Qmld.dll
         ${QT_BIN_DIR}/Qt5Cored.dll
         ${QT_BIN_DIR}/Qt5Networkd.dll
    )
else("Release")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
         ${QT_BIN_DIR}/Qt5Qml.dll
         ${QT_BIN_DIR}/Qt5Core.dll
         ${QT_BIN_DIR}/Qt5Network.dll
    )
endif()

# install libs
install(DIRECTORY resources DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS} DESTINATION ".")
# install exe
install(TARGETS ${PROJECT_NAME} DESTINATION ".")
