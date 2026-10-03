cmake_minimum_required (VERSION 3.19.0)
project (Commander_Wars LANGUAGES CXX C)

# Delegate to the standalone tests project when configuring a tests-only build
# (used by the desktop-tests-debug preset to build into build_tests/).
if (COW_BUILD_TESTS_ONLY)
    enable_testing()
    add_subdirectory(tests)
    return()
endif()

# Information for building commander wars on other platforms
# 1. make sure the CMAKE_PREFIX_PATH targets to the correct qt installation
# 2. make sure the LIBRARY_PATH targets the library folder of qt e.g. LD_LIBRARY_PATH=$HOME/Qt/5.15.2/gcc_64/lib
# 3. make sure that -DOPENSSL_ROOT_DIR= targets your local openssl installation e.g. -DOPENSSL_ROOT_DIR=D:/Qt/Tools/OpenSSL

# Special Information for Android
# ANDROID_NATIVE_API_LEVEL needs to be at least 26 when audio support is enabled.
# NDK Version needs to be: 21.3.6528147 -> This needs to be matched exactly

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set tool chain info
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
option(OPENSSL_USE_STATIC_LIBS "you may disable linking static against openssl but it's not recommended. The installation process won't deploy the libs." TRUE)
option(OPENSSL_MSVC_STATIC_RT "you may disable linking static against openssl but it's not recommended. The installation process won't deploy the libs." TRUE)
option(AUDIOSUPPORT "enables audio support for the game and compiles music/sound into the exe" ON)
option(GRAPHICSUPPORT "enables sprites for the game and compiles them into the exe" ON)
option(PRECOMPILEDOPENSSL "if the openssl are precompiled libs or or package exists" OFF)
option(USEAPPCONFIGPATH "use the system location for saving data, instead of the working directory" OFF)
option(DEPLOY_RESOURCES_AS_FOLDER "if files should be compiled into the exe or deployed as folder" OFF)
option(UPDATESUPPORT "Enables the automatic updater for the game which updates to the latest github version should be turned off for local builds not supported for android" OFF)
option(DEFAULTAIPIPE "If the system should use a separate process for the ai as default variant. Disable it for system that can't launch the same exe twice e.g. android." ON)
option(PRECOMPILED_HEADERS "Whether to use precompiled headers to speed up compilation. This can make precompiled headers less effective." ON)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer (Linux/GCC only)" OFF)
option(ENABLE_ASAN "Enable Address Sanitizer for memory leak detection (Linux/GCC only)" OFF)
option(ENABLE_VALGRIND "Enable Valgrind instrumentation for memory debugging (Linux/GCC only)" OFF)
option(SCRIPTSOURCESUPPORT "Exposes script function source text to JS. Needs the QtQml private headers and is disabled when they are missing" ON)

set(UPDATE_FILE "Commander_Wars-Win10_x64"  CACHE STRING "UPDATE File")
###################################################################################
# show configuration
###################################################################################

get_filename_component(CXX_COMPILER_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)
get_filename_component(C_COMPILER_PATH "${CMAKE_C_COMPILER}" DIRECTORY)
message("Prefix Path: ${CMAKE_PREFIX_PATH}")
message("Module Path: ${CMAKE_MODULE_PATH}")
message("Qt-Host-Path: ${QT_HOST_PATH}")
message("Qt-Dir: ${Qt6_DIR}")
message("Qt-Tools Path: ${Qt6CoreTools_DIR}")
message("C-Compiler Path: ${C_COMPILER_PATH}")
message("C-Compiler: ${CMAKE_C_COMPILER}")
message("C++-Compiler Path: ${CXX_COMPILER_PATH}")
message("C++-Compiler: ${CMAKE_CXX_COMPILER}")
message("CFlags: ${CMAKE_C_FLAGS}")
message("CXX Flags: ${CMAKE_CXX_FLAGS}")
message("CMake Systemname: ${CMAKE_SYSTEM_NAME}")
message("CMake Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message("OpenSsL-Dir:" ${OPENSSL_ROOT_DIR})
message("Toolchain-file: ${CMAKE_TOOLCHAIN_FILE}")
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    message("SDK: ${ANDROID_SDK_ROOT}")
    message("NDK: ${ANDROID_NDK}")
    message("STL: ${ANDROID_STL}")
endif()


###################################################################################
# Find external libs
###################################################################################
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Xml REQUIRED)
find_package(Qt6 COMPONENTS Sql REQUIRED)
if (SCRIPTSOURCESUPPORT)
    find_package(Qt6 COMPONENTS QmlPrivate REQUIRED)
endif()
if (GRAPHICSUPPORT)
    find_package(Qt6 COMPONENTS OpenGL REQUIRED)
endif()

