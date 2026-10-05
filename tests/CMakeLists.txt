cmake_minimum_required (VERSION 3.19.0)

# Standalone configure (cmake -S tests -B build_tests) needs its own project()
# call; when included from the root project via COW_BUILD_TESTS_ONLY the root
# project() call already ran.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Commander_Wars_Tests LANGUAGES CXX C)
endif()

get_filename_component(COW_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Fixed configuration for the test build:
# GRAPHICSUPPORT stays ON because the tested actor queueing/threading code paths
# (Actor::requiresThreadChange) only exist with graphics enabled.
# AUDIOSUPPORT stays ON to compile the proven desktop configuration (the harness
# never initializes audio; the ON+OFF combo is not supported by the engine).
set(AUDIOSUPPORT ON)
set(GRAPHICSUPPORT ON)
set(UPDATESUPPORT OFF)
set(USEAPPCONFIGPATH OFF)
set(DEPLOY_RESOURCES_AS_FOLDER OFF)
set(DEFAULTAIPIPE OFF)
set(SCRIPTSOURCESUPPORT OFF)
# the main build compiles with precompiled headers; the test build does the same
# so both compile the sources in the same environment
set(PRECOMPILED_HEADERS ON)
# openssl can be provided precompiled via -DPRECOMPILEDOPENSSL=ON
# -DOPENSSL_LIB_PATH=... -DOPENSSL_INCLUDE_DIR=... (used on the windows runner)
option(PRECOMPILEDOPENSSL "if the openssl are precompiled libs or or package exists" OFF)
option(OPENSSL_USE_STATIC_LIBS "link statically against openssl" TRUE)

set(COW_BUILD_TAG "tests")
set(COW_BUILD_NAME "tests")
set(UPDATE_FILE "tests")

find_package(Qt6 COMPONENTS Core Gui Widgets Qml Network Xml Sql OpenGL Test REQUIRED)

# all game sources include headers relative to the repository root
include_directories(${COW_ROOT_DIR})

# shared source lists, compile definitions and link libraries
include(${COW_ROOT_DIR}/CommanderWarsSources.cmake)

add_executable(commander_wars_tests
    testgamewindow.h
    testgamewindow.cpp
    oxygine_lifetime/tst_oxyginelifetime.cpp
    ${Commander_Wars_OXYGINE_SRCS}
    ${Commander_Wars_SRCS}
    ${Commander_Wars_OS_SRCS}
    ${Commander_Wars_SmtpClient_SRCS}
    ${Commander_Wars_OpenNN_SRCS}
    ${Commander_Wars_UPDATE_SRCS}
    ${Commander_Wars_AUDIO_SRCS}
    ${WIN_TEST_ICON}
)

if (PRECOMPILED_HEADERS)
    cow_setup_precompiled_headers(commander_wars_tests)
endif()

target_link_libraries(commander_wars_tests PUBLIC
    ${QT_LIBRARIES}
    Qt6::Test
    )

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # the rc file resolves its icon relative to the build working directory
    # (matching the main windows pipeline's layout), so mirror the icon there
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/../distribution/res/icons")
    file(COPY_FILE "${COW_ROOT_DIR}/distribution/res/icons/icoclient_windows.ico"
         "${CMAKE_BINARY_DIR}/../distribution/res/icons/icoclient_windows.ico" ONLY_IF_DIFFERENT)
    set(WIN_TEST_ICON ${COW_ROOT_DIR}/distribution/res/ICON.rc)
    target_link_libraries(commander_wars_tests PUBLIC
        # special linking of libs for windows
        Dbghelp
        Xinput
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    target_link_libraries(commander_wars_tests PUBLIC
        "-framework IOKit"
        )
endif()

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    # the runner's Qt is not on the default PATH, so the test exe cannot start
    # without the Qt runtime DLLs next to it (Windows shows a modal missing-DLL
    # dialog that stalls a headless runner forever). Deploy them like the main
    # build does via windeployqt.
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    if (WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET commander_wars_tests POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                "$<TARGET_FILE:commander_wars_tests>"
                --libdir "$<TARGET_FILE_DIR:commander_wars_tests>"
                --plugindir "$<TARGET_FILE_DIR:commander_wars_tests>"
                --no-quick-import
                --no-translations
            COMMENT "Deploying Qt runtime for tests..."
        )
    else()
        message(WARNING "windeployqt not found - the test exe may not start on windows without Qt DLLs on PATH")
    endif()
endif()

enable_testing()
add_test(NAME oxygine_lifetime
    COMMAND commander_wars_tests
    WORKING_DIRECTORY ${COW_ROOT_DIR}
)
# the test harness never shows a window or creates a GL context, so it runs
# headless on every platform; the timeout keeps a wedged runner from stalling
set_tests_properties(oxygine_lifetime PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    TIMEOUT 120
)

add_executable(matchmaking_tests
    matchmaking/tst_elocalculator.cpp
    ${COW_ROOT_DIR}/network/elocalculator.cpp
    ${COW_ROOT_DIR}/network/elocalculator.h
)
target_include_directories(matchmaking_tests PRIVATE ${COW_ROOT_DIR})
target_link_libraries(matchmaking_tests PRIVATE Qt6::Core Qt6::Test)
add_test(NAME matchmaking_elo COMMAND matchmaking_tests)
