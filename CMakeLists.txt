cmake_minimum_required (VERSION 3.19.0)
project (Commander_Wars LANGUAGES CXX C)

# Information for building commander wars on other platforms
# 1. make sure the CMAKE_PREFIX_PATH targets to the correct qt installation
# 2. make sure the LIBRARY_PATH targets the library folder of qt e.g. LD_LIBRARY_PATH=$HOME/Qt/5.15.2/gcc_64/lib
# 3. make sure that -DOPENSSL_ROOT_DIR= targets your local openssl installation e.g. -DOPENSSL_ROOT_DIR=D:/Qt/Tools/OpenSSL

# Special Information for Android
# ANDROID_NATIVE_API_LEVEL needs to be 27
# NDK Version needs to be: 21.3.6528147 -> This needs to be matched exactly

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set tool chain info
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
option(OPENSSL_USE_STATIC_LIBS "you may disable linking static against openssl but it's not recommended. The installation process won't deploy the libs." TRUE)
option(OPENSSL_MSVC_STATIC_RT "you may disable linking static against openssl but it's not recommended. The installation process won't deploy the libs." TRUE)
option(AUDIOSUPPORT "enables audio support for the game and compiles music/sound into the exe" ON)
option(GRAPHICSUPPORT "enables sprites for the game and compiles them into the exe" ON)
option(USEVULKAN "This swaps from opengl to vulkan rendering. Incompatible with GRAPHICSUPPORT OFF. In development don't use" OFF)
option(PRECOMPILEDOPENSSL "if the openssl are precompiled libs or or package exists" OFF)
option(USEAPPCONFIGPATH "use the system location for saving data, instead of the working directory" OFF)
option(DEPLOY_RESOURCES_AS_FOLDER "if files should be compiled into the exe or deployed as folder" OFF)
option(UPDATESUPPORT "Enables the automatic updater for the game which updates to the latest github version should be turned off for local builds not supported for android" OFF)
option(DEFAULTAIPIPE "If the system should use a separate process for the ai as default variant. Disable it for system that can't launch the same exe twice e.g. android." ON)
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
if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
endif()
# Retrieve the absolute path to qmake and then use that path to find
# the binaries
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
endif()

###################################################################################
# OpenSsl
###################################################################################

