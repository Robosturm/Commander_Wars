# Shared build content for Commander Wars.
# Consumed by the root project (CMakeLists.txt) and the standalone tests
# project (tests/CMakeLists.txt).
# The includer must define:
#   - COW_ROOT_DIR: absolute path to the repository root (this file's directory)
#   - the options AUDIOSUPPORT, GRAPHICSUPPORT, PRECOMPILEDOPENSSL,
#     OPENSSL_USE_STATIC_LIBS, SCRIPTSOURCESUPPORT, UPDATESUPPORT,
#     DEFAULTAIPIPE and friends (matching the root project's option defaults)

###################################################################################
# OpenSsl
###################################################################################

if(PRECOMPILEDOPENSSL AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    message("Using local pre-compiled openssl version from path: ${OPENSSL_LIB_PATH} for android")
    set(OPENSSL_LIBS
        ${OPENSSL_LIB_PATH}/libcrypto.a
        ${OPENSSL_LIB_PATH}/libssl.a
    CACHE INTERNAL "")
    include_directories(${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL Include directories:" ${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL libs:" ${OPENSSL_LIBS})
    # Qt's TLS backend plugin dlopen's libssl/libcrypto at runtime instead of using the
    # statically linked symbols above, so the shared libs also need to be bundled into the APK.
    include(${COW_ROOT_DIR}/android_openssl/android_openssl.cmake)
elseif(PRECOMPILEDOPENSSL AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    message("Using local pre-compiled openssl version from path: ${OPENSSL_LIB_PATH} for windows")
    set(OPENSSL_LIBS
        ${OPENSSL_LIB_PATH}/libcrypto-4-x64.dll
        ${OPENSSL_LIB_PATH}/libssl-4-x64.dll
    CACHE INTERNAL "")
    include_directories(${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL Include directories:" ${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL libs:" ${OPENSSL_LIBS})
else()
    message("Using openssl package with root-dir ${OPENSSL_ROOT_DIR}")
    find_package(OpenSSL REQUIRED)
    message ("OpenSsL Include directories:" ${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL All-Libs:" ${OPENSSL_LIBRARIES})
    include_directories(${OPENSSL_INCLUDE_DIR})
    set(OPENSSL_LIBS
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

###################################################################################
# Set build dependend defines
###################################################################################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("Compiling as Debug")
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
        add_definitions(
            -DGAMEDEBUG                 # adds additional js checks and asserts
            -DDEBUG_LEVEL=0             # default console log level
            -DHEAVY_AI                 # experimental heavy ai unfinished
            #-DMEMORYTRACING            # only enable it if you can deal with a drastical performance decrease
            -DOXYGINE_DEBUG_SAFECAST    # changes static casts to dynamic casts
            )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(
            -DGAMEDEBUG                 # adds additional js checks and asserts
            -DDEBUG_LEVEL=0             # default console log level
            -DHEAVY_AI                 # experimental heavy ai unfinished
            #-DMEMORYTRACING            # only enable it if you can deal with a drastical performance decrease
            -DOXYGINE_DEBUG_SAFECAST    # changes static casts to dynamic casts
            )
    else()
        add_definitions(
            -DGAMEDEBUG                 # adds additional js checks and asserts
            -DDEBUG_LEVEL=0             # default console log level
            -DHEAVY_AI                 # experimental heavy ai unfinished
            #-DMEMORYTRACING            # only enable it if you can deal with a drastical performance decrease
            -DOXYGINE_DEBUG_SAFECAST    # changes static casts to dynamic casts
            )
    endif()
else("Release")
    message("Compiling as Release")
    add_definitions(
        -DDEBUG_LEVEL=2             # default console log level
    )
endif()

###################################################################################
# Audio support (PortAudio & Opus)
###################################################################################

if (AUDIOSUPPORT)
    message("Building with Audio (PortAudio & Opus)")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
        # Upstream PortAudio ships no Android host API (no ALSA/PulseAudio/JACK device nodes
        # exist on stock Android), so Pa_Initialize() would succeed with zero usable devices
        # and every stream would silently fail to open. Use a small AAudio-backed shim instead
        # that implements the exact subset of the PortAudio C API AudioManager relies on.
        message("Using AAudio-backed PortAudio compatibility shim for Android")
        add_library(portaudio STATIC
        )
        target_include_directories(portaudio PUBLIC ${COW_ROOT_DIR}/3rd_party/portaudio/include)
        target_link_libraries(portaudio PRIVATE aaudio log android)

        set(Commander_Wars_AUDIO_SRCS
            ${COW_ROOT_DIR}/3rd_party/portaudio_android/pa_android_aaudio.cpp
            ${COW_ROOT_DIR}/coreengine/audiodecoder.cpp ${COW_ROOT_DIR}/coreengine/audiodecoder.h
        )
    else()
        set(PA_BUILD_SHARED_LIBS OFF CACHE BOOL "Build PortAudio as static" FORCE)
        set(PA_BUILD_TESTS OFF CACHE BOOL "Build PortAudio tests" FORCE)
        set(PA_BUILD_EXAMPLES OFF CACHE BOOL "Build PortAudio examples" FORCE)
        add_subdirectory(${COW_ROOT_DIR}/3rd_party/portaudio ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/portaudio EXCLUDE_FROM_ALL)
        set_target_properties(portaudio PROPERTIES POSITION_INDEPENDENT_CODE ON)
        set(Commander_Wars_AUDIO_SRCS
            ${COW_ROOT_DIR}/coreengine/audiodecoder.cpp ${COW_ROOT_DIR}/coreengine/audiodecoder.h
        )
    endif()

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Ogg static" FORCE)
    set(INSTALL_DOCS OFF CACHE BOOL "" FORCE)
    set(INSTALL_PKG_CONFIG_MODULE OFF CACHE BOOL "" FORCE)
    set(INSTALL_CMAKE_PACKAGE_MODULE OFF CACHE BOOL "" FORCE)
    set(INCLUDE_INTTYPES_H 1)
    set(INCLUDE_STDINT_H 1)
    set(INCLUDE_SYS_TYPES_H 1)
    set(SIZE16 int16_t)
    set(USIZE16 uint16_t)
    set(SIZE32 int32_t)
    set(USIZE32 uint32_t)
    set(SIZE64 int64_t)
    set(USIZE64 uint64_t)
    set(OGG_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/3rd_party/libogg/include")
    file(MAKE_DIRECTORY "${OGG_GENERATED_INCLUDE_DIR}/ogg")
    configure_file(
        ${COW_ROOT_DIR}/3rd_party/libogg/include/ogg/config_types.h.in
        "${OGG_GENERATED_INCLUDE_DIR}/ogg/config_types.h"
    )
    add_library(ogg STATIC
        ${COW_ROOT_DIR}/3rd_party/libogg/src/bitwise.c
        ${COW_ROOT_DIR}/3rd_party/libogg/src/framing.c
    )
    target_include_directories(ogg PUBLIC
        ${COW_ROOT_DIR}/3rd_party/libogg/include
        "${OGG_GENERATED_INCLUDE_DIR}"
    )

    set(OPUS_BUILD_SHARED_LIBRARY OFF CACHE BOOL "Build Opus static" FORCE)
    set(OPUS_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(OPUS_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${COW_ROOT_DIR}/3rd_party/libopus ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/libopus EXCLUDE_FROM_ALL)

    set_target_properties(ogg PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(opus PROPERTIES POSITION_INDEPENDENT_CODE ON C_VISIBILITY_PRESET hidden)

    add_library(opusfile STATIC
        ${COW_ROOT_DIR}/3rd_party/libopusfile/src/info.c
        ${COW_ROOT_DIR}/3rd_party/libopusfile/src/internal.c
        ${COW_ROOT_DIR}/3rd_party/libopusfile/src/opusfile.c
        ${COW_ROOT_DIR}/3rd_party/libopusfile/src/stream.c
    )
    set_target_properties(opusfile PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(opusfile PRIVATE OP_DISABLE_HTTP=1)
    target_include_directories(opusfile PUBLIC
        ${COW_ROOT_DIR}/3rd_party/libopusfile/include
    )
    target_include_directories(opusfile PRIVATE
        ${COW_ROOT_DIR}/3rd_party/libopusfile/src
        ${COW_ROOT_DIR}/3rd_party/libogg/include
        ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/libogg/include
        ${COW_ROOT_DIR}/3rd_party/libopus/include
    )
    target_link_libraries(opusfile PRIVATE ogg opus)
    if (UNIX AND NOT APPLE AND NOT ANDROID)
        target_link_libraries(opusfile PRIVATE m)
    endif()

    add_definitions(
        -DAUDIOSUPPORT
    )
    include_directories(
        ${COW_ROOT_DIR}/3rd_party/portaudio/include
        ${COW_ROOT_DIR}/3rd_party/audio_decoders
        ${COW_ROOT_DIR}/3rd_party/libopusfile/include
        ${COW_ROOT_DIR}/3rd_party/libopus/include
        ${COW_ROOT_DIR}/3rd_party/libogg/include
        ${CMAKE_CURRENT_BINARY_DIR}/3rd_party/libogg/include
    )
else()
    message("Building without Audio")
    set(Commander_Wars_AUDIO_SRCS
    )
endif()

if(GRAPHICSUPPORT)
    message("Building with OpenGl")
    add_definitions(
        -DGRAPHICSUPPORT
    )
else()
    message("Building without UI")
endif()

if (USEAPPCONFIGPATH)
    message("Building with user config path")
    add_definitions(
        -DUSEAPPCONFIGPATH
    )
else()
    message("Building without user config path")
endif()

if (DEFAULTAIPIPE)
    message("Using ai pipe as default")
    add_definitions(
        -DDEFAULTAIPIPE=true
    )
else()
    message("Using no ai pipe as default")
    add_definitions(
        -DDEFAULTAIPIPE=false
    )
endif()

###################################################################################
# General version information
###################################################################################
add_definitions(
    -DVERSION_MAJOR=0
    -DVERSION_MINOR=40
    -DVERSION_REVISION=0
    -DVERSION_SUFFIX="main"
    -DCOW_BUILD_TAG="${COW_BUILD_TAG}"
    -DCOW_BUILD_NAME="${COW_BUILD_NAME}"
    -DUPDATE_FILE="${UPDATE_FILE}"
)

###################################################################################
# Set up some compiler and linking options
# NOTE: keep this block AFTER the audio section: the portaudio subdirectory is
# added above and must NOT inherit WIN32_LEAN_AND_MEAN (its DirectSound backend
# needs mmsystem.h types like WAVEFORMATEX that the define hides on MinGW).
###################################################################################

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj -g ${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "-Wa,-mbig-obj -g ${CMAKE_C_FLAGS}")
        add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    if ("${ANDROID_ABI}" STREQUAL armeabi-v7a)
        set(CMAKE_C_FLAGS "-Wno-implicit-function-declaration ${CMAKE_C_FLAGS}")
        add_link_options("-long-plt")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-Wno-implicit-function-declaration ${CMAKE_C_FLAGS}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "-rdynamic ${CMAKE_CXX_FLAGS}")

    # Add sanitizers for Linux/GCC
    if(ENABLE_UBSAN)
        message("Enabling Undefined Behavior Sanitizer (UBSan)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
        add_link_options("-fsanitize=undefined")
    endif()

    if(ENABLE_VALGRIND)
        message("Enabling Valgrind instrumentation")
        # Valgrind requires debug symbols and typically no optimization for better diagnostics
        if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            message(WARNING "Valgrind works best with Debug builds. Consider setting CMAKE_BUILD_TYPE=Debug")
        endif()
        set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
        add_definitions(-DVALGRIND_ENABLED)
        # Valgrind and ASAN conflict, so disable ASAN if Valgrind is enabled
        if(ENABLE_ASAN)
            message(STATUS "Disabling Address Sanitizer (ASAN) because Valgrind is enabled")
            set(ENABLE_ASAN OFF)
        endif()
    elseif(ENABLE_ASAN)
        message("Enabling Address Sanitizer (ASan)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
        add_link_options("-fsanitize=address")
    endif()
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

###################################################################################
# start oxygine stuff -> qoxygine
###################################################################################
set(Commander_Wars_OXYGINE_SRCS
    # top level
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/AnimationFrame.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/AnimationFrame.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Clock.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Clock.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Draggable.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Draggable.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/EventDispatcher.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/EventDispatcher.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Input.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Input.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Material.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/Material.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/MaterialCache.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/MaterialCache.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/PointerState.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/PointerState.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/RenderDelegate.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/RenderDelegate.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/STDRenderer.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/STDRenderer.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/RenderState.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/oxygine-forwards.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/TextStyle.h
    #core
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/oxygine.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/oxygine.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/gamewindow.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/gamewindow.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/VideoDriver.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/renderer.h ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/renderer.cpp
    # actor
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Actor.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Actor.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Button.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Button.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/SlidingActor.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Sprite.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Sprite.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Stage.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/Stage.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/TextField.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/TextField.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/slidingsprite.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h
    # tween
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/Tween.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/Tween.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenAnim.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenAnimColumn.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenAnimColumn.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenQueue.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/TweenQueue.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenwait.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenwait.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenscreenshake.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenscreenshake.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenshakey.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenshakey.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenchangenumbertext.h ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/tween/tweenchangenumbertext.cpp
    # res stuff
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAnim.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAnim.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAtlas.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAtlas.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/Resource.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/Resource.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/Resources.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/Resources.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/SingleResAnim.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h
    # text utils
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/Aligner.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/Node.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/Node.h
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h
    # closure
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/closure.h
    # math
    ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/math/ScalarMath.h
)

if (GRAPHICSUPPORT)
    include_directories("${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl")
    set(Commander_Wars_OXYGINE_SRCS
        ${Commander_Wars_OXYGINE_SRCS}
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/texture.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/texture.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/ShaderProgram.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/ShaderProgram.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/VideoDriver.cpp
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/UberShaderProgram.cpp
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/opengl/renderer.cpp
        ${COW_ROOT_DIR}/game/gamerecording/opengl/gamemapimagesaver.cpp
    )
else()
    include_directories("${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none")
    set(Commander_Wars_OXYGINE_SRCS
        ${Commander_Wars_OXYGINE_SRCS}
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/windowBase.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/windowBase.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/texture.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/texture.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/ShaderProgram.cpp ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/ShaderProgram.h
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/VideoDriver.cpp
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/UberShaderProgram.cpp
        ${COW_ROOT_DIR}/3rd_party/oxygine-framework/oxygine/core/none/renderer.cpp
        ${COW_ROOT_DIR}/game/gamerecording/none/gamemapimagesaver.cpp
    )
endif()

###################################################################################
# end oxygine stuff -> qoxygine
###################################################################################

###################################################################################
# start opennn support stuff
###################################################################################
include_directories("${COW_ROOT_DIR}/3rd_party/opennn/eigen")
set(Commander_Wars_OpenNN_SRCS
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/adaptive_moment_estimation.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/adaptive_moment_estimation.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/addition_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/addition_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/bounding_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/bounding_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/convolutional_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/convolutional_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/correlations.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/correlations.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/cross_entropy_error.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/cross_entropy_error.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/cross_entropy_error_3d.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/cross_entropy_error_3d.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/dataset.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/dataset.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/dense_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/dense_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/embedding_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/embedding_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/flatten_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/flatten_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/formula_expression.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/formula_expression.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/genetic_algorithm.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/genetic_algorithm.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/growing_inputs.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/growing_inputs.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/growing_neurons.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/growing_neurons.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/image_dataset.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/image_dataset.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/image_utilities.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/image_utilities.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/inputs_selection.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/inputs_selection.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/kmeans.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/kmeans.h
    # ${COW_ROOT_DIR}/3rd_party/opennn/opennn/language_dataset.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/language_dataset.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/levenberg_marquardt_algorithm.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/levenberg_marquardt_algorithm.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/loss.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/loss.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/mean_squared_error.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/mean_squared_error.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/minkowski_error.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/minkowski_error.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/model_expression.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/model_expression.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/model_selection.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/model_selection.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/multihead_attention_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/multihead_attention_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/neural_network.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/neural_network.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/neuron_selection.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/neuron_selection.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/normalization_layer_3d.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/normalization_layer_3d.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/normalized_squared_error.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/normalized_squared_error.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/optimizer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/optimizer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pch.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pch.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pooling_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pooling_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pooling_layer_3d.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/pooling_layer_3d.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/quasi_newton_method.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/quasi_newton_method.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/random_utilities.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/random_utilities.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/recurrent_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/recurrent_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/registry.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/registry.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/response_optimization.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/response_optimization.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/scaling.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/scaling.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/scaling_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/scaling_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/standard_networks.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/standard_networks.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/statistics.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/statistics.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/stochastic_gradient_descent.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/stochastic_gradient_descent.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/string_utilities.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/string_utilities.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/tensor_utilities.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/tensor_utilities.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/testing_analysis.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/testing_analysis.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/time_series_dataset.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/time_series_dataset.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/tinyxml2.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/tinyxml2.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/training_strategy.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/training_strategy.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/unscaling_layer.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/unscaling_layer.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/variable.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/variable.h
    ${COW_ROOT_DIR}/3rd_party/opennn/opennn/weighted_squared_error.cpp ${COW_ROOT_DIR}/3rd_party/opennn/opennn/weighted_squared_error.h
)

set(Commander_Wars_OpenNN_SRCS_ONLY ${Commander_Wars_OpenNN_SRCS})
list(FILTER Commander_Wars_OpenNN_SRCS_ONLY INCLUDE REGEX "^.*\.cpp?$")

set_source_files_properties(${Commander_Wars_OpenNN_SRCS} PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(${Commander_Wars_OpenNN_SRCS_ONLY} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

###################################################################################
# start 2 factor authentication stuff
###################################################################################
set(Commander_Wars_SmtpClient_SRCS
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/emailaddress.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/emailaddress.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeattachment.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeattachment.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebase64encoder.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebase64encoder.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebase64formatter.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebase64formatter.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebytearrayattachment.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimebytearrayattachment.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimecontentencoder.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimecontentencoder.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimecontentformatter.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimecontentformatter.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimefile.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimefile.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimehtml.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimehtml.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeinlinefile.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeinlinefile.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimemessage.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimemessage.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimemultipart.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimemultipart.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimepart.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimepart.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeqpencoder.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeqpencoder.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeqpformatter.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeqpformatter.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimetext.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimetext.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/quotedprintable.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/quotedprintable.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/smtpclient.cpp ${COW_ROOT_DIR}/3rd_party/smtpClient/src/smtpclient.h
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/SmtpMime
    ${COW_ROOT_DIR}/3rd_party/smtpClient/src/mimeattachment.h

    # qr code generation (Nayuki qrcodegen, MIT license)
    ${COW_ROOT_DIR}/3rd_party/qrcodegen/qrcodegen.cpp ${COW_ROOT_DIR}/3rd_party/qrcodegen/qrcodegen.hpp
)

###################################################################################
# end 2 factor authentication stuff
###################################################################################

###################################################################################
# source list of commander wars specific files
###################################################################################
###################################################################################
# start update support stuff
###################################################################################

if (UPDATESUPPORT AND NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    message("Building with update support")
    add_definitions(
        -DUPDATESUPPORT
        -DCOW_UPDATETARGET="${COW_UPDATETARGET}"
        -DCOW_INSTALLDIR="${COW_INSTALL_DIR}"
    )
    set(Commander_Wars_UPDATE_SRCS
        ${COW_ROOT_DIR}/updater/gameupdater.h ${COW_ROOT_DIR}/updater/gameupdater.cpp
        ${COW_ROOT_DIR}/updater/filedownloader.h ${COW_ROOT_DIR}/updater/filedownloader.cpp
    )
else()
    message("Building without update support")
    set(Commander_Wars_UPDATE_SRCS
    )
endif()

# Note: main.cpp is intentionally NOT part of this list. It is added by the
# root project only; the tests project brings its own test main (QTEST_MAIN).
set(Commander_Wars_SRCS
    #objects --> i don't have a good place to put them right now
    ${COW_ROOT_DIR}/objects/coinfoactor.cpp ${COW_ROOT_DIR}/objects/coinfoactor.h
    ${COW_ROOT_DIR}/objects/rotatingsprite.cpp ${COW_ROOT_DIR}/objects/rotatingsprite.h
    ${COW_ROOT_DIR}/objects/ruleselection.cpp ${COW_ROOT_DIR}/objects/ruleselection.h
    ${COW_ROOT_DIR}/objects/minimap.cpp ${COW_ROOT_DIR}/objects/minimap.h
    ${COW_ROOT_DIR}/objects/qrcodeactor.cpp ${COW_ROOT_DIR}/objects/qrcodeactor.h
    ${COW_ROOT_DIR}/objects/perkselection.cpp ${COW_ROOT_DIR}/objects/perkselection.h
    ${COW_ROOT_DIR}/objects/mapselectionview.cpp ${COW_ROOT_DIR}/objects/mapselectionview.h
    ${COW_ROOT_DIR}/objects/loadingscreen.cpp ${COW_ROOT_DIR}/objects/loadingscreen.h
    ${COW_ROOT_DIR}/objects/editorselection.cpp ${COW_ROOT_DIR}/objects/editorselection.h
    ${COW_ROOT_DIR}/objects/mapselection.cpp ${COW_ROOT_DIR}/objects/mapselection.h
    ${COW_ROOT_DIR}/objects/coselection.cpp ${COW_ROOT_DIR}/objects/coselection.h
    ${COW_ROOT_DIR}/objects/playerselection.cpp ${COW_ROOT_DIR}/objects/playerselection.h
    ${COW_ROOT_DIR}/objects/achievementbanner.cpp ${COW_ROOT_DIR}/objects/achievementbanner.h
    ${COW_ROOT_DIR}/objects/unitstatisticview.cpp ${COW_ROOT_DIR}/objects/unitstatisticview.h
    ${COW_ROOT_DIR}/objects/boxterrainpalettepreview.h ${COW_ROOT_DIR}/objects/boxterrainpalettepreview.cpp
    # objects base
    ${COW_ROOT_DIR}/objects/base/EventTextEdit.cpp ${COW_ROOT_DIR}/objects/base/EventTextEdit.h
    ${COW_ROOT_DIR}/objects/base/textinput.cpp ${COW_ROOT_DIR}/objects/base/textinput.h
    ${COW_ROOT_DIR}/objects/base/topbar.cpp ${COW_ROOT_DIR}/objects/base/topbar.h
    ${COW_ROOT_DIR}/objects/base/textbox.cpp ${COW_ROOT_DIR}/objects/base/textbox.h
    ${COW_ROOT_DIR}/objects/base/multilinetextbox.cpp ${COW_ROOT_DIR}/objects/base/multilinetextbox.h
    ${COW_ROOT_DIR}/objects/base/h_scrollbar.cpp ${COW_ROOT_DIR}/objects/base/h_scrollbar.h
    ${COW_ROOT_DIR}/objects/base/v_scrollbar.cpp ${COW_ROOT_DIR}/objects/base/v_scrollbar.h
    ${COW_ROOT_DIR}/objects/base/dropdownmenu.cpp ${COW_ROOT_DIR}/objects/base/dropdownmenu.h
    ${COW_ROOT_DIR}/objects/base/dropdownmenucolor.cpp ${COW_ROOT_DIR}/objects/base/dropdownmenucolor.h
    ${COW_ROOT_DIR}/objects/base/panel.cpp ${COW_ROOT_DIR}/objects/base/panel.h
    ${COW_ROOT_DIR}/objects/base/spinbox.cpp ${COW_ROOT_DIR}/objects/base/spinbox.h
    ${COW_ROOT_DIR}/objects/base/checkbox.cpp ${COW_ROOT_DIR}/objects/base/checkbox.h
    ${COW_ROOT_DIR}/objects/base/slider.cpp ${COW_ROOT_DIR}/objects/base/slider.h
    ${COW_ROOT_DIR}/objects/base/multislider.cpp ${COW_ROOT_DIR}/objects/base/multislider.h
    ${COW_ROOT_DIR}/objects/base/passwordbox.cpp ${COW_ROOT_DIR}/objects/base/passwordbox.h
    ${COW_ROOT_DIR}/objects/base/progressinfobar.cpp ${COW_ROOT_DIR}/objects/base/progressinfobar.h
    ${COW_ROOT_DIR}/objects/base/selectkey.cpp ${COW_ROOT_DIR}/objects/base/selectkey.h
    ${COW_ROOT_DIR}/objects/base/chat.cpp ${COW_ROOT_DIR}/objects/base/chat.h
    ${COW_ROOT_DIR}/objects/base/timespinbox.cpp ${COW_ROOT_DIR}/objects/base/timespinbox.h
    ${COW_ROOT_DIR}/objects/base/dropdownmenubase.cpp ${COW_ROOT_DIR}/objects/base/dropdownmenubase.h
    ${COW_ROOT_DIR}/objects/base/label.cpp ${COW_ROOT_DIR}/objects/base/label.h
    ${COW_ROOT_DIR}/objects/base/colorselector.cpp ${COW_ROOT_DIR}/objects/base/colorselector.h
    ${COW_ROOT_DIR}/objects/base/dropdownmenusprite.cpp ${COW_ROOT_DIR}/objects/base/dropdownmenusprite.h
    ${COW_ROOT_DIR}/objects/base/tooltip.cpp ${COW_ROOT_DIR}/objects/base/tooltip.h
    ${COW_ROOT_DIR}/objects/base/focusableobject.cpp ${COW_ROOT_DIR}/objects/base/focusableobject.h
    ${COW_ROOT_DIR}/objects/base/tableview.cpp ${COW_ROOT_DIR}/objects/base/tableview.h
    ${COW_ROOT_DIR}/objects/base/closeablepopup.cpp ${COW_ROOT_DIR}/objects/base/closeablepopup.h
    ${COW_ROOT_DIR}/objects/base/moveinbutton.cpp ${COW_ROOT_DIR}/objects/base/moveinbutton.h
    ${COW_ROOT_DIR}/objects/base/spriteobject.cpp ${COW_ROOT_DIR}/objects/base/spriteobject.h
    ${COW_ROOT_DIR}/objects/base/tabbedbox.h ${COW_ROOT_DIR}/objects/base/tabbedbox.cpp
    ${COW_ROOT_DIR}/objects/base/box9object.h ${COW_ROOT_DIR}/objects/base/box9object.cpp
    ${COW_ROOT_DIR}/objects/base/coloredbar.h ${COW_ROOT_DIR}/objects/base/coloredbar.cpp
    # objects dialogs
    ${COW_ROOT_DIR}/objects/dialogs/colorselectiondialog.cpp ${COW_ROOT_DIR}/objects/dialogs/colorselectiondialog.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogconnecting.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogconnecting.h
    ${COW_ROOT_DIR}/objects/dialogs/filedialog.cpp ${COW_ROOT_DIR}/objects/dialogs/filedialog.h
    ${COW_ROOT_DIR}/objects/dialogs/mapfiledialog.cpp ${COW_ROOT_DIR}/objects/dialogs/mapfiledialog.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogcostyle.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogcostyle.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogmessagebox.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogmessagebox.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogmodsyncprogress.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogmodsyncprogress.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogtextinput.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogtextinput.h
    ${COW_ROOT_DIR}/objects/dialogs/folderdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/folderdialog.h
    ${COW_ROOT_DIR}/objects/dialogs/dialogvaluecounter.cpp ${COW_ROOT_DIR}/objects/dialogs/dialogvaluecounter.h
    ${COW_ROOT_DIR}/objects/dialogs/gamepadinfo.cpp ${COW_ROOT_DIR}/objects/dialogs/gamepadinfo.h
    ${COW_ROOT_DIR}/objects/dialogs/customdialog.h ${COW_ROOT_DIR}/objects/dialogs/customdialog.cpp
    ${COW_ROOT_DIR}/objects/dialogs/dialogawbwrecorddownloader.h ${COW_ROOT_DIR}/objects/dialogs/dialogawbwrecorddownloader.cpp
    # objects dialogs editor
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifyunit.cpp ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifyunit.h
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifybuilding.cpp ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifybuilding.h
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifyterrain.cpp ${COW_ROOT_DIR}/objects/dialogs/editor/dialogmodifyterrain.h
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialograndommap.cpp ${COW_ROOT_DIR}/objects/dialogs/editor/dialograndommap.h
    ${COW_ROOT_DIR}/objects/dialogs/editor/mapeditdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/editor/mapeditdialog.h
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialogviewmapstats.h ${COW_ROOT_DIR}/objects/dialogs/editor/dialogviewmapstats.cpp
    ${COW_ROOT_DIR}/objects/dialogs/editor/dialogextendmap.h ${COW_ROOT_DIR}/objects/dialogs/editor/dialogextendmap.cpp
    # objects dialogs rules
    ${COW_ROOT_DIR}/objects/dialogs/rules/actionlistdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/actionlistdialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/buildlistdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/buildlistdialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/cobannlistdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/cobannlistdialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/coselectiondialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/coselectiondialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/perkselectiondialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/perkselectiondialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/playerselectiondialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/playerselectiondialog.h
    ${COW_ROOT_DIR}/objects/dialogs/rules/ruleselectiondialog.cpp ${COW_ROOT_DIR}/objects/dialogs/rules/ruleselectiondialog.h
    # objects dialogs ingame
    ${COW_ROOT_DIR}/objects/dialogs/ingame/coinfodialog.cpp ${COW_ROOT_DIR}/objects/dialogs/ingame/coinfodialog.h
    ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogvictoryconditions.cpp ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogvictoryconditions.h
    ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogattacklog.cpp ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogattacklog.h
    ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogunitinfo.cpp ${COW_ROOT_DIR}/objects/dialogs/ingame/dialogunitinfo.h
    ${COW_ROOT_DIR}/objects/dialogs/ingame/victoryrulepopup.cpp ${COW_ROOT_DIR}/objects/dialogs/ingame/victoryrulepopup.h
    # objects dialogs map selection
    ${COW_ROOT_DIR}/objects/dialogs/mapSelection/mapselectionfilterdialog.cpp ${COW_ROOT_DIR}/objects/dialogs/mapSelection/mapselectionfilterdialog.h

    #modding
    ${COW_ROOT_DIR}/modding/csvtableimporter.cpp ${COW_ROOT_DIR}/modding/csvtableimporter.h

    #menues
    ${COW_ROOT_DIR}/menue/basemenu.cpp ${COW_ROOT_DIR}/menue/basemenu.h
    ${COW_ROOT_DIR}/menue/basegamemenu.cpp ${COW_ROOT_DIR}/menue/basegamemenu.h
    ${COW_ROOT_DIR}/menue/mainwindow.cpp ${COW_ROOT_DIR}/menue/mainwindow.h
    ${COW_ROOT_DIR}/menue/editormenue.cpp ${COW_ROOT_DIR}/menue/editormenue.h
    ${COW_ROOT_DIR}/menue/gamemenue.cpp ${COW_ROOT_DIR}/menue/gamemenue.h
    ${COW_ROOT_DIR}/menue/movementplanner.cpp ${COW_ROOT_DIR}/menue/movementplanner.h
    ${COW_ROOT_DIR}/menue/optionmenue.cpp ${COW_ROOT_DIR}/menue/optionmenue.h
    ${COW_ROOT_DIR}/menue/mapselectionmapsmenue.cpp ${COW_ROOT_DIR}/menue/mapselectionmapsmenue.h
    ${COW_ROOT_DIR}/menue/creditsmenue.cpp ${COW_ROOT_DIR}/menue/creditsmenue.h
    ${COW_ROOT_DIR}/menue/victorymenue.cpp ${COW_ROOT_DIR}/menue/victorymenue.h
    ${COW_ROOT_DIR}/menue/campaignmenu.cpp ${COW_ROOT_DIR}/menue/campaignmenu.h
    ${COW_ROOT_DIR}/menue/wikimenu.cpp ${COW_ROOT_DIR}/menue/wikimenu.h
    ${COW_ROOT_DIR}/menue/costylemenu.cpp ${COW_ROOT_DIR}/menue/costylemenu.h
    ${COW_ROOT_DIR}/menue/replaymenu.cpp ${COW_ROOT_DIR}/menue/replaymenu.h
    ${COW_ROOT_DIR}/menue/achievementmenu.cpp ${COW_ROOT_DIR}/menue/achievementmenu.h
    ${COW_ROOT_DIR}/menue/shopmenu.cpp ${COW_ROOT_DIR}/menue/shopmenu.h
    ${COW_ROOT_DIR}/menue/generatormenu.h ${COW_ROOT_DIR}/menue/generatormenu.cpp

    # ressource management
    ${COW_ROOT_DIR}/resource_management/unitspritemanager.cpp ${COW_ROOT_DIR}/resource_management/unitspritemanager.h
    ${COW_ROOT_DIR}/resource_management/terrainmanager.cpp ${COW_ROOT_DIR}/resource_management/terrainmanager.h
    ${COW_ROOT_DIR}/resource_management/fontmanager.cpp ${COW_ROOT_DIR}/resource_management/fontmanager.h
    ${COW_ROOT_DIR}/resource_management/backgroundmanager.cpp ${COW_ROOT_DIR}/resource_management/backgroundmanager.h
    ${COW_ROOT_DIR}/resource_management/objectmanager.cpp ${COW_ROOT_DIR}/resource_management/objectmanager.h
    ${COW_ROOT_DIR}/resource_management/buildingspritemanager.cpp ${COW_ROOT_DIR}/resource_management/buildingspritemanager.h
    ${COW_ROOT_DIR}/resource_management/movementtablemanager.cpp ${COW_ROOT_DIR}/resource_management/movementtablemanager.h
    ${COW_ROOT_DIR}/resource_management/gamemanager.cpp ${COW_ROOT_DIR}/resource_management/gamemanager.h
    ${COW_ROOT_DIR}/resource_management/gameanimationmanager.cpp ${COW_ROOT_DIR}/resource_management/gameanimationmanager.h
    ${COW_ROOT_DIR}/resource_management/weaponmanager.cpp ${COW_ROOT_DIR}/resource_management/weaponmanager.h
    ${COW_ROOT_DIR}/resource_management/cospritemanager.cpp ${COW_ROOT_DIR}/resource_management/cospritemanager.h
    ${COW_ROOT_DIR}/resource_management/gamerulemanager.cpp ${COW_ROOT_DIR}/resource_management/gamerulemanager.h
    ${COW_ROOT_DIR}/resource_management/battleanimationmanager.cpp ${COW_ROOT_DIR}/resource_management/battleanimationmanager.h
    ${COW_ROOT_DIR}/resource_management/coperkmanager.cpp ${COW_ROOT_DIR}/resource_management/coperkmanager.h
    ${COW_ROOT_DIR}/resource_management/achievementmanager.cpp ${COW_ROOT_DIR}/resource_management/achievementmanager.h
    ${COW_ROOT_DIR}/resource_management/shoploader.cpp ${COW_ROOT_DIR}/resource_management/shoploader.h
    ${COW_ROOT_DIR}/resource_management/movementplanneraddinmanager.h ${COW_ROOT_DIR}/resource_management/movementplanneraddinmanager.cpp
    ${COW_ROOT_DIR}/resource_management/uimanager.h ${COW_ROOT_DIR}/resource_management/uimanager.cpp
    ${COW_ROOT_DIR}/resource_management/ressourcemanagement.h

    # core engine
    ${COW_ROOT_DIR}/coreengine/newsDownloader.cpp ${COW_ROOT_DIR}/coreengine/newsDownloader.h
    ${COW_ROOT_DIR}/coreengine/mainapp.cpp ${COW_ROOT_DIR}/coreengine/mainapp.h
    ${COW_ROOT_DIR}/coreengine/settings.cpp ${COW_ROOT_DIR}/coreengine/settings.h
    ${COW_ROOT_DIR}/coreengine/interpreter.cpp ${COW_ROOT_DIR}/coreengine/interpreter.h
    ${COW_ROOT_DIR}/coreengine/gameconsole.cpp ${COW_ROOT_DIR}/coreengine/gameconsole.h
    ${COW_ROOT_DIR}/coreengine/audiomanager.cpp ${COW_ROOT_DIR}/coreengine/audiomanager.h
    ${COW_ROOT_DIR}/coreengine/pathfindingsystem.cpp ${COW_ROOT_DIR}/coreengine/pathfindingsystem.h
    ${COW_ROOT_DIR}/coreengine/qmlvector.cpp ${COW_ROOT_DIR}/coreengine/qmlvector.h
    ${COW_ROOT_DIR}/coreengine/scriptvariables.cpp ${COW_ROOT_DIR}/coreengine/scriptvariables.h
    ${COW_ROOT_DIR}/coreengine/scriptvariable.cpp ${COW_ROOT_DIR}/coreengine/scriptvariable.h
    ${COW_ROOT_DIR}/coreengine/workerObject.cpp ${COW_ROOT_DIR}/coreengine/workerObject.h
    ${COW_ROOT_DIR}/coreengine/timer.cpp ${COW_ROOT_DIR}/coreengine/timer.h
    ${COW_ROOT_DIR}/coreengine/userdata.cpp ${COW_ROOT_DIR}/coreengine/userdata.h
    ${COW_ROOT_DIR}/coreengine/crashreporter.cpp ${COW_ROOT_DIR}/coreengine/crashreporter.h
    ${COW_ROOT_DIR}/coreengine/filesupport.cpp ${COW_ROOT_DIR}/coreengine/filesupport.h
    ${COW_ROOT_DIR}/coreengine/globalutils.cpp ${COW_ROOT_DIR}/coreengine/globalutils.h
    ${COW_ROOT_DIR}/coreengine/scriptfunctionsource.cpp ${COW_ROOT_DIR}/coreengine/scriptfunctionsource.h
    ${COW_ROOT_DIR}/coreengine/scriptvariablefile.cpp ${COW_ROOT_DIR}/coreengine/scriptvariablefile.h
    ${COW_ROOT_DIR}/coreengine/metatyperegister.cpp ${COW_ROOT_DIR}/coreengine/metatyperegister.h
    ${COW_ROOT_DIR}/coreengine/GamepadShared.cpp ${COW_ROOT_DIR}/coreengine/Gamepad.h
    ${COW_ROOT_DIR}/coreengine/commandlineparser.cpp ${COW_ROOT_DIR}/coreengine/commandlineparser.h
    ${COW_ROOT_DIR}/coreengine/JsCallback.h
    ${COW_ROOT_DIR}/coreengine/memorymanagement.h ${COW_ROOT_DIR}/coreengine/memorymanagement.cpp
    ${COW_ROOT_DIR}/coreengine/gameversion.h ${COW_ROOT_DIR}/coreengine/gameversion.cpp
    ${COW_ROOT_DIR}/coreengine/refobject.h
    ${COW_ROOT_DIR}/coreengine/jsthis.h ${COW_ROOT_DIR}/coreengine/jsthis.cpp
    ${COW_ROOT_DIR}/coreengine/virtualpaths.h ${COW_ROOT_DIR}/coreengine/virtualpaths.cpp

    # network engine
    ${COW_ROOT_DIR}/network/smtpmailsender.h ${COW_ROOT_DIR}/network/smtpmailsender.cpp
    ${COW_ROOT_DIR}/network/tcpclient.cpp ${COW_ROOT_DIR}/network/tcpclient.h
    ${COW_ROOT_DIR}/network/tcpserver.cpp ${COW_ROOT_DIR}/network/tcpserver.h
    ${COW_ROOT_DIR}/network/localserver.cpp ${COW_ROOT_DIR}/network/localserver.h
    ${COW_ROOT_DIR}/network/localclient.cpp ${COW_ROOT_DIR}/network/localclient.h
    ${COW_ROOT_DIR}/network/txtask.cpp ${COW_ROOT_DIR}/network/txtask.h
    ${COW_ROOT_DIR}/network/rxtask.cpp ${COW_ROOT_DIR}/network/rxtask.h
    ${COW_ROOT_DIR}/network/networkInterface.h ${COW_ROOT_DIR}/network/networkInterface.cpp
    ${COW_ROOT_DIR}/network/networkgamedata.cpp ${COW_ROOT_DIR}/network/networkgamedata.h
    ${COW_ROOT_DIR}/network/mainserver.cpp ${COW_ROOT_DIR}/network/mainserver.h
    ${COW_ROOT_DIR}/network/twoFactorAuthenticatorServer.h ${COW_ROOT_DIR}/network/twoFactorAuthenticatorServer.cpp
    ${COW_ROOT_DIR}/network/automatchmaker.h ${COW_ROOT_DIR}/network/automatchmaker.cpp
    ${COW_ROOT_DIR}/network/networkgame.cpp ${COW_ROOT_DIR}/network/networkgame.h
    ${COW_ROOT_DIR}/network/matchmakingcoordinator.h ${COW_ROOT_DIR}/network/matchmakingcoordinator.cpp
    ${COW_ROOT_DIR}/network/mapfileserver.h ${COW_ROOT_DIR}/network/mapfileserver.cpp
    ${COW_ROOT_DIR}/network/replayrecordfileserver.h ${COW_ROOT_DIR}/network/replayrecordfileserver.cpp
    ${COW_ROOT_DIR}/network/filepeer.h ${COW_ROOT_DIR}/network/filepeer.cpp
    ${COW_ROOT_DIR}/network/sqlmapfiltercreator.h ${COW_ROOT_DIR}/network/sqlmapfiltercreator.cpp
    ${COW_ROOT_DIR}/network/tcpgatewayserver.h ${COW_ROOT_DIR}/network/tcpgatewayserver.cpp
    ${COW_ROOT_DIR}/network/gatewayserver.h ${COW_ROOT_DIR}/network/gatewayserver.cpp
    ${COW_ROOT_DIR}/network/sslserver.h ${COW_ROOT_DIR}/network/sslserver.cpp
    ${COW_ROOT_DIR}/network/JsonKeys.h

    # game
    ${COW_ROOT_DIR}/game/gamemap.cpp ${COW_ROOT_DIR}/game/gamemap.h
    ${COW_ROOT_DIR}/game/terrain.cpp ${COW_ROOT_DIR}/game/terrain.h
    ${COW_ROOT_DIR}/game/building.cpp ${COW_ROOT_DIR}/game/building.h
    ${COW_ROOT_DIR}/game/co.cpp ${COW_ROOT_DIR}/game/co.h
    ${COW_ROOT_DIR}/game/player.cpp ${COW_ROOT_DIR}/game/player.h
    ${COW_ROOT_DIR}/game/unit.cpp ${COW_ROOT_DIR}/game/unit.h
    ${COW_ROOT_DIR}/game/terrainfindingsystem.cpp ${COW_ROOT_DIR}/game/terrainfindingsystem.h
    ${COW_ROOT_DIR}/game/gameaction.cpp ${COW_ROOT_DIR}/game/gameaction.h
    ${COW_ROOT_DIR}/game/unitpathfindingsystem.cpp ${COW_ROOT_DIR}/game/unitpathfindingsystem.h
    ${COW_ROOT_DIR}/game/GameEnums.cpp ${COW_ROOT_DIR}/game/GameEnums.h
    ${COW_ROOT_DIR}/game/gamerules.cpp ${COW_ROOT_DIR}/game/gamerules.h
    ${COW_ROOT_DIR}/game/gamerule.cpp ${COW_ROOT_DIR}/game/gamerule.h
    ${COW_ROOT_DIR}/game/victoryrule.cpp ${COW_ROOT_DIR}/game/victoryrule.h
    ${COW_ROOT_DIR}/game/weather.cpp ${COW_ROOT_DIR}/game/weather.h
    ${COW_ROOT_DIR}/game/cursor.cpp ${COW_ROOT_DIR}/game/cursor.h
    ${COW_ROOT_DIR}/game/createoutline.cpp ${COW_ROOT_DIR}/game/createoutline.h
    ${COW_ROOT_DIR}/game/actionperformer.h ${COW_ROOT_DIR}/game/actionperformer.cpp
    # cool ingame recording
    ${COW_ROOT_DIR}/game/gamerecording/iReplayReader.h
    ${COW_ROOT_DIR}/game/gamerecording/daytodayrecord.cpp ${COW_ROOT_DIR}/game/gamerecording/daytodayrecord.h
    ${COW_ROOT_DIR}/game/gamerecording/playerrecord.cpp ${COW_ROOT_DIR}/game/gamerecording/playerrecord.h
    ${COW_ROOT_DIR}/game/gamerecording/specialevent.cpp ${COW_ROOT_DIR}/game/gamerecording/specialevent.h
    ${COW_ROOT_DIR}/game/gamerecording/gamerecorder.cpp ${COW_ROOT_DIR}/game/gamerecording/gamerecorder.h
    ${COW_ROOT_DIR}/game/gamerecording/replayrecorder.cpp ${COW_ROOT_DIR}/game/gamerecording/replayrecorder.h
    ${COW_ROOT_DIR}/game/gamerecording/gamemapimagesaver.h

    # cool ingame script support
    ${COW_ROOT_DIR}/game/gamescript.cpp ${COW_ROOT_DIR}/game/gamescript.h
    ${COW_ROOT_DIR}/game/campaign.cpp ${COW_ROOT_DIR}/game/campaign.h
    # animation stuff
    ${COW_ROOT_DIR}/game/gameanimation/gameanimation.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimation.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationfactory.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationfactory.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationwalk.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationwalk.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationcapture.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationcapture.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationdialog.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationdialog.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationpower.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationpower.h
    ${COW_ROOT_DIR}/game/gameanimation/gameanimationnextday.cpp ${COW_ROOT_DIR}/game/gameanimation/gameanimationnextday.h
    ${COW_ROOT_DIR}/game/gameanimation/battleanimation.cpp ${COW_ROOT_DIR}/game/gameanimation/battleanimation.h
    ${COW_ROOT_DIR}/game/gameanimation/battleanimationsprite.cpp ${COW_ROOT_DIR}/game/gameanimation/battleanimationsprite.h
    ${COW_ROOT_DIR}/game/gameanimation/animationskipper.h ${COW_ROOT_DIR}/game/gameanimation/animationskipper.cpp
    # replay/observer
    ${COW_ROOT_DIR}/game/viewplayer.cpp ${COW_ROOT_DIR}/game/viewplayer.h

    # terrain flow data
    ${COW_ROOT_DIR}/game/jsData/terrainflowdata.cpp ${COW_ROOT_DIR}/game/jsData/terrainflowdata.h
    ${COW_ROOT_DIR}/game/jsData/campaignmapdata.cpp ${COW_ROOT_DIR}/game/jsData/campaignmapdata.h

    #ui
    ${COW_ROOT_DIR}/game/ui/playerinfo.cpp ${COW_ROOT_DIR}/game/ui/playerinfo.h
    ${COW_ROOT_DIR}/game/ui/ingameinfobar.cpp ${COW_ROOT_DIR}/game/ui/ingameinfobar.h
    ${COW_ROOT_DIR}/game/ui/copowermeter.cpp ${COW_ROOT_DIR}/game/ui/copowermeter.h
    ${COW_ROOT_DIR}/game/ui/customcoboostinfo.cpp ${COW_ROOT_DIR}/game/ui/customcoboostinfo.h
    ${COW_ROOT_DIR}/game/ui/humanquickbuttons.cpp ${COW_ROOT_DIR}/game/ui/humanquickbuttons.h
    ${COW_ROOT_DIR}/game/ui/damagecalculator.cpp ${COW_ROOT_DIR}/game/ui/damagecalculator.h
    ${COW_ROOT_DIR}/game/ui/movementplanneraddin.h ${COW_ROOT_DIR}/game/ui/movementplanneraddin.cpp

    # game input
    ${COW_ROOT_DIR}/gameinput/basegameinputif.cpp ${COW_ROOT_DIR}/gameinput/basegameinputif.h
    ${COW_ROOT_DIR}/gameinput/humanplayerinput.cpp ${COW_ROOT_DIR}/gameinput/humanplayerinput.h
    ${COW_ROOT_DIR}/gameinput/humanplayerinputmenu.cpp ${COW_ROOT_DIR}/gameinput/humanplayerinputmenu.h
    ${COW_ROOT_DIR}/gameinput/menudata.cpp ${COW_ROOT_DIR}/gameinput/menudata.h
    ${COW_ROOT_DIR}/gameinput/markedfielddata.cpp ${COW_ROOT_DIR}/gameinput/markedfielddata.h
    ${COW_ROOT_DIR}/gameinput/cursordata.cpp ${COW_ROOT_DIR}/gameinput/cursordata.h
    ${COW_ROOT_DIR}/gameinput/mapmover.cpp ${COW_ROOT_DIR}/gameinput/mapmover.h
    ${COW_ROOT_DIR}/gameinput/moveplannerinput.h ${COW_ROOT_DIR}/gameinput/moveplannerinput.cpp

    # map importing/exporting support
    # and resizing etc.
    ${COW_ROOT_DIR}/mapsupport/importcowtxt.cpp
    ${COW_ROOT_DIR}/mapsupport/refactorMap.cpp
    ${COW_ROOT_DIR}/mapsupport/randomMapGenerator.cpp ${COW_ROOT_DIR}/mapsupport/randomMapGenerator.h
    ${COW_ROOT_DIR}/mapsupport/importexport_awds.cpp
    ${COW_ROOT_DIR}/mapsupport/importexport_awdc.cpp
    ${COW_ROOT_DIR}/mapsupport/importawbyweb_text.cpp
    ${COW_ROOT_DIR}/mapsupport/mapfilter.cpp ${COW_ROOT_DIR}/mapsupport/mapfilter.h

    # ai
    ${COW_ROOT_DIR}/ai/coreai.cpp ${COW_ROOT_DIR}/ai/coreai.h
    ${COW_ROOT_DIR}/ai/veryeasyai.cpp ${COW_ROOT_DIR}/ai/veryeasyai.h
    ${COW_ROOT_DIR}/ai/targetedunitpathfindingsystem.cpp ${COW_ROOT_DIR}/ai/targetedunitpathfindingsystem.h
    ${COW_ROOT_DIR}/ai/islandmap.cpp ${COW_ROOT_DIR}/ai/islandmap.h
    ${COW_ROOT_DIR}/ai/coreai_predefinedai.cpp
    ${COW_ROOT_DIR}/ai/proxyai.cpp ${COW_ROOT_DIR}/ai/proxyai.h
    ${COW_ROOT_DIR}/ai/normalai.cpp ${COW_ROOT_DIR}/ai/normalai.h
    ${COW_ROOT_DIR}/ai/transporterselector.h ${COW_ROOT_DIR}/ai/transporterselector.cpp
    ${COW_ROOT_DIR}/ai/influencefrontmap.cpp ${COW_ROOT_DIR}/ai/influencefrontmap.h
    ${COW_ROOT_DIR}/ai/heavyai/heavyai.cpp ${COW_ROOT_DIR}/ai/heavyai/heavyai.h
    ${COW_ROOT_DIR}/ai/heavyai/heavyAiSharedData.h
    ${COW_ROOT_DIR}/ai/heavyai/situationevaluator.h ${COW_ROOT_DIR}/ai/heavyai/situationevaluator.cpp
    ${COW_ROOT_DIR}/ai/heavyai/simulationmap.h ${COW_ROOT_DIR}/ai/heavyai/simulationmap.cpp
    ${COW_ROOT_DIR}/ai/heavyai/unittargetedpathfindingsystem.h ${COW_ROOT_DIR}/ai/heavyai/unittargetedpathfindingsystem.cpp
    ${COW_ROOT_DIR}/ai/heavyai/heavyaitrainingdatagenerator.h ${COW_ROOT_DIR}/ai/heavyai/heavyaitrainingdatagenerator.cpp
    ${COW_ROOT_DIR}/ai/dummyai.h ${COW_ROOT_DIR}/ai/dummyai.cpp
    ${COW_ROOT_DIR}/ai/capturebuildingselector.h ${COW_ROOT_DIR}/ai/capturebuildingselector.cpp
    ${COW_ROOT_DIR}/ai/aiprocesspipe.h ${COW_ROOT_DIR}/ai/aiprocesspipe.cpp
    ${COW_ROOT_DIR}/ai/trainingdatagenerator.h ${COW_ROOT_DIR}/ai/trainingdatagenerator.cpp
    # production system
    ${COW_ROOT_DIR}/ai/productionSystem/productionactiondata.h ${COW_ROOT_DIR}/ai/productionSystem/productionactiondata.cpp
    ${COW_ROOT_DIR}/ai/productionSystem/simpleproductionsystem.h ${COW_ROOT_DIR}/ai/productionSystem/simpleproductionsystem.cpp

    # decision tree
    ${COW_ROOT_DIR}/ai/decisiontree/leaf.cpp ${COW_ROOT_DIR}/ai/decisiontree/leaf.h
    ${COW_ROOT_DIR}/ai/decisiontree/decisionnode.cpp ${COW_ROOT_DIR}/ai/decisiontree/decisionnode.h
    ${COW_ROOT_DIR}/ai/decisiontree/question.cpp ${COW_ROOT_DIR}/ai/decisiontree/question.h
    ${COW_ROOT_DIR}/ai/decisiontree/decisionquestion.cpp ${COW_ROOT_DIR}/ai/decisiontree/decisionquestion.h
    ${COW_ROOT_DIR}/ai/decisiontree/decisiontree.cpp ${COW_ROOT_DIR}/ai/decisiontree/decisiontree.h

    # multiplayer
    ${COW_ROOT_DIR}/multiplayer/lobbymenu.cpp ${COW_ROOT_DIR}/multiplayer/lobbymenu.h
    ${COW_ROOT_DIR}/multiplayer/multiplayermenu.cpp ${COW_ROOT_DIR}/multiplayer/multiplayermenu.h
    ${COW_ROOT_DIR}/multiplayer/gamedata.cpp ${COW_ROOT_DIR}/multiplayer/gamedata.h
    ${COW_ROOT_DIR}/multiplayer/password.cpp ${COW_ROOT_DIR}/multiplayer/password.h
    ${COW_ROOT_DIR}/multiplayer/totp.cpp ${COW_ROOT_DIR}/multiplayer/totp.h
    ${COW_ROOT_DIR}/multiplayer/dialogpasswordandadress.cpp ${COW_ROOT_DIR}/multiplayer/dialogpasswordandadress.h
    ${COW_ROOT_DIR}/multiplayer/dialogpassword.cpp ${COW_ROOT_DIR}/multiplayer/dialogpassword.h
    ${COW_ROOT_DIR}/multiplayer/networkgamedataview.h ${COW_ROOT_DIR}/multiplayer/networkgamedataview.cpp
    ${COW_ROOT_DIR}/multiplayer/dialogotherlobbyinfo.h ${COW_ROOT_DIR}/multiplayer/dialogotherlobbyinfo.cpp
    ${COW_ROOT_DIR}/multiplayer/dialogcostatsinfo.cpp ${COW_ROOT_DIR}/multiplayer/dialogcostatsinfo.h
    ${COW_ROOT_DIR}/multiplayer/dialogAutoMatches.cpp ${COW_ROOT_DIR}/multiplayer/dialogAutoMatches.h
    ${COW_ROOT_DIR}/multiplayer/dialogselectdownloadmap.cpp ${COW_ROOT_DIR}/multiplayer/dialogselectdownloadmap.h
    ${COW_ROOT_DIR}/multiplayer/dialogselectdownloadrecord.cpp ${COW_ROOT_DIR}/multiplayer/dialogselectdownloadrecord.h
    ${COW_ROOT_DIR}/multiplayer/networkcommands.h

    #campaign support
    ${COW_ROOT_DIR}/ingamescriptsupport/campaigneditor.cpp ${COW_ROOT_DIR}/ingamescriptsupport/campaigneditor.h

    # script support
    ${COW_ROOT_DIR}/ingamescriptsupport/scripteditor.cpp ${COW_ROOT_DIR}/ingamescriptsupport/scripteditor.h
    ${COW_ROOT_DIR}/ingamescriptsupport/genericbox.cpp ${COW_ROOT_DIR}/ingamescriptsupport/genericbox.h
    ${COW_ROOT_DIR}/ingamescriptsupport/scriptdialogdialog.cpp ${COW_ROOT_DIR}/ingamescriptsupport/scriptdialogdialog.h
    ${COW_ROOT_DIR}/ingamescriptsupport/scriptdata.cpp ${COW_ROOT_DIR}/ingamescriptsupport/scriptdata.h
    # condition stuff
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptcondition.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptcondition.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionvictory.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionvictory.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionstartofturn.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionstartofturn.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditioneachday.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditioneachday.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionunitdestroyed.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionunitdestroyed.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionterraindestroyed.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionterraindestroyed.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingcaptured.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingcaptured.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionplayerdefeated.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionplayerdefeated.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionunitsdestroyed.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionunitsdestroyed.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingsowned.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionbuildingsowned.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionplayerreachedarea.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionplayerreachedarea.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditioncheckvariable.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditioncheckvariable.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionisco.cpp ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditionisco.h
    ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditiongatheredfunds.h ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptconditiongatheredfunds.cpp
    # event stuff
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scriptevent.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scriptevent.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventdialog.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventdialog.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventspawnunit.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventspawnunit.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventdefeatplayer.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventdefeatplayer.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangebuildlist.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangebuildlist.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventaddfunds.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventaddfunds.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeweather.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeweather.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangecobar.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangecobar.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyunit.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyunit.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyterrain.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyterrain.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventanimation.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventanimation.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventvictoryinfo.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventvictoryinfo.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyvariable.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventmodifyvariable.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventgeneric.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventgeneric.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeunitai.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeunitai.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangebuildingowner.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangebuildingowner.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeunitowner.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeunitowner.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeplayerteam.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventchangeplayerteam.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventspawnbuilding.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventspawnbuilding.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventcentermap.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventcentermap.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventplaysound.cpp ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventplaysound.h
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventbuildingfirecounter.h ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventbuildingfirecounter.cpp
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventvolcanfire.h ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventvolcanfire.cpp
    ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventextendmap.h ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventextendmap.cpp

    # wiki stuff
    ${COW_ROOT_DIR}/wiki/terraininfo.cpp ${COW_ROOT_DIR}/wiki/terraininfo.h
    ${COW_ROOT_DIR}/wiki/unitinfo.cpp ${COW_ROOT_DIR}/wiki/unitinfo.h
    ${COW_ROOT_DIR}/wiki/fieldinfo.cpp ${COW_ROOT_DIR}/wiki/fieldinfo.h
    ${COW_ROOT_DIR}/wiki/wikidatabase.cpp ${COW_ROOT_DIR}/wiki/wikidatabase.h
    ${COW_ROOT_DIR}/wiki/wikipage.cpp ${COW_ROOT_DIR}/wiki/wikipage.h
    ${COW_ROOT_DIR}/wiki/defaultwikipage.cpp ${COW_ROOT_DIR}/wiki/defaultwikipage.h
    ${COW_ROOT_DIR}/wiki/wikiview.cpp ${COW_ROOT_DIR}/wiki/wikiview.h
    ${COW_ROOT_DIR}/wiki/damagetablepage.cpp ${COW_ROOT_DIR}/wiki/damagetablepage.h
    ${COW_ROOT_DIR}/wiki/actionwikipage.cpp ${COW_ROOT_DIR}/wiki/actionwikipage.h

    # table view
    ${COW_ROOT_DIR}/objects/tableView/basetableitem.h ${COW_ROOT_DIR}/objects/tableView/basetableitem.cpp
    ${COW_ROOT_DIR}/objects/tableView/stringtableitem.h ${COW_ROOT_DIR}/objects/tableView/stringtableitem.cpp
    ${COW_ROOT_DIR}/objects/tableView/xofytableitem.h ${COW_ROOT_DIR}/objects/tableView/xofytableitem.cpp
    ${COW_ROOT_DIR}/objects/tableView/locktableitem.h ${COW_ROOT_DIR}/objects/tableView/locktableitem.cpp
    ${COW_ROOT_DIR}/objects/tableView/complextableview.h ${COW_ROOT_DIR}/objects/tableView/complextableview.cpp

    # spriting support
    ${COW_ROOT_DIR}/spritingsupport/spritecreator.cpp ${COW_ROOT_DIR}/spritingsupport/spritecreator.h

    # ui
    ${COW_ROOT_DIR}/ui_reader/uifactory.cpp ${COW_ROOT_DIR}/ui_reader/uifactory.h
    ${COW_ROOT_DIR}/ui_reader/createdgui.cpp ${COW_ROOT_DIR}/ui_reader/createdgui.h


    ${COW_ROOT_DIR}/zipSupport/qzipreader.cpp ${COW_ROOT_DIR}/zipSupport/qzipreader.h
    ${COW_ROOT_DIR}/zipSupport/adler32.c
    ${COW_ROOT_DIR}/zipSupport/crc32.c ${COW_ROOT_DIR}/zipSupport/crc32.h
    ${COW_ROOT_DIR}/zipSupport/deflate.c ${COW_ROOT_DIR}/zipSupport/deflate.h
    ${COW_ROOT_DIR}/zipSupport/gzguts.h
    ${COW_ROOT_DIR}/zipSupport/gzlib.c
    ${COW_ROOT_DIR}/zipSupport/gzread.c
    ${COW_ROOT_DIR}/zipSupport/inflate.c ${COW_ROOT_DIR}/zipSupport/inflate.h
    ${COW_ROOT_DIR}/zipSupport/inftrees.c ${COW_ROOT_DIR}/zipSupport/inftrees.h
    ${COW_ROOT_DIR}/zipSupport/inffast.c ${COW_ROOT_DIR}/zipSupport/inffast.h
    ${COW_ROOT_DIR}/zipSupport/inffixed.h
    ${COW_ROOT_DIR}/zipSupport/trees.c ${COW_ROOT_DIR}/zipSupport/trees.h
    ${COW_ROOT_DIR}/zipSupport/zlib.h
    ${COW_ROOT_DIR}/zipSupport/uncompr.c
    ${COW_ROOT_DIR}/zipSupport/zutil.c ${COW_ROOT_DIR}/zipSupport/zutil.h

    # awbw replayer reader for ai training support
    ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayerreader.h ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayerreader.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwdataparser.h ${COW_ROOT_DIR}/awbwReplayReader/awbwdataparser.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayplayer.h ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayplayer.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwmapdownloader.h ${COW_ROOT_DIR}/awbwReplayReader/awbwmapdownloader.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwdatatypes.h ${COW_ROOT_DIR}/awbwReplayReader/awbwdatatypes.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwactionparser.h ${COW_ROOT_DIR}/awbwReplayReader/awbwactionparser.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwrecordcreator.h ${COW_ROOT_DIR}/awbwReplayReader/awbwrecordcreator.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwreplaydownloader.h ${COW_ROOT_DIR}/awbwReplayReader/awbwreplaydownloader.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayscandownloader.h ${COW_ROOT_DIR}/awbwReplayReader/awbwreplayscandownloader.cpp
    ${COW_ROOT_DIR}/awbwReplayReader/iAwbwAction.h

    # co generator
    ${COW_ROOT_DIR}/coGenerator/cogeneratormenu.h ${COW_ROOT_DIR}/coGenerator/cogeneratormenu.cpp
    ${COW_ROOT_DIR}/coGenerator/coability.h ${COW_ROOT_DIR}/coGenerator/coability.cpp
)

##############################################################################
# OS Specific files #
######################################
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(Commander_Wars_OS_SRCS
        ${COW_ROOT_DIR}/coreengine/windows/crashreporter_os.cpp
        ${COW_ROOT_DIR}/coreengine/windows/Gamepad.cpp
    )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    set(Commander_Wars_OS_SRCS
        ${COW_ROOT_DIR}/coreengine/android/crashreporter_os.cpp
        ${COW_ROOT_DIR}/coreengine/android/Gamepad.cpp
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(Commander_Wars_OS_SRCS
        ${COW_ROOT_DIR}/coreengine/ios/crashreporter_os.cpp
        ${COW_ROOT_DIR}/coreengine/ios/Gamepad.cpp
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(Commander_Wars_OS_SRCS
        ${COW_ROOT_DIR}/coreengine/linux/crashreporter_os.cpp
        ${COW_ROOT_DIR}/coreengine/linux/Gamepad.cpp
    )
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

##############################################################################
# Lib setup                                                                  #
##############################################################################

# link libraries
set(QT_LIBRARIES
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    Qt6::Widgets
    Qt6::Xml
    Qt6::Sql
    ${OPENSSL_LIBS}
    )

if (AUDIOSUPPORT)
    set(QT_LIBRARIES
        ${QT_LIBRARIES}
        portaudio
        opusfile
        opus
        ogg
    )
endif()

if (SCRIPTSOURCESUPPORT)
    set(QT_LIBRARIES
        ${QT_LIBRARIES}
        Qt6::QmlPrivate
    )
endif()

if (GRAPHICSUPPORT)
    set(QT_LIBRARIES
        ${QT_LIBRARIES}
        Qt6::OpenGL
    )
endif()

###################################################################################
# Precompiled header support
###################################################################################
# Sets up the shared precompiled headers on the given target.
# Note: Editing any of the listed files will trigger a FULL rebuild of the
#       target. The list is rather conservative for that reason and includes
#       mostly dependencies and very common headers in Commander Wars.
function(cow_setup_precompiled_headers targetName)
    set(CowOxygineHeaders ${Commander_Wars_OXYGINE_SRCS})
    list(FILTER CowOxygineHeaders INCLUDE REGEX "^.*\.hh?$")

    set(CowResourceManagementHeaders ${Commander_Wars_SRCS})
    list(FILTER CowResourceManagementHeaders INCLUDE REGEX "^.*\.hh?$")
    list(FILTER CowResourceManagementHeaders INCLUDE REGEX "resource_management")

    set(CowObjectsBaseHeaders ${Commander_Wars_SRCS})
    list(FILTER CowObjectsBaseHeaders INCLUDE REGEX "^.*\.hh?$")
    list(FILTER CowObjectsBaseHeaders INCLUDE REGEX "objects/base/")

    set(AppHeadersCxx
        ${CowOxygineHeaders}
        ${CowResourceManagementHeaders}
        ${CowObjectsBaseHeaders}

        ${COW_ROOT_DIR}/ai/coreai.h
        ${COW_ROOT_DIR}/coreengine/audiomanager.h
        ${COW_ROOT_DIR}/coreengine/fileserializable.h
        ${COW_ROOT_DIR}/coreengine/filesupport.h
        ${COW_ROOT_DIR}/coreengine/gameconsole.h
        ${COW_ROOT_DIR}/coreengine/globalutils.h
        ${COW_ROOT_DIR}/coreengine/interpreter.h
        ${COW_ROOT_DIR}/coreengine/jsthis.h
        ${COW_ROOT_DIR}/coreengine/mainapp.h
        ${COW_ROOT_DIR}/coreengine/memorymanagement.h
        ${COW_ROOT_DIR}/coreengine/qmlvector.h
        ${COW_ROOT_DIR}/coreengine/scriptvariables.h
        ${COW_ROOT_DIR}/coreengine/settings.h
        ${COW_ROOT_DIR}/coreengine/userdata.h
        ${COW_ROOT_DIR}/coreengine/virtualpaths.h
        ${COW_ROOT_DIR}/game/building.h
        ${COW_ROOT_DIR}/game/co.h
        ${COW_ROOT_DIR}/game/gameaction.h
        ${COW_ROOT_DIR}/game/GameEnums.h
        ${COW_ROOT_DIR}/game/gamemap.h
        ${COW_ROOT_DIR}/game/player.h
        ${COW_ROOT_DIR}/game/terrain.h
        ${COW_ROOT_DIR}/game/unit.h
        ${COW_ROOT_DIR}/ingamescriptsupport/conditions/scriptcondition.h
        ${COW_ROOT_DIR}/ingamescriptsupport/events/scripteventgeneric.h
        ${COW_ROOT_DIR}/ingamescriptsupport/events/scriptevent.h
        ${COW_ROOT_DIR}/ingamescriptsupport/genericbox.h
        ${COW_ROOT_DIR}/ingamescriptsupport/scriptdata.h
        ${COW_ROOT_DIR}/ingamescriptsupport/scripteditor.h
        ${COW_ROOT_DIR}/menue/basegamemenu.h
        ${COW_ROOT_DIR}/menue/basemenu.h
        ${COW_ROOT_DIR}/menue/gamemenue.h
        ${COW_ROOT_DIR}/menue/mainwindow.h
        ${COW_ROOT_DIR}/multiplayer/networkcommands.h
        ${COW_ROOT_DIR}/network/JsonKeys.h
        ${COW_ROOT_DIR}/network/mainserver.h
        ${COW_ROOT_DIR}/network/networkInterface.h
        ${COW_ROOT_DIR}/objects/dialogs/customdialog.h
        ${COW_ROOT_DIR}/objects/dialogs/dialogmessagebox.h
        ${COW_ROOT_DIR}/objects/dialogs/filedialog.h
        ${COW_ROOT_DIR}/objects/dialogs/mapfiledialog.h
        ${COW_ROOT_DIR}/ui_reader/createdgui.h
        ${COW_ROOT_DIR}/ui_reader/uifactory.h
        ${COW_ROOT_DIR}/wiki/wikidatabase.h
    )

    set(SystemHeaderSources # Every header used 5 or more times.
        <algorithm$<ANGLE-R>
        <bench/BenchTimer.h$<ANGLE-R>
        <chrono$<ANGLE-R>
        <cmath$<ANGLE-R>
        <complex$<ANGLE-R>
        <cstdlib$<ANGLE-R>
        <cstring$<ANGLE-R>
        <ctime$<ANGLE-R>
        <exception$<ANGLE-R>
        <fstream$<ANGLE-R>
        <functional$<ANGLE-R>
        <iomanip$<ANGLE-R>
        <iostream$<ANGLE-R>
        <limits$<ANGLE-R>
        <limits.h$<ANGLE-R>
        <list$<ANGLE-R>
        <map$<ANGLE-R>
        <math.h$<ANGLE-R>
        <memory$<ANGLE-R>
        <numeric$<ANGLE-R>
        <QApplication$<ANGLE-R>
        <QBuffer$<ANGLE-R>
        <QByteArray$<ANGLE-R>
        <QColor$<ANGLE-R>
        <QCoreApplication$<ANGLE-R>
        <QCryptographicHash$<ANGLE-R>
        <QDataStream$<ANGLE-R>
        <QDateTime$<ANGLE-R>
        <QDir$<ANGLE-R>
        <QDirIterator$<ANGLE-R>
        <QDomDocument$<ANGLE-R>
        <QElapsedTimer$<ANGLE-R>
        <QFile$<ANGLE-R>
        <QFileInfo$<ANGLE-R>
        <QImage$<ANGLE-R>
        <QIODevice$<ANGLE-R>
        <QJsonArray$<ANGLE-R>
        <QJsonDocument$<ANGLE-R>
        <QJsonObject$<ANGLE-R>
        <QKeyEvent$<ANGLE-R>
        <QList$<ANGLE-R>
        <QObject$<ANGLE-R>
        <QPoint$<ANGLE-R>
        <QProcess$<ANGLE-R>
        <QRect$<ANGLE-R>
        <QString$<ANGLE-R>
        <QStringList$<ANGLE-R>
        <QTextStream$<ANGLE-R>
        <QThread$<ANGLE-R>
        <QTimer$<ANGLE-R>
        <QtMath$<ANGLE-R>
        <QVector$<ANGLE-R>
        <random$<ANGLE-R>
        <regex$<ANGLE-R>
        <sstream$<ANGLE-R>
        <stddef.h$<ANGLE-R>
        <stdexcept$<ANGLE-R>
        <stdint.h$<ANGLE-R>
        <stdio.h$<ANGLE-R>
        <stdlib.h$<ANGLE-R>
        <string$<ANGLE-R>
        <time.h$<ANGLE-R>
        <typeinfo$<ANGLE-R>
        <utility$<ANGLE-R>
        <vector$<ANGLE-R>
    )

    target_precompile_headers(${targetName}
        PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:${AppHeadersCxx}>
        $<$<COMPILE_LANGUAGE:CXX>:${SystemHeaderSources}>
    )
    set_source_files_properties(${targetName}_autogen/mocs_compilation.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endfunction()