if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
endif()
# Retrieve the absolute path to qmake and then use that path to find
# the binaries
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
if(NOT DEFINED QT_VERSION)
    find_program(QMAKE_EXE NAMES qmake)
    execute_process(COMMAND ${QMAKE_EXE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)
endif()
message("Qt-Version: ${QT_VERSION}")
if(Qt6Core_VERSION VERSION_LESS "6.5.0")
    add_compile_definitions(Qt_6_5=Qt_6_4)
endif()

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    find_program(ADDR_2_LINE addr2line)
    message("addr2Line: ${ADDR_2_LINE}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
endif()

if (SCRIPTSOURCESUPPORT)
    # distro packages may ship Qt6::QmlPrivate without the headers it points at
    find_file(QTQML_PRIVATE_HEADER qv4function_p.h
        PATHS ${Qt6Qml_INCLUDE_DIRS}
        PATH_SUFFIXES "${Qt6Qml_VERSION}/QtQml/private"
        NO_DEFAULT_PATH
    )
    if (QTQML_PRIVATE_HEADER)
        message("Building with script source support")
        add_definitions(
            -DSCRIPTSOURCESUPPORT
        )
    else()
        message(WARNING "QtQml private headers not found, building without script source support")
        set(SCRIPTSOURCESUPPORT OFF)
    endif()
endif()

if(GRAPHICSUPPORT)
    message("Building with OpenGl")
else()
    message("Building without UI")
endif()

if (USEAPPCONFIGPATH)
    message("Building with user config path")
else()
    message("Building without user config path")
endif()

###################################################################################
# General version information (android packaging)
###################################################################################

set(COW_ANDROID_VERSION_NAME "0.40.0-main")
set(COW_ANDROID_VERSION_CODE "72")

###################################################################################
# Shared sources, libs and flags (also used by the tests project)
###################################################################################
set(COW_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include(CommanderWarsSources.cmake)

##############################################################################
# Resources files #
##############################################################################
########################################

if (NOT DEPLOY_RESOURCES_AS_FOLDER)
    message("Using compiled resources")

    if (GRAPHICSUPPORT AND AUDIOSUPPORT)
        qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
            "general.qrc"
            "sprites.qrc"
            "music.qrc"
            "sound.qrc"
        )
    elseif(AUDIOSUPPORT)
        qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
            "general.qrc"
            "music.qrc"
            "sound.qrc"
        )
    elseif(GRAPHICSUPPORT)
        qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
            "general.qrc"
            "sprites.qrc"
        )
    else()
        qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
            "general.qrc"
        )
    endif()
else()
    message("Deploying resources as folder")
    add_definitions(-DDEPLOY_RESOURCES_AS_FOLDER)
endif()

qt_add_resources(${PROJECT_NAME}_Resource_SRCS "core.qrc") # These files are always deployed using QRCs.

##############################################################################
# This section finds all resources so they can be linked to a rcc file later #
##############################################################################
# this sections adds custom targets to view resources in the ide which don't work with qt-creators ide creation
FILE(GLOB_RECURSE ResourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.qrc")
FILE(GLOB_RECURSE TS_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "translation/*.ts")
FILE(GLOB_RECURSE TestScripts "_test/*.*")

add_custom_target(Resources SOURCES
    ${ResourceFiles}
    ${TS_FILES})

add_custom_target(Test SOURCES
    ${TestScripts}
    )

##############################################################
# Linking and executable section                             #
##############################################################

if (GRAPHICSUPPORT)
    SET(WIN_ICON distribution/res/ICON.rc)
else()
    SET(WIN_ICON distribution/res/ICON_SERVER.rc)
endif()
# rc.exe substitutes ${COW_WORKSPACE_ROOT} when compiling the icon resource;
# it points at the checkout root so the icon is found from any build directory
set(COW_WORKSPACE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION
    main.cpp
    ${${PROJECT_NAME}_OXYGINE_SRCS}
    ${${PROJECT_NAME}_SRCS}
    ${${PROJECT_NAME}_Resource_SRCS}
    ${${PROJECT_NAME}_OS_SRCS}
    ${${PROJECT_NAME}_SmtpClient_SRCS}
    ${${PROJECT_NAME}_OpenNN_SRCS}
    ${${PROJECT_NAME}_UPDATE_SRCS}
    ${${PROJECT_NAME}_AUDIO_SRCS}
    ${WIN_ICON}
)

if (PRECOMPILED_HEADERS)
    cow_setup_precompiled_headers(${PROJECT_NAME})
endif()

if (SCRIPTSOURCESUPPORT AND PRECOMPILED_HEADERS)
    # The QV4 private headers compile without the shared precompiled header.
    set_source_files_properties(coreengine/scriptfunctionsource.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_ANDROID_VERSION_NAME ${COW_ANDROID_VERSION_NAME}
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_ANDROID_VERSION_CODE ${COW_ANDROID_VERSION_CODE}
    )

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        # special linking of libs for windows
        Dbghelp
        Xinput
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        "-framework IOKit"
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        )
endif()