if(PRECOMPILEDOPENSSL AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    message("Using local pre-compiled openssl version from path: ${OPENSSL_LIB_PATH}")
    set(OPENSSL_LIBS
        ${OPENSSL_LIB_PATH}/libcrypto_${ANDROID_ABI}.a
        ${OPENSSL_LIB_PATH}/libssl_${ANDROID_ABI}.a
    CACHE INTERNAL "")
    include_directories(${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL Include directories:" ${OPENSSL_INCLUDE_DIR})
    message ("OpenSsL libs:" ${OPENSSL_LIBS})
else()
    message("Using openssl package")
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
        -DDEBUG_LEVEL=1             # default console log level
    )
endif()

if (AUDIOSUPPORT)
    message("Building with Audio")
    find_package(Qt6 COMPONENTS Multimedia REQUIRED)
    add_definitions(
        -DAUDIOSUPPORT
    )
else()
    message("Building without Audio")
endif()

if(GRAPHICSUPPORT)
    if (USEVULKAN)
        message("Building with vulkan")
        find_package(Vulkan REQUIRED)
    else()
        message("Building with OpenGl")
        find_package(Qt6 COMPONENTS OpenGL REQUIRED)
    endif()
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
    -DVERSION_MINOR=31
    -DVERSION_REVISION=2
    -DVERSION_SUFFIX="main"
    -DCOW_BUILD_TAG="${COW_BUILD_TAG}"
    )

set(COW_ANDROID_VERSION_NAME "0.31.2-main")
set(COW_ANDROID_VERSION_CODE "38")

###################################################################################
# Set up some compiler and linking options
###################################################################################

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj ${CMAKE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "-Wa,-mbig-obj ${CMAKE_C_FLAGS}")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    if ("${ANDROID_ABI}" STREQUAL armeabi-v7a)
        add_link_options("-long-plt")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

###################################################################################
# start oxygine stuff -> qoxygine
###################################################################################
set(${PROJECT_NAME}_OXYGINE_SRCS
    # top level
    3rd_party/oxygine-framework/oxygine/AnimationFrame.cpp 3rd_party/oxygine-framework/oxygine/AnimationFrame.h
    3rd_party/oxygine-framework/oxygine/Clock.cpp 3rd_party/oxygine-framework/oxygine/Clock.h
    3rd_party/oxygine-framework/oxygine/Draggable.cpp 3rd_party/oxygine-framework/oxygine/Draggable.h
    3rd_party/oxygine-framework/oxygine/EventDispatcher.cpp 3rd_party/oxygine-framework/oxygine/EventDispatcher.h
    3rd_party/oxygine-framework/oxygine/Input.cpp 3rd_party/oxygine-framework/oxygine/Input.h
    3rd_party/oxygine-framework/oxygine/Material.cpp 3rd_party/oxygine-framework/oxygine/Material.h
    3rd_party/oxygine-framework/oxygine/MaterialCache.cpp 3rd_party/oxygine-framework/oxygine/MaterialCache.h
    3rd_party/oxygine-framework/oxygine/PointerState.cpp 3rd_party/oxygine-framework/oxygine/PointerState.h
    3rd_party/oxygine-framework/oxygine/RenderDelegate.cpp 3rd_party/oxygine-framework/oxygine/RenderDelegate.h
    3rd_party/oxygine-framework/oxygine/STDRenderer.cpp 3rd_party/oxygine-framework/oxygine/STDRenderer.h    
    3rd_party/oxygine-framework/oxygine/RenderState.h
    3rd_party/oxygine-framework/oxygine/oxygine-forwards.h
    3rd_party/oxygine-framework/oxygine/TextStyle.h
    #core
    3rd_party/oxygine-framework/oxygine/core/oxygine.cpp 3rd_party/oxygine-framework/oxygine/core/oxygine.h
    3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h
    3rd_party/oxygine-framework/oxygine/core/gamewindow.cpp 3rd_party/oxygine-framework/oxygine/core/gamewindow.h
    3rd_party/oxygine-framework/oxygine/core/ref_counter.cpp 3rd_party/oxygine-framework/oxygine/core/ref_counter.h
    3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h
    3rd_party/oxygine-framework/oxygine/core/VideoDriver.h
    # actor
    3rd_party/oxygine-framework/oxygine/actor/Actor.cpp 3rd_party/oxygine-framework/oxygine/actor/Actor.h
    3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.cpp 3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h
    3rd_party/oxygine-framework/oxygine/actor/Button.cpp 3rd_party/oxygine-framework/oxygine/actor/Button.h
    3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.cpp 3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h
    3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.cpp 3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h
    3rd_party/oxygine-framework/oxygine/actor/SlidingActor.cpp 3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h
    3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.cpp 3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.h
    3rd_party/oxygine-framework/oxygine/actor/Sprite.cpp 3rd_party/oxygine-framework/oxygine/actor/Sprite.h
    3rd_party/oxygine-framework/oxygine/actor/Stage.cpp 3rd_party/oxygine-framework/oxygine/actor/Stage.h
    3rd_party/oxygine-framework/oxygine/actor/TextField.cpp 3rd_party/oxygine-framework/oxygine/actor/TextField.h
    3rd_party/oxygine-framework/oxygine/actor/slidingsprite.cpp 3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h
    3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.cpp 3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h
    # tween
    3rd_party/oxygine-framework/oxygine/tween/Tween.cpp 3rd_party/oxygine-framework/oxygine/tween/Tween.h
    3rd_party/oxygine-framework/oxygine/tween/TweenAnim.cpp 3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h
    3rd_party/oxygine-framework/oxygine/tween/TweenAnimColumn.cpp 3rd_party/oxygine-framework/oxygine/tween/TweenAnimColumn.h
    3rd_party/oxygine-framework/oxygine/tween/TweenQueue.cpp 3rd_party/oxygine-framework/oxygine/tween/TweenQueue.h
    3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.cpp 3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h
    3rd_party/oxygine-framework/oxygine/tween/tweenwait.cpp 3rd_party/oxygine-framework/oxygine/tween/tweenwait.h
    3rd_party/oxygine-framework/oxygine/tween/tweenscreenshake.cpp 3rd_party/oxygine-framework/oxygine/tween/tweenscreenshake.h
    3rd_party/oxygine-framework/oxygine/tween/tweenshakey.cpp 3rd_party/oxygine-framework/oxygine/tween/tweenshakey.h
    3rd_party/oxygine-framework/oxygine/tween/tweenchangenumbertext.h 3rd_party/oxygine-framework/oxygine/tween/tweenchangenumbertext.cpp
    # res stuff
    3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.cpp 3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h
    3rd_party/oxygine-framework/oxygine/res/ResAnim.cpp 3rd_party/oxygine-framework/oxygine/res/ResAnim.h
    3rd_party/oxygine-framework/oxygine/res/ResAtlas.cpp 3rd_party/oxygine-framework/oxygine/res/ResAtlas.h
    3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.cpp 3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h
    3rd_party/oxygine-framework/oxygine/res/Resource.cpp 3rd_party/oxygine-framework/oxygine/res/Resource.h
    3rd_party/oxygine-framework/oxygine/res/Resources.cpp 3rd_party/oxygine-framework/oxygine/res/Resources.h
    3rd_party/oxygine-framework/oxygine/res/SingleResAnim.cpp 3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h
    # text utils
    3rd_party/oxygine-framework/oxygine/text_utils/Aligner.cpp 3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h
    3rd_party/oxygine-framework/oxygine/text_utils/Node.cpp 3rd_party/oxygine-framework/oxygine/text_utils/Node.h
    3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.cpp 3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h
    # closure
    3rd_party/oxygine-framework/oxygine/core/closure.h
    # math
    3rd_party/oxygine-framework/oxygine/math/ScalarMath.h
)

if (GRAPHICSUPPORT)
    if (USEVULKAN)
        include_directories("3rd_party/oxygine-framework/oxygine/core/vulkan")
        set(${PROJECT_NAME}_OXYGINE_SRCS
            ${${PROJECT_NAME}_OXYGINE_SRCS}
            3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.cpp 3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h
            3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.cpp 3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.h
            3rd_party/oxygine-framework/oxygine/core/vulkan/texture.cpp 3rd_party/oxygine-framework/oxygine/core/vulkan/texture.h
            3rd_party/oxygine-framework/oxygine/core/vulkan/ShaderProgram.cpp 3rd_party/oxygine-framework/oxygine/core/vulkan/ShaderProgram.h
            3rd_party/oxygine-framework/oxygine/core/vulkan/VideoDriver.cpp
            3rd_party/oxygine-framework/oxygine/core/vulkan/UberShaderProgram.cpp
            game/gamerecording/vulkan/gamemapimagesaver.cpp
        )
    else()
        include_directories("3rd_party/oxygine-framework/oxygine/core/opengl")
        set(${PROJECT_NAME}_OXYGINE_SRCS
            ${${PROJECT_NAME}_OXYGINE_SRCS}
            3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.cpp 3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h
            3rd_party/oxygine-framework/oxygine/core/opengl/texture.cpp 3rd_party/oxygine-framework/oxygine/core/opengl/texture.h
            3rd_party/oxygine-framework/oxygine/core/opengl/ShaderProgram.cpp 3rd_party/oxygine-framework/oxygine/core/opengl/ShaderProgram.h
            3rd_party/oxygine-framework/oxygine/core/opengl/VideoDriver.cpp
            3rd_party/oxygine-framework/oxygine/core/opengl/UberShaderProgram.cpp
            game/gamerecording/opengl/gamemapimagesaver.cpp
        )
    endif()
else()
    include_directories("3rd_party/oxygine-framework/oxygine/core/none")
    set(${PROJECT_NAME}_OXYGINE_SRCS
        ${${PROJECT_NAME}_OXYGINE_SRCS}
        3rd_party/oxygine-framework/oxygine/core/none/windowBase.cpp 3rd_party/oxygine-framework/oxygine/core/none/windowBase.h
        3rd_party/oxygine-framework/oxygine/core/none/texture.cpp 3rd_party/oxygine-framework/oxygine/core/none/texture.h
        3rd_party/oxygine-framework/oxygine/core/none/ShaderProgram.cpp 3rd_party/oxygine-framework/oxygine/core/none/ShaderProgram.h
        3rd_party/oxygine-framework/oxygine/core/none/VideoDriver.cpp
        3rd_party/oxygine-framework/oxygine/core/none/UberShaderProgram.cpp
        game/gamerecording/none/gamemapimagesaver.cpp
    )
endif()

###################################################################################
# end oxygine stuff -> qoxygine
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
    include_directories("updater/zipSupport")
    set(${PROJECT_NAME}_UPDATE_SRCS
        updater/gameupdater.h updater/gameupdater.cpp
        updater/filedownloader.h updater/filedownloader.cpp
        updater/zipSupport/qzipreader.cpp updater/zipSupport/qzipreader.h
        updater/zipSupport/adler32.c
        updater/zipSupport/crc32.c updater/zipSupport/crc32.h
        updater/zipSupport/deflate.c updater/zipSupport/deflate.h
        updater/zipSupport/gzguts.h
        updater/zipSupport/gzlib.c
        updater/zipSupport/gzread.c
        updater/zipSupport/inflate.c updater/zipSupport/inflate.h
        updater/zipSupport/inftrees.c updater/zipSupport/inftrees.h
        updater/zipSupport/inffast.c updater/zipSupport/inffast.h
        updater/zipSupport/inffixed.h
        updater/zipSupport/trees.c updater/zipSupport/trees.h
        updater/zipSupport/zlib.h
        updater/zipSupport/uncompr.c
        updater/zipSupport/zutil.c updater/zipSupport/zutil.h
    )
else()
    message("Building without update support")
    set(${PROJECT_NAME}_UPDATE_SRCS
    )
endif()

###################################################################################
# start stmp client stuff
###################################################################################
set(${PROJECT_NAME}_SmtpClient_SRCS
    3rd_party/smtpClient/src/emailaddress.cpp 3rd_party/smtpClient/src/emailaddress.h
    3rd_party/smtpClient/src/mimeattachment.cpp 3rd_party/smtpClient/src/mimeattachment.h
    3rd_party/smtpClient/src/mimebase64encoder.cpp 3rd_party/smtpClient/src/mimebase64encoder.h
    3rd_party/smtpClient/src/mimebase64formatter.cpp 3rd_party/smtpClient/src/mimebase64formatter.h
    3rd_party/smtpClient/src/mimebytearrayattachment.cpp 3rd_party/smtpClient/src/mimebytearrayattachment.h
    3rd_party/smtpClient/src/mimecontentencoder.cpp 3rd_party/smtpClient/src/mimecontentencoder.h
    3rd_party/smtpClient/src/mimecontentformatter.cpp 3rd_party/smtpClient/src/mimecontentformatter.h
    3rd_party/smtpClient/src/mimefile.cpp 3rd_party/smtpClient/src/mimefile.h
    3rd_party/smtpClient/src/mimehtml.cpp 3rd_party/smtpClient/src/mimehtml.h
    3rd_party/smtpClient/src/mimeinlinefile.cpp 3rd_party/smtpClient/src/mimeinlinefile.h
    3rd_party/smtpClient/src/mimemessage.cpp 3rd_party/smtpClient/src/mimemessage.h
    3rd_party/smtpClient/src/mimemultipart.cpp 3rd_party/smtpClient/src/mimemultipart.h
    3rd_party/smtpClient/src/mimepart.cpp 3rd_party/smtpClient/src/mimepart.h
    3rd_party/smtpClient/src/mimeqpencoder.cpp 3rd_party/smtpClient/src/mimeqpencoder.h
    3rd_party/smtpClient/src/mimeqpformatter.cpp 3rd_party/smtpClient/src/mimeqpformatter.h
    3rd_party/smtpClient/src/mimetext.cpp 3rd_party/smtpClient/src/mimetext.h
    3rd_party/smtpClient/src/quotedprintable.cpp 3rd_party/smtpClient/src/quotedprintable.h
    3rd_party/smtpClient/src/smtpclient.cpp 3rd_party/smtpClient/src/smtpclient.h
    3rd_party/smtpClient/src/SmtpMime
    3rd_party/smtpClient/src/mimeattachment.h
)
###################################################################################
# end stmp client stuff
###################################################################################

###################################################################################
# source list of commander wars specific files
###################################################################################
set(${PROJECT_NAME}_SRCS
    main.cpp

    #objects --> i don't have a good place to put them right now
    objects/coinfoactor.cpp objects/coinfoactor.h
    objects/rotatingsprite.cpp objects/rotatingsprite.h
    objects/ruleselection.cpp objects/ruleselection.h
    objects/minimap.cpp objects/minimap.h
    objects/perkselection.cpp objects/perkselection.h
    objects/mapselectionview.cpp objects/mapselectionview.h
    objects/loadingscreen.cpp objects/loadingscreen.h
    objects/editorselection.cpp objects/editorselection.h
    objects/mapselection.cpp objects/mapselection.h
    objects/coselection.cpp objects/coselection.h
    objects/playerselection.cpp objects/playerselection.h
    objects/achievementbanner.cpp objects/achievementbanner.h
    objects/unitstatisticview.cpp objects/unitstatisticview.h
    # objects base
    objects/base/EventTextEdit.cpp objects/base/EventTextEdit.h
    objects/base/textinput.cpp objects/base/textinput.h
    objects/base/topbar.cpp objects/base/topbar.h
    objects/base/textbox.cpp objects/base/textbox.h
    objects/base/multilinetextbox.cpp objects/base/multilinetextbox.h
    objects/base/h_scrollbar.cpp objects/base/h_scrollbar.h
    objects/base/v_scrollbar.cpp objects/base/v_scrollbar.h
    objects/base/dropdownmenu.cpp objects/base/dropdownmenu.h
    objects/base/dropdownmenucolor.cpp objects/base/dropdownmenucolor.h
    objects/base/panel.cpp objects/base/panel.h
    objects/base/spinbox.cpp objects/base/spinbox.h
    objects/base/checkbox.cpp objects/base/checkbox.h
    objects/base/slider.cpp objects/base/slider.h
    objects/base/multislider.cpp objects/base/multislider.h
    objects/base/passwordbox.cpp objects/base/passwordbox.h
    objects/base/progressinfobar.cpp objects/base/progressinfobar.h
    objects/base/selectkey.cpp objects/base/selectkey.h
    objects/base/chat.cpp objects/base/chat.h
    objects/base/timespinbox.cpp objects/base/timespinbox.h
    objects/base/dropdownmenubase.cpp objects/base/dropdownmenubase.h
    objects/base/label.cpp objects/base/label.h
    objects/base/colorselector.cpp objects/base/colorselector.h
    objects/base/dropdownmenusprite.cpp objects/base/dropdownmenusprite.h
    objects/base/tooltip.cpp objects/base/tooltip.h
    objects/base/focusableobject.cpp objects/base/focusableobject.h
    objects/base/tableview.cpp objects/base/tableview.h
    objects/base/closeablepopup.cpp objects/base/closeablepopup.h
    objects/base/moveinbutton.cpp objects/base/moveinbutton.h
    objects/base/spriteobject.cpp objects/base/spriteobject.h
    # objects dialogs
    objects/dialogs/colorselectiondialog.cpp objects/dialogs/colorselectiondialog.h
    objects/dialogs/dialogconnecting.cpp objects/dialogs/dialogconnecting.h
    objects/dialogs/filedialog.cpp objects/dialogs/filedialog.h
    objects/dialogs/dialogcostyle.cpp objects/dialogs/dialogcostyle.h
    objects/dialogs/dialogmessagebox.cpp objects/dialogs/dialogmessagebox.h
    objects/dialogs/dialogtextinput.cpp objects/dialogs/dialogtextinput.h
    objects/dialogs/folderdialog.cpp objects/dialogs/folderdialog.h
    objects/dialogs/dialogvaluecounter.cpp objects/dialogs/dialogvaluecounter.h
    objects/dialogs/gamepadinfo.cpp objects/dialogs/gamepadinfo.h
    objects/dialogs/customdialog.h objects/dialogs/customdialog.cpp
    # objects dialogs editor
    objects/dialogs/editor/dialogmodifyunit.cpp objects/dialogs/editor/dialogmodifyunit.h
    objects/dialogs/editor/dialogmodifybuilding.cpp objects/dialogs/editor/dialogmodifybuilding.h
    objects/dialogs/editor/dialogmodifyterrain.cpp objects/dialogs/editor/dialogmodifyterrain.h
    objects/dialogs/editor/dialograndommap.cpp objects/dialogs/editor/dialograndommap.h
    objects/dialogs/editor/mapeditdialog.cpp objects/dialogs/editor/mapeditdialog.h
    objects/dialogs/editor/dialogviewmapstats.h objects/dialogs/editor/dialogviewmapstats.cpp
    # objects dialogs rules
    objects/dialogs/rules/actionlistdialog.cpp objects/dialogs/rules/actionlistdialog.h
    objects/dialogs/rules/buildlistdialog.cpp objects/dialogs/rules/buildlistdialog.h
    objects/dialogs/rules/cobannlistdialog.cpp objects/dialogs/rules/cobannlistdialog.h
    objects/dialogs/rules/coselectiondialog.cpp objects/dialogs/rules/coselectiondialog.h
    objects/dialogs/rules/perkselectiondialog.cpp objects/dialogs/rules/perkselectiondialog.h
    objects/dialogs/rules/playerselectiondialog.cpp objects/dialogs/rules/playerselectiondialog.h
    objects/dialogs/rules/ruleselectiondialog.cpp objects/dialogs/rules/ruleselectiondialog.h
    # objects dialogs ingame
    objects/dialogs/ingame/coinfodialog.cpp objects/dialogs/ingame/coinfodialog.h
    objects/dialogs/ingame/dialogvictoryconditions.cpp objects/dialogs/ingame/dialogvictoryconditions.h
    objects/dialogs/ingame/dialogattacklog.cpp objects/dialogs/ingame/dialogattacklog.h
    objects/dialogs/ingame/dialogunitinfo.cpp objects/dialogs/ingame/dialogunitinfo.h
    objects/dialogs/ingame/victoryrulepopup.cpp objects/dialogs/ingame/victoryrulepopup.h
    # objects dialogs map selection
    objects/dialogs/mapSelection/mapselectionfilterdialog.cpp objects/dialogs/mapSelection/mapselectionfilterdialog.h

    #modding
    modding/csvtableimporter.cpp modding/csvtableimporter.h

    #menues
    menue/basemenu.cpp menue/basemenu.h
    menue/basegamemenu.cpp menue/basegamemenu.h
    menue/mainwindow.cpp menue/mainwindow.h
    menue/editormenue.cpp menue/editormenue.h
    menue/gamemenue.cpp menue/gamemenue.h    
    menue/movementplanner.cpp menue/movementplanner.h
    menue/optionmenue.cpp menue/optionmenue.h
    menue/mapselectionmapsmenue.cpp menue/mapselectionmapsmenue.h
    menue/creditsmenue.cpp menue/creditsmenue.h
    menue/victorymenue.cpp menue/victorymenue.h
    menue/campaignmenu.cpp menue/campaignmenu.h
    menue/wikimenu.cpp menue/wikimenu.h
    menue/costylemenu.cpp menue/costylemenu.h
    menue/replaymenu.cpp menue/replaymenu.h
    menue/achievementmenu.cpp menue/achievementmenu.h
    menue/shopmenu.cpp menue/shopmenu.h
    menue/generatormenu.h menue/generatormenu.cpp

    # ressource management
    resource_management/unitspritemanager.cpp resource_management/unitspritemanager.h
    resource_management/terrainmanager.cpp resource_management/terrainmanager.h
    resource_management/fontmanager.cpp resource_management/fontmanager.h
    resource_management/backgroundmanager.cpp resource_management/backgroundmanager.h
    resource_management/objectmanager.cpp resource_management/objectmanager.h
    resource_management/buildingspritemanager.cpp resource_management/buildingspritemanager.h
    resource_management/movementtablemanager.cpp resource_management/movementtablemanager.h
    resource_management/gamemanager.cpp resource_management/gamemanager.h
    resource_management/gameanimationmanager.cpp resource_management/gameanimationmanager.h
    resource_management/weaponmanager.cpp resource_management/weaponmanager.h
    resource_management/cospritemanager.cpp resource_management/cospritemanager.h
    resource_management/gamerulemanager.cpp resource_management/gamerulemanager.h
    resource_management/battleanimationmanager.cpp resource_management/battleanimationmanager.h
    resource_management/coperkmanager.cpp resource_management/coperkmanager.h
    resource_management/achievementmanager.cpp resource_management/achievementmanager.h
    resource_management/shoploader.cpp resource_management/shoploader.h
    resource_management/movementplanneraddinmanager.h resource_management/movementplanneraddinmanager.cpp
    resource_management/uimanager.h resource_management/uimanager.cpp
    resource_management/ressourcemanagement.h

    # core engine
    coreengine/mainapp.cpp coreengine/mainapp.h
    coreengine/settings.cpp coreengine/settings.h
    coreengine/interpreter.cpp coreengine/interpreter.h
    coreengine/gameconsole.cpp coreengine/gameconsole.h
    coreengine/audiomanager.cpp coreengine/audiomanager.h
    coreengine/pathfindingsystem.cpp coreengine/pathfindingsystem.h
    coreengine/qmlvector.cpp coreengine/qmlvector.h
    coreengine/scriptvariables.cpp coreengine/scriptvariables.h
    coreengine/scriptvariable.cpp coreengine/scriptvariable.h
    coreengine/workerthread.cpp coreengine/workerthread.h
    coreengine/timer.cpp coreengine/timer.h
    coreengine/userdata.cpp coreengine/userdata.h
    coreengine/crashreporter.cpp coreengine/crashreporter.h
    coreengine/filesupport.cpp coreengine/filesupport.h
    coreengine/globalutils.cpp coreengine/globalutils.h
    coreengine/scriptvariablefile.cpp coreengine/scriptvariablefile.h
    coreengine/metatyperegister.cpp coreengine/metatyperegister.h
    coreengine/GamepadShared.cpp coreengine/Gamepad.h
    coreengine/commandlineparser.cpp coreengine/commandlineparser.h
    coreengine/JsCallback.h

    # network engine
    network/smtpmailsender.h network/smtpmailsender.cpp
    network/tcpclient.cpp network/tcpclient.h
    network/tcpserver.cpp network/tcpserver.h    
    network/localserver.cpp network/localserver.h
    network/localclient.cpp network/localclient.h
    network/txtask.cpp network/txtask.h
    network/rxtask.cpp network/rxtask.h
    network/NetworkInterface.h
    network/networkgamedata.cpp network/networkgamedata.h
    network/mainserver.cpp network/mainserver.h
    network/automatchmaker.h network/automatchmaker.cpp
    network/networkgame.cpp network/networkgame.h
    network/rsacypherhandler.h network/rsacypherhandler.cpp
    network/JsonKeys.h

    # game
    game/gamemap.cpp game/gamemap.h
    game/terrain.cpp game/terrain.h
    game/building.cpp game/building.h
    game/co.cpp game/co.h
    game/player.cpp game/player.h
    game/unit.cpp game/unit.h
    game/terrainfindingsystem.cpp game/terrainfindingsystem.h
    game/gameaction.cpp game/gameaction.h
    game/unitpathfindingsystem.cpp game/unitpathfindingsystem.h
    game/GameEnums.cpp game/GameEnums.h
    game/gamerules.cpp game/gamerules.h
    game/gamerule.cpp game/gamerule.h
    game/victoryrule.cpp game/victoryrule.h
    game/weather.cpp game/weather.h
    game/cursor.cpp game/cursor.h
    game/createoutline.cpp game/createoutline.h
    game/actionperformer.h game/actionperformer.cpp
    # cool ingame recording
    game/gamerecording/daytodayrecord.cpp game/gamerecording/daytodayrecord.h
    game/gamerecording/playerrecord.cpp game/gamerecording/playerrecord.h
    game/gamerecording/specialevent.cpp game/gamerecording/specialevent.h
    game/gamerecording/gamerecorder.cpp game/gamerecording/gamerecorder.h
    game/gamerecording/replayrecorder.cpp game/gamerecording/replayrecorder.h
    game/gamerecording/gamemapimagesaver.h

    # cool ingame script support
    game/gamescript.cpp game/gamescript.h
    game/campaign.cpp game/campaign.h
    # animation stuff
    game/gameanimation/gameanimation.cpp game/gameanimation/gameanimation.h
    game/gameanimation/gameanimationfactory.cpp game/gameanimation/gameanimationfactory.h
    game/gameanimation/gameanimationwalk.cpp game/gameanimation/gameanimationwalk.h
    game/gameanimation/gameanimationcapture.cpp game/gameanimation/gameanimationcapture.h
    game/gameanimation/gameanimationdialog.cpp game/gameanimation/gameanimationdialog.h
    game/gameanimation/gameanimationpower.cpp game/gameanimation/gameanimationpower.h
    game/gameanimation/gameanimationnextday.cpp game/gameanimation/gameanimationnextday.h
    game/gameanimation/battleanimation.cpp game/gameanimation/battleanimation.h
    game/gameanimation/battleanimationsprite.cpp game/gameanimation/battleanimationsprite.h
    game/gameanimation/animationskipper.h game/gameanimation/animationskipper.cpp
    # replay/observer
    game/viewplayer.cpp game/viewplayer.h

    # terrain flow data
    game/jsData/terrainflowdata.cpp game/jsData/terrainflowdata.h
    game/jsData/campaignmapdata.cpp game/jsData/campaignmapdata.h

    #ui
    game/ui/playerinfo.cpp game/ui/playerinfo.h
    game/ui/ingameinfobar.cpp game/ui/ingameinfobar.h
    game/ui/copowermeter.cpp game/ui/copowermeter.h
    game/ui/customcoboostinfo.cpp game/ui/customcoboostinfo.h
    game/ui/humanquickbuttons.cpp game/ui/humanquickbuttons.h
    game/ui/damagecalculator.cpp game/ui/damagecalculator.h
    game/ui/movementplanneraddin.h game/ui/movementplanneraddin.cpp

    # game input
    gameinput/basegameinputif.cpp gameinput/basegameinputif.h
    gameinput/humanplayerinput.cpp gameinput/humanplayerinput.h
    gameinput/humanplayerinputmenu.cpp gameinput/humanplayerinputmenu.h
    gameinput/menudata.cpp gameinput/menudata.h
    gameinput/markedfielddata.cpp gameinput/markedfielddata.h
    gameinput/cursordata.cpp gameinput/cursordata.h
    gameinput/mapmover.cpp gameinput/mapmover.h
    gameinput/moveplannerinput.h gameinput/moveplannerinput.cpp

    # map importing/exporting support
    # and resizing etc.
    mapsupport/importcowtxt.cpp
    mapsupport/refactorMap.cpp
    mapsupport/randomMapGenerator.cpp mapsupport/randomMapGenerator.h
    mapsupport/importexport_awds.cpp
    mapsupport/importexport_awdc.cpp
    mapsupport/importawbyweb_text.cpp
    mapsupport/mapfilter.cpp mapsupport/mapfilter.h

    # ai
    ai/coreai.cpp ai/coreai.h
    ai/veryeasyai.cpp ai/veryeasyai.h
    ai/targetedunitpathfindingsystem.cpp ai/targetedunitpathfindingsystem.h
    ai/islandmap.cpp ai/islandmap.h
    ai/coreai_predefinedai.cpp
    ai/proxyai.cpp ai/proxyai.h
    ai/normalai.cpp ai/normalai.h
    ai/heavyai.cpp ai/heavyai.h
    ai/heavyai/heavyaiproduction.cpp
    ai/heavyai/heavyaitraining.cpp
    ai/influencefrontmap.cpp ai/influencefrontmap.h
    ai/dummyai.h ai/dummyai.cpp
    ai/aiprocesspipe.h ai/aiprocesspipe.cpp
    # production system
    ai/productionSystem/simpleproductionsystem.h ai/productionSystem/simpleproductionsystem.cpp

    # neuralnetwork
    ai/neuralnetwork/neural/edge.cpp ai/neuralnetwork/neural/edge.h
    ai/neuralnetwork/neural/layer.cpp ai/neuralnetwork/neural/layer.h
    ai/neuralnetwork/neural/neuralnetwork.cpp ai/neuralnetwork/neural/neuralnetwork.h
    ai/neuralnetwork/neural/neuron.cpp ai/neuralnetwork/neural/neuron.h
    # decision tree
    ai/decisiontree/leaf.cpp ai/decisiontree/leaf.h
    ai/decisiontree/decisionnode.cpp ai/decisiontree/decisionnode.h
    ai/decisiontree/question.cpp ai/decisiontree/question.h
    ai/decisiontree/decisionquestion.cpp ai/decisiontree/decisionquestion.h
    ai/decisiontree/decisiontree.cpp ai/decisiontree/decisiontree.h

    # multiplayer
    multiplayer/lobbymenu.cpp multiplayer/lobbymenu.h
    multiplayer/multiplayermenu.cpp multiplayer/multiplayermenu.h
    multiplayer/gamedata.cpp multiplayer/gamedata.h
    multiplayer/password.cpp multiplayer/password.h
    multiplayer/dialogpasswordandadress.cpp multiplayer/dialogpasswordandadress.h
    multiplayer/dialogpassword.cpp multiplayer/dialogpassword.h
    multiplayer/networkgamedataview.h multiplayer/networkgamedataview.cpp
    multiplayer/networkcommands.h

    #campaign support
    ingamescriptsupport/campaigneditor.cpp ingamescriptsupport/campaigneditor.h

    # script support
    ingamescriptsupport/scripteditor.cpp ingamescriptsupport/scripteditor.h
    ingamescriptsupport/genericbox.cpp ingamescriptsupport/genericbox.h
    ingamescriptsupport/scriptdialogdialog.cpp ingamescriptsupport/scriptdialogdialog.h
    ingamescriptsupport/scriptdata.cpp ingamescriptsupport/scriptdata.h
    # condition stuff
    ingamescriptsupport/conditions/scriptcondition.cpp ingamescriptsupport/conditions/scriptcondition.h
    ingamescriptsupport/conditions/scriptconditionvictory.cpp ingamescriptsupport/conditions/scriptconditionvictory.h
    ingamescriptsupport/conditions/scriptconditionstartofturn.cpp ingamescriptsupport/conditions/scriptconditionstartofturn.h
    ingamescriptsupport/conditions/scriptconditioneachday.cpp ingamescriptsupport/conditions/scriptconditioneachday.h
    ingamescriptsupport/conditions/scriptconditionunitdestroyed.cpp ingamescriptsupport/conditions/scriptconditionunitdestroyed.h
    ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.cpp ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.h
    ingamescriptsupport/conditions/scriptconditionterraindestroyed.cpp ingamescriptsupport/conditions/scriptconditionterraindestroyed.h
    ingamescriptsupport/conditions/scriptconditionbuildingcaptured.cpp ingamescriptsupport/conditions/scriptconditionbuildingcaptured.h
    ingamescriptsupport/conditions/scriptconditionplayerdefeated.cpp ingamescriptsupport/conditions/scriptconditionplayerdefeated.h
    ingamescriptsupport/conditions/scriptconditionunitsdestroyed.cpp ingamescriptsupport/conditions/scriptconditionunitsdestroyed.h
    ingamescriptsupport/conditions/scriptconditionbuildingsowned.cpp ingamescriptsupport/conditions/scriptconditionbuildingsowned.h
    ingamescriptsupport/conditions/scriptconditionplayerreachedarea.cpp ingamescriptsupport/conditions/scriptconditionplayerreachedarea.h
    ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.cpp ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.h
    ingamescriptsupport/conditions/scriptconditioncheckvariable.cpp ingamescriptsupport/conditions/scriptconditioncheckvariable.h
    ingamescriptsupport/conditions/scriptconditionisco.cpp ingamescriptsupport/conditions/scriptconditionisco.h
    # event stuff
    ingamescriptsupport/events/scriptevent.cpp ingamescriptsupport/events/scriptevent.h
    ingamescriptsupport/events/scripteventdialog.cpp ingamescriptsupport/events/scripteventdialog.h
    ingamescriptsupport/events/scripteventspawnunit.cpp ingamescriptsupport/events/scripteventspawnunit.h
    ingamescriptsupport/events/scripteventdefeatplayer.cpp ingamescriptsupport/events/scripteventdefeatplayer.h
    ingamescriptsupport/events/scripteventchangebuildlist.cpp ingamescriptsupport/events/scripteventchangebuildlist.h
    ingamescriptsupport/events/scripteventaddfunds.cpp ingamescriptsupport/events/scripteventaddfunds.h
    ingamescriptsupport/events/scripteventchangeweather.cpp ingamescriptsupport/events/scripteventchangeweather.h
    ingamescriptsupport/events/scripteventchangecobar.cpp ingamescriptsupport/events/scripteventchangecobar.h
    ingamescriptsupport/events/scripteventmodifyunit.cpp ingamescriptsupport/events/scripteventmodifyunit.h
    ingamescriptsupport/events/scripteventmodifyterrain.cpp ingamescriptsupport/events/scripteventmodifyterrain.h
    ingamescriptsupport/events/scripteventanimation.cpp ingamescriptsupport/events/scripteventanimation.h
    ingamescriptsupport/events/scripteventvictoryinfo.cpp ingamescriptsupport/events/scripteventvictoryinfo.h
    ingamescriptsupport/events/scripteventmodifyvariable.cpp ingamescriptsupport/events/scripteventmodifyvariable.h
    ingamescriptsupport/events/scripteventgeneric.cpp ingamescriptsupport/events/scripteventgeneric.h
    ingamescriptsupport/events/scripteventchangeunitai.cpp ingamescriptsupport/events/scripteventchangeunitai.h
    ingamescriptsupport/events/scripteventchangebuildingowner.cpp ingamescriptsupport/events/scripteventchangebuildingowner.h
    ingamescriptsupport/events/scripteventchangeunitowner.cpp ingamescriptsupport/events/scripteventchangeunitowner.h
    ingamescriptsupport/events/scripteventchangeplayerteam.cpp ingamescriptsupport/events/scripteventchangeplayerteam.h
    ingamescriptsupport/events/scripteventspawnbuilding.cpp ingamescriptsupport/events/scripteventspawnbuilding.h
    ingamescriptsupport/events/scripteventcentermap.cpp ingamescriptsupport/events/scripteventcentermap.h
    ingamescriptsupport/events/scripteventplaysound.cpp ingamescriptsupport/events/scripteventplaysound.h

    # wiki stuff
    wiki/terraininfo.cpp wiki/terraininfo.h
    wiki/unitinfo.cpp wiki/unitinfo.h
    wiki/fieldinfo.cpp wiki/fieldinfo.h
    wiki/wikidatabase.cpp wiki/wikidatabase.h
    wiki/wikipage.cpp wiki/wikipage.h
    wiki/defaultwikipage.cpp wiki/defaultwikipage.h
    wiki/wikiview.cpp wiki/wikiview.h
    wiki/damagetablepage.cpp wiki/damagetablepage.h
    wiki/actionwikipage.cpp wiki/actionwikipage.h

    # table view
    objects/tableView/basetableitem.h objects/tableView/basetableitem.cpp
    objects/tableView/stringtableitem.h objects/tableView/stringtableitem.cpp
    objects/tableView/xofytableitem.h objects/tableView/xofytableitem.cpp
    objects/tableView/locktableitem.h objects/tableView/locktableitem.cpp
    objects/tableView/complextableview.h objects/tableView/complextableview.cpp

    # spriting support
    spritingsupport/spritecreator.cpp spritingsupport/spritecreator.h

    # ui
    ui_reader/uifactory.cpp ui_reader/uifactory.h
    ui_reader/createdgui.cpp ui_reader/createdgui.h
)

##############################################################################
# OS Specific files #
######################################
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(${PROJECT_NAME}_OS_SRCS
        coreengine/windows/crashreporter_os.cpp
        coreengine/windows/Gamepad.cpp
        #coreengine/sounds/soundcaching.cpp
        coreengine/sounds/ondemandsound.cpp
    )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    set(${PROJECT_NAME}_OS_SRCS
        coreengine/android/crashreporter_os.cpp
        coreengine/android/Gamepad.cpp
        coreengine/sounds/ondemandsound.cpp
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(${PROJECT_NAME}_OS_SRCS
        coreengine/ios/crashreporter_os.cpp
        coreengine/ios/Gamepad.cpp
        coreengine/sounds/ondemandsound.cpp
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(${PROJECT_NAME}_OS_SRCS
        coreengine/linux/crashreporter_os.cpp
        coreengine/linux/Gamepad.cpp
        coreengine/sounds/ondemandsound.cpp
    )
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

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
    message("Deploying resources")
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
        Qt6::Multimedia
    )
endif()

if (GRAPHICSUPPORT)
    if (USEVULKAN)
    else()
        set(QT_LIBRARIES
            ${QT_LIBRARIES}
            Qt6::OpenGL
        )
    endif()
endif()

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

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION
    ${${PROJECT_NAME}_OXYGINE_SRCS}
    ${${PROJECT_NAME}_SRCS}
    ${${PROJECT_NAME}_Resource_SRCS}
    ${${PROJECT_NAME}_OS_SRCS}
    ${${PROJECT_NAME}_SmtpClient_SRCS}
    ${${PROJECT_NAME}_UPDATE_SRCS}
    ICON.rc
    )

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
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        )
endif()

qt_finalize_executable(${PROJECT_NAME})

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
    set(LibNameStart "../lib/libQt6")
    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
        ${QT_BIN_DIR}/../plugins/platforms/libqeglfs.so
        ${QT_BIN_DIR}/../plugins/platforms/libqoffscreen.so
        ${QT_BIN_DIR}/../plugins/platforms/libqwayland-xcomposite-egl.so
        ${QT_BIN_DIR}/../plugins/platforms/libqlinuxfb.so
        ${QT_BIN_DIR}/../plugins/platforms/libqvnc.so
        ${QT_BIN_DIR}/../plugins/platforms/libqwayland-xcomposite-glx.so
        ${QT_BIN_DIR}/../plugins/platforms/libqminimalegl.so
        ${QT_BIN_DIR}/../plugins/platforms/libqwayland-egl.so
        ${QT_BIN_DIR}/../plugins/platforms/libqxcb.so
        ${QT_BIN_DIR}/../plugins/platforms/libqminimal.so
        ${QT_BIN_DIR}/../plugins/platforms/libqwayland-generic.so
        ${QT_BIN_DIR}/../plugins/sqldrivers/qsqlite.so
    )
    set(${PROJECT_NAME}_INSTALL_QT_SQL_LIBS
        ${QT_BIN_DIR}/../plugins/sqldrivers/qsqlite.so
    )

    set(LibNameEnd ".so.6")
    set(${PROJECT_NAME}_INSTALL_OS_LIBS
        ${QT_BIN_DIR}/${LibNameStart}Qml${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Core${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Network${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Multimedia${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Gui${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Widgets${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Xml${LibNameEnd}
        ${QT_BIN_DIR}/${LibNameStart}Sql${LibNameEnd}
    )

    if (GRAPHICSUPPORT)
        if (USEVULKAN)
        else()
            set(${PROJECT_NAME}_INSTALL_OS_LIBS
                ${${PROJECT_NAME}_INSTALL_OS_LIBS}
                ${QT_BIN_DIR}/${LibNameStart}OpenGL${LibNameEnd}
            )
        endif()
    endif()
    set(LibNameEnd ".so")
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
else()
    message(FATAL_ERROR "Unsupported OS found")
endif()

message("using qt lib naming convention: ${LibNameStart}Libname${LibNameEnd}")

set(${PROJECT_NAME}_INSTALL_QT_LIBS
    ${QT_BIN_DIR}/${LibNameStart}Qml${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Core${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Network${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Multimedia${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Gui${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Widgets${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Xml${LibNameEnd}
    ${QT_BIN_DIR}/${LibNameStart}Sql${LibNameEnd}
)

if (GRAPHICSUPPORT)
    if (USEVULKAN)
    else()
        set(${PROJECT_NAME}_INSTALL_QT_LIBS
            ${${PROJECT_NAME}_INSTALL_QT_LIBS}
            ${QT_BIN_DIR}/${LibNameStart}OpenGL${LibNameEnd}
        )
    endif()
endif()

message("install lib dir: ${CMAKE_INSTALL_PREFIX}")
# install folders and executable

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(TARGETS ${PROJECT_NAME}
        RUNTIME         DESTINATION         "."
        BUNDLE          DESTINATION         "."
        LIBRARY         DESTINATION         ".")

    install(DIRECTORY templates DESTINATION "/")
    if (DEPLOY_RESOURCES_AS_FOLDER)
        message("Installing resources as folder")
        install(DIRECTORY system DESTINATION                "/")
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
    install(DIRECTORY ${QT_BIN_DIR}/../plugins/multimedia       DESTINATION     "/"             OPTIONAL)
    install(FILES xinput1_3.dll                                 DESTINATION     "/"             OPTIONAL)

elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME_LOWER}"
    )
    install(TARGETS ${PROJECT_NAME}
        RUNTIME         DESTINATION         "bin"
        BUNDLE          DESTINATION         "bin"
        LIBRARY         DESTINATION         "bin")
    # install libs
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS}            DESTINATION "lib/${PROJECT_NAME_LOWER}"                              OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_OS_LIBS}            DESTINATION "lib/${PROJECT_NAME_LOWER}"                              OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS}  DESTINATION "lib/${PROJECT_NAME_LOWER}"                              OPTIONAL)
    install(DIRECTORY templates                                 DESTINATION "share/${PROJECT_NAME_LOWER}"                            OPTIONAL)
    install(DIRECTORY ${QT_BIN_DIR}/../plugins/multimedia       DESTINATION "lib/${PROJECT_NAME_LOWER}"                              OPTIONAL)
    install(FILES "icon.png"                                    DESTINATION "share/pixmaps"       RENAME "${PROJECT_NAME_LOWER}.png" OPTIONAL)
    install(FILES "Commander_Wars.desktop"                      DESTINATION "share/applications"  RENAME "${PROJECT_NAME}.desktop"   OPTIONAL)
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

if (DOXYGEN_FOUND)
    doxygen_add_docs(doxyGen ${${PROJECT_NAME}_SRCS})
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
        COMMENT "Running windeployqt..."
    )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    message("Build will run macdeployqt ${MACDEPLOYQT_EXECUTABLE}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.."
            -always-overwrite
        COMMENT "Running macdeployqt..."
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${QT_BIN_DIR}/../plugins/multimedia/libdarwinmediaplugin.dylib "${CMAKE_INSTALL_PREFIX}/Commander_Wars.app/Contents/PlugIns/multimedia/libdarwinmediaplugin.dylib")
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