if(PRECOMPILEDOPENSSL AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    # bundles libssl_3.so/libcrypto_3.so into the APK so Qt's TLS backend can dlopen them
    add_android_openssl_libraries(${PROJECT_NAME})
endif()

qt_finalize_target(${PROJECT_NAME})

##############################################################
# Translation section                                        #
##############################################################
if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    # translation target isn't supported by qt 6 for android right now
    # create translation here :)
    set(LUPDATE_OPTIONS "-I ${CMAKE_CURRENT_SOURCE_DIR}")
    message ("Language Update Options: ${LUPDATE_OPTIONS}")
    # created translation files
    set(TS_FILES
        translation/lang_de_DE.ts
        translation/lang_en.ts
        translation/lang_zh-tw.ts
        )
    # adds the translations and a target for it

    FILE(GLOB_RECURSE scripts RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "resources/*.js")
    FILE(GLOB_RECURSE uiXmls RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "resources/ui/*.xml")
    qt_create_translation(
        QM_FILES
        ${${PROJECT_NAME}_SRCS}
        ${scripts}
        ${uiXmls}
        ${TS_FILES}
        OPTIONS "${LUPDATE_OPTIONS}")
    qt_add_translation(${PROJECT_NAME}_QM_FILES ${TS_FILES})
    add_custom_target(${PROJECT_NAME}_translations DEPENDS ${QM_FILES})
endif()

##############################################################
# end of this is just for the ide view nothing for execution #
##############################################################

##############################################################
# Install section                                            #
##############################################################
# get qt dir
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${_qmake_executable}" DIRECTORY)
message("Qt-Bin-Dir: " ${QT_BIN_DIR})

# those libs are not the same for each platform and need to be selected based on the os
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(LibNameStart "Qt6")
    set(LibNameEnd ".dll")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(LibNameEnd "d.dll")
    endif()
    set(${PROJECT_NAME}_INSTALL_OS_LIBS
        ${QT_BIN_DIR}/libstdc++-6.dll
        ${QT_BIN_DIR}/libwinpthread-1.dll
        ${QT_BIN_DIR}/libgcc_s_seh-1.dll
        ${OPENSSL_INCLUDE_DIR}/../bin/libcrypto-1_1-x64.dll
        ${OPENSSL_INCLUDE_DIR}/../bin/libcrypto-1_1.dll
        ${ADDR_2_LINE}
    )
    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
        ${QT_BIN_DIR}/../plugins/platforms/qwindows${LibNameEnd}
    )
    set(${PROJECT_NAME}_INSTALL_QT_SQL_LIBS
        ${QT_BIN_DIR}/../plugins/sqldrivers/qsqlite${LibNameEnd}
    )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(LibNameStart "../lib/libQt6")
    set(LibNameEnd ".so")
    set(${PROJECT_NAME}_INSTALL_OS_LIBS
    )
    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
    )

elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

message("using qt lib naming convention: ${LibNameStart}Libname${LibNameEnd}")

set(${PROJECT_NAME}_INSTALL_QT_LIBS
    ${QT_BIN_DIR}/${LibNameStart}Qml${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Core${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Network${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Gui${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Widgets${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Xml${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Sql${LibNameEnd}
)

if (GRAPHICSUPPORT)
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${${PROJECT_NAME}_INSTALL_QT_LIBS}
        ${QT_BIN_DIR}/${LibNameStart}OpenGL${LibNameEnd}
    )
endif()

message("install lib dir: ${CMAKE_INSTALL_PREFIX}")
# install folders and executable

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(TARGETS ${PROJECT_NAME}
        RUNTIME         DESTINATION         "."
        BUNDLE          DESTINATION         "."
        LIBRARY         DESTINATION         "."
    )

    install(DIRECTORY templates DESTINATION "/")
    if (DEPLOY_RESOURCES_AS_FOLDER)
        message("Installing resources as folder")
        install(DIRECTORY resources DESTINATION             "/")
        install(DIRECTORY maps DESTINATION                  "/")
        install(FILES ${QM_FILES} DESTINATION               "resources/translation/" OPTIONAL)
        install(DIRECTORY customTerrainImages DESTINATION   "/")
        install(DIRECTORY mods DESTINATION                  "/")
        install(DIRECTORY savegames DESTINATION             "/")
        install(DIRECTORY data DESTINATION                  "/")
    endif()
    # install libs
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS}            DESTINATION     "/"             OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_OS_LIBS}            DESTINATION     "/"             OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS}  DESTINATION     "platforms"     OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_SQL_LIBS}        DESTINATION     "sqldrivers"    OPTIONAL)
    install(FILES xinput1_3.dll                                 DESTINATION     "/"             OPTIONAL)
    install(FILES ${OPENSSL_LIBS}                               DESTINATION     "/"             OPTIONAL)

elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME_LOWER}"
    )
    install(TARGETS ${PROJECT_NAME}
        RUNTIME         DESTINATION         "bin"
        BUNDLE          DESTINATION         "bin"
        LIBRARY         DESTINATION         "bin"
    )

    if (DEPLOY_RESOURCES_AS_FOLDER)
        message("Installing resources as folder")
        install(DIRECTORY resources DESTINATION             "share/commander_wars")
        install(DIRECTORY maps DESTINATION                  "share/commander_wars")
        install(DIRECTORY customTerrainImages DESTINATION   "share/commander_wars")
        install(DIRECTORY mods DESTINATION                  "share/commander_wars")
        install(DIRECTORY savegames DESTINATION             "share/commander_wars")
        install(DIRECTORY data DESTINATION                  "share/commander_wars")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/android-build/build/outputs/apk/debug/android-build-debug.apk"
                DESTINATION "."                   RENAME "${PROJECT_NAME}_${ANDROID_ABI}.apk" OPTIONAL)
    else("Release")
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/android-build/build/outputs/apk/release/android-build-release-signed.apk"
                DESTINATION "."                   RENAME "${PROJECT_NAME}_${ANDROID_ABI}.apk" OPTIONAL)
    endif()
else()
    install(TARGETS ${PROJECT_NAME}
        RUNTIME         DESTINATION         "."
        BUNDLE          DESTINATION         "."
        LIBRARY         DESTINATION         ".")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(FullInstallPath "${CMAKE_INSTALL_PREFIX}/${INSTALL_PATH}/")
    message("Build will run windeployqt ${_qt_bin_dir} ${WINDEPLOYQT_EXECUTABLE} with path ${FullInstallPath}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                "$<TARGET_FILE:${PROJECT_NAME}>"
                --libdir \"${FullInstallPath}\"
                --plugindir \"${FullInstallPath}\"
                --compiler-runtime
                --no-quick-import
                --no-translations
        COMMENT "Running windeployqt..."
    )
    # create pdb information
    add_custom_target(createPdb
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cv2pdb/cv2pdb.exe ${CMAKE_INSTALL_PREFIX}/Commander_Wars.exe
        COMMENT "Running cv2pdb.exe..."
    )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    message("Build will run macdeployqt ${MACDEPLOYQT_EXECUTABLE}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.."
            -always-overwrite
        COMMENT "Running macdeployqt..."
    )
    # Re-sign the bundle (ad-hoc) after macdeployqt/manual plugin copies modified its contents.
    # `codesign --deep` signs bundles outside-in and mishandles versioned framework bundles
    # (Contents/Frameworks/Qt*.framework/Versions/A + "Current" symlink), which can leave nested
    # frameworks/plugins with an invalid/stale signature even though the outer .app signature
    # itself still verifies fine. Launching the raw executable directly only checks its own
    # signature, so it "works", while LaunchServices (double-clicking the .app in Finder) performs
    # a full recursive validation, finds the broken nested signatures and the app hangs/never
    # finishes launching. Sign every nested framework/plugin individually first (inside-out),
    # then sign the outer bundle without --deep. Also strip any leftover xattrs (e.g. quarantine
    # flags reintroduced by zipping) that can make codesign/Gatekeeper re-flag nested binaries.
    # Use "-exec ... +" (batches args, no trailing ";") instead of "-exec ... \;" since the
    # semicolon needs multiple layers of escaping (CMake, make, shell) that's easy to break.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND /usr/bin/xattr -cr "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.."
        COMMAND /bin/sh -c "find '$<TARGET_FILE_DIR:${PROJECT_NAME}>/../../Contents/Frameworks' -name '*.framework' -exec codesign --force --sign - {} +"
        COMMAND /bin/sh -c "find '$<TARGET_FILE_DIR:${PROJECT_NAME}>/../../Contents/PlugIns' -name '*.dylib' -exec codesign --force --sign - {} +"
        COMMAND /bin/sh -c "find '$<TARGET_FILE_DIR:${PROJECT_NAME}>/../../Contents/Frameworks' -name '*.dylib' -exec codesign --force --sign - {} +"
        COMMAND codesign --force --sign - "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.."
        COMMENT "Ad-hoc codesigning app bundle (inside-out)..."
    )
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message("Build will zip artifacts")
    add_custom_target(createReleaseZip
        WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/../"
        COMMAND ${CMAKE_COMMAND} -E tar
        "cfv"
        "${CMAKE_INSTALL_PREFIX}/../Commander_Wars.zip"
        --format=zip
        "${CMAKE_INSTALL_PREFIX}/"
        COMMENT "Zipping Commander Wars..."
    )
endif()
