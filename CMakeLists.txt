cmake_minimum_required (VERSION 3.19.0)
project (Commander_Wars LANGUAGES CXX)

# Information for building commander wars on other platforms
# 1. make sure the CMAKE_PREFIX_PATH targets to the correct qt installation
# 2. make sure the LIBRARY_PATH targets the library folder of qt e.g. LD_LIBRARY_PATH=$HOME/Qt/5.15.2/gcc_64/lib

# Special Information for Android
# ANDROID_NATIVE_API_LEVEL needs to be 26
# NDK Version needs to be: 21.3.6528147 -> This needs to be matched exactly

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set tool chain info
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

###################################################################################
# Find external libs
###################################################################################
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Xml REQUIRED)
find_package(Qt6 COMPONENTS Multimedia REQUIRED)
find_package(Qt6 COMPONENTS OpenGL REQUIRED)
if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
endif()

###################################################################################
# Set build dependend defines
###################################################################################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("Compiling as Debug")
    add_definitions(
        -DQT_DECLARATIVE_DEBUG
        -DQT_QML_DEBUG
        -DGAMEDEBUG
        #-DMEMORYTRACING # only enable it if you can deal with a drastical performance decrease
        -DOXYGINE_DEBUG_SAFECAST
        )

    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
        set(DEPLOY_RESOURCES_AS_FOLDER 0)
        add_definitions(
            -DEnableMultimedia
        )
    else()
        set(DEPLOY_RESOURCES_AS_FOLDER 0)
    endif()
else("Release")
    message("Compiling as Release")
    add_definitions(
        -DGAMERELEASE
        )
    set(DEPLOY_RESOURCES_AS_FOLDER 0)
endif()

###################################################################################
# General defines
###################################################################################
add_definitions(
    -DMAJOR=0
    -DMINOR=20
    -DREVISION=0
    )

###################################################################################
# Set up some compiler and linking options
###################################################################################

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-Wa,-mbig-obj ${CMAKE_C_FLAGS}")
    find_program(ADDR_2_LINE addr2line)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    set(ADDR_2_LINE "")
    if ("${ANDROID_ABI}" STREQUAL armeabi-v7a)
        add_link_options("-long-plt")
    endif()
else()  # this branch works at least for linux
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "${QT_BIN_DIR}/../")
    find_program(ADDR_2_LINE addr2line)
endif()


###################################################################################
# show configuration
###################################################################################

message("Prefix Path: ${CMAKE_PREFIX_PATH}")
message("Module Path: ${CMAKE_MODULE_PATH}")
message("Qt-Dir: ${Qt6_DIR}")
message("Addr2Line Path: ${ADDR_2_LINE}")
message("C-Compiler Path: ${CMAKE_C_COMPILER}")
message("C++-Compiler Path: ${CMAKE_CXX_COMPILER}")
message("OS: ${CMAKE_SYSTEM_NAME}")
message("CFlags: ${CMAKE_C_FLAGS}")
message("CXX Flags: ${CMAKE_CXX_FLAGS}")
message("CMake Systemname: ${CMAKE_SYSTEM_NAME}")

###################################################################################
# start oxygine stuff -> qoxygine
###################################################################################
set(${PROJECT_NAME}_OXYGINE_SRCS
    # top level
    3rd_party/oxygine-framework/oxygine/AnimationFrame.cpp
    3rd_party/oxygine-framework/oxygine/Clock.cpp
    3rd_party/oxygine-framework/oxygine/Draggable.cpp
    3rd_party/oxygine-framework/oxygine/EventDispatcher.cpp
    3rd_party/oxygine-framework/oxygine/Font.cpp
    3rd_party/oxygine-framework/oxygine/Image.cpp
    3rd_party/oxygine-framework/oxygine/Input.cpp
    3rd_party/oxygine-framework/oxygine/MaskedRenderer.cpp
    3rd_party/oxygine-framework/oxygine/Material.cpp
    3rd_party/oxygine-framework/oxygine/MaterialCache.cpp
    3rd_party/oxygine-framework/oxygine/PointerState.cpp
    3rd_party/oxygine-framework/oxygine/RenderDelegate.cpp
    3rd_party/oxygine-framework/oxygine/STDRenderer.cpp
    3rd_party/oxygine-framework/oxygine/VisualStyle.cpp
    3rd_party/oxygine-framework/oxygine/RenderState.h
    3rd_party/oxygine-framework/oxygine/oxygine-forwards.h
    #core
    3rd_party/oxygine-framework/oxygine/core/ImageData.cpp
    3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.cpp
    3rd_party/oxygine-framework/oxygine/core/Object.cpp
    3rd_party/oxygine-framework/oxygine/core/oxygine.cpp
    3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.cpp
    3rd_party/oxygine-framework/oxygine/core/VideoDriver.cpp
    3rd_party/oxygine-framework/oxygine/core/gamewindow.cpp
    3rd_party/oxygine-framework/oxygine/core/ref_counter.cpp
    3rd_party/oxygine-framework/oxygine/core/texture.cpp
    3rd_party/oxygine-framework/oxygine/core/ShaderProgram.cpp
    3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h
    # actor
    3rd_party/oxygine-framework/oxygine/actor/Actor.cpp
    3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/Button.cpp
    3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.cpp
    3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/MaskedSprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/SlidingActor.cpp
    3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.cpp
    3rd_party/oxygine-framework/oxygine/actor/Sprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/Stage.cpp
    3rd_party/oxygine-framework/oxygine/actor/TextField.cpp
    3rd_party/oxygine-framework/oxygine/actor/slidingsprite.cpp
    # tween
    3rd_party/oxygine-framework/oxygine/tween/Tween.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenAnim.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenQueue.cpp
    3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.cpp
    3rd_party/oxygine-framework/oxygine/tween/tweenwait.cpp
    3rd_party/oxygine-framework/oxygine/tween/tweenscreenshake.cpp
    # res stuff
    3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.cpp
    3rd_party/oxygine-framework/oxygine/res/ResAnim.cpp
    3rd_party/oxygine-framework/oxygine/res/ResAtlas.cpp
    3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.cpp
    3rd_party/oxygine-framework/oxygine/res/ResBuffer.cpp
    3rd_party/oxygine-framework/oxygine/res/ResFont.cpp
    3rd_party/oxygine-framework/oxygine/res/ResFontBM.cpp
    3rd_party/oxygine-framework/oxygine/res/Resource.cpp
    3rd_party/oxygine-framework/oxygine/res/Resources.cpp
    3rd_party/oxygine-framework/oxygine/res/SingleResAnim.cpp
    # text utils
    3rd_party/oxygine-framework/oxygine/text_utils/Aligner.cpp
    3rd_party/oxygine-framework/oxygine/text_utils/Node.cpp
    3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.cpp
    # text utils
    3rd_party/oxygine-framework/oxygine/utils/AtlasBuilder.cpp
    # closure
    3rd_party/oxygine-framework/oxygine/core/closure.h
    # math
    3rd_party/oxygine-framework/oxygine/math/AffineTransform.h
    3rd_party/oxygine-framework/oxygine/math/Rect.h
    3rd_party/oxygine-framework/oxygine/math/ScalarMath.h
    3rd_party/oxygine-framework/oxygine/math/Vector2.h
    )
###################################################################################
# end oxygine stuff -> qoxygine
###################################################################################

###################################################################################
# source list of commander wars specific files
###################################################################################
set(${PROJECT_NAME}_SRCS
    main.cpp

    #objects --> i don't have a good place to put them right now
    objects/coinfoactor.cpp
    objects/rotatingsprite.cpp
    objects/ruleselection.cpp
    objects/minimap.cpp
    objects/perkselection.cpp
    objects/mapselectionview.cpp
    objects/loadingscreen.cpp
    objects/gameplayandkeys.cpp
    objects/editorselection.cpp
    objects/mapselection.cpp
    objects/coselection.cpp
    objects/playerselection.cpp
    objects/achievementbanner.cpp
    objects/unitstatisticview.cpp
    # objects base
    objects/base/topbar.cpp
    objects/base/textbox.cpp
    objects/base/h_scrollbar.cpp
    objects/base/v_scrollbar.cpp
    objects/base/dropdownmenu.cpp
    objects/base/dropdownmenucolor.cpp
    objects/base/panel.cpp
    objects/base/spinbox.cpp
    objects/base/checkbox.cpp
    objects/base/slider.cpp
    objects/base/multislider.cpp
    objects/base/passwordbox.cpp
    objects/base/progressinfobar.cpp
    objects/base/selectkey.cpp
    objects/base/chat.cpp
    objects/base/timespinbox.cpp
    objects/base/dropdownmenubase.cpp
    objects/base/label.cpp
    objects/base/colorselector.cpp
    objects/base/dropdownmenusprite.cpp
    objects/base/tooltip.cpp
    objects/base/focusableobject.cpp
    objects/base/tableview.cpp
    objects/base/closeablepopup.cpp
    objects/base/moveinbutton.cpp
    # objects dialogs
    objects/dialogs/colorselectiondialog.cpp
    objects/dialogs/dialogconnecting.cpp
    objects/dialogs/filedialog.cpp
    objects/dialogs/dialogcostyle.cpp
    objects/dialogs/dialogmessagebox.cpp
    objects/dialogs/dialogtextinput.cpp
    objects/dialogs/folderdialog.cpp
    objects/dialogs/dialogvaluecounter.cpp
    # objects dialogs editor
    objects/dialogs/editor/dialogmodifyunit.cpp
    objects/dialogs/editor/dialogmodifybuilding.cpp
    objects/dialogs/editor/dialogmodifyterrain.cpp
    objects/dialogs/editor/dialograndommap.cpp
    objects/dialogs/editor/mapeditdialog.cpp
    # objects dialogs rules
    objects/dialogs/rules/actionlistdialog.cpp
    objects/dialogs/rules/buildlistdialog.cpp
    objects/dialogs/rules/cobannlistdialog.cpp
    objects/dialogs/rules/coselectiondialog.cpp
    objects/dialogs/rules/perkselectiondialog.cpp
    objects/dialogs/rules/playerselectiondialog.cpp
    objects/dialogs/rules/ruleselectiondialog.cpp
    # objects dialogs ingame
    objects/dialogs/ingame/coinfodialog.cpp
    objects/dialogs/ingame/dialogvictoryconditions.cpp
    objects/dialogs/ingame/dialogattacklog.cpp
    objects/dialogs/ingame/dialogunitinfo.cpp
    objects/dialogs/ingame/victoryrulepopup.cpp

    #modding
    modding/csvtableimporter.cpp

    #menues
    menue/basemenu.cpp
    menue/mainwindow.cpp
    menue/ingamemenue.cpp
    menue/editormenue.cpp
    menue/gamemenue.cpp
    menue/optionmenue.cpp
    menue/mapselectionmapsmenue.cpp
    menue/creditsmenue.cpp
    menue/victorymenue.cpp
    menue/campaignmenu.cpp
    menue/wikimenu.cpp
    menue/costylemenu.cpp
    menue/replaymenu.cpp
    menue/achievementmenu.cpp
    menue/shopmenu.cpp

    # ressource management
    resource_management/unitspritemanager.cpp
    resource_management/terrainmanager.cpp
    resource_management/fontmanager.cpp
    resource_management/backgroundmanager.cpp
    resource_management/objectmanager.cpp
    resource_management/buildingspritemanager.cpp
    resource_management/movementtablemanager.cpp
    resource_management/gamemanager.cpp
    resource_management/gameanimationmanager.cpp
    resource_management/weaponmanager.cpp
    resource_management/cospritemanager.cpp
    resource_management/gamerulemanager.cpp
    resource_management/battleanimationmanager.cpp
    resource_management/coperkmanager.cpp
    resource_management/achievementmanager.cpp
    resource_management/shoploader.cpp

    # core engine
    coreengine/mainapp.cpp
    coreengine/settings.cpp
    coreengine/interpreter.cpp
    coreengine/console.cpp
    coreengine/audiothread.cpp
    coreengine/pathfindingsystem.cpp
    coreengine/qmlvector.cpp
    coreengine/scriptvariables.cpp
    coreengine/scriptvariable.cpp
    coreengine/workerthread.cpp
    coreengine/timer.cpp
    coreengine/userdata.cpp
    coreengine/crashreporter.cpp
    coreengine/filesupport.cpp
    coreengine/globalutils.cpp
    coreengine/scriptvariablefile.cpp

    # network engine
    network/tcpclient.cpp
    network/tcpserver.cpp
    network/txtask.cpp
    network/rxtask.cpp
    network/NetworkInterface.h
    network/networkgamedata.cpp
    network/mainserver.cpp
    network/localserver.cpp
    network/networkgame.cpp
    network/localclient.cpp

    # game
    game/gamemap.cpp
    game/terrain.cpp
    game/building.cpp
    game/co.cpp
    game/player.cpp
    game/unit.cpp
    game/terrainfindingsystem.cpp
    game/gameaction.cpp
    game/unitpathfindingsystem.cpp
    game/GameEnums.cpp
    game/gamerules.cpp
    game/gamerule.cpp
    game/victoryrule.cpp
    game/weather.cpp
    game/cursor.cpp
    game/createoutline.cpp
    # cool ingame recording
    game/gamerecording/daytodayrecord.cpp
    game/gamerecording/playerrecord.cpp
    game/gamerecording/specialevent.cpp
    game/gamerecording/gamerecorder.cpp
    game/gamerecording/replayrecorder.cpp
    # cool ingame script support
    game/gamescript.cpp
    game/campaign.cpp
    # animation stuff
    game/gameanimation/gameanimation.cpp
    game/gameanimation/gameanimationfactory.cpp
    game/gameanimation/gameanimationwalk.cpp
    game/gameanimation/gameanimationcapture.cpp
    game/gameanimation/gameanimationdialog.cpp
    game/gameanimation/gameanimationpower.cpp
    game/gameanimation/gameanimationnextday.cpp
    game/gameanimation/battleanimation.cpp
    game/gameanimation/battleanimationsprite.cpp
    # replay/observer
    game/viewplayer.cpp

    #ui
    game/ui/playerinfo.cpp
    game/ui/ingameinfobar.cpp
    game/ui/copowermeter.cpp

    # game input
    gameinput/basegameinputif.cpp
    gameinput/humanplayerinput.cpp
    gameinput/humanplayerinputmenu.cpp
    gameinput/menudata.cpp
    gameinput/markedfielddata.cpp
    gameinput/cursordata.cpp
    gameinput/mapmover.cpp

    # map importing/exporting support
    # and resizing etc.
    mapsupport/importcowtxt.cpp
    mapsupport/refactorMap.cpp
    mapsupport/randomMap.cpp
    mapsupport/importexport_awds.cpp
    mapsupport/importexport_awdc.cpp
    mapsupport/importawbyweb_text.cpp

    # ai
    ai/leaf.cpp
    ai/decisionnode.cpp
    ai/question.cpp
    ai/decisionquestion.cpp
    ai/decisiontree.cpp
    ai/coreai.cpp
    ai/veryeasyai.cpp
    ai/targetedunitpathfindingsystem.cpp
    ai/islandmap.cpp
    ai/coreai_predefinedai.cpp
    ai/proxyai.cpp
    ai/normalai.cpp
    ai/heavyai.cpp
    ai/heavyai/heavyaiproduction.cpp
    ai/influencefrontmap.cpp

    ai/neuralnetwork/neural/edge.cpp
    ai/neuralnetwork/neural/layer.cpp
    ai/neuralnetwork/neural/neuralnetwork.cpp
    ai/neuralnetwork/neural/neuron.cpp

    # multiplayer
    multiplayer/lobbymenu.cpp
    multiplayer/multiplayermenu.cpp
    multiplayer/gamedata.cpp
    multiplayer/password.cpp
    multiplayer/dialogpasswordandadress.cpp
    multiplayer/dialogpassword.cpp

    #campaign support
    ingamescriptsupport/campaigneditor.cpp

    # script support
    ingamescriptsupport/scripteditor.cpp
    ingamescriptsupport/genericbox.cpp
    ingamescriptsupport/scriptdialogdialog.cpp
    ingamescriptsupport/scriptdata.cpp
    # condition stuff
    ingamescriptsupport/conditions/scriptcondition.cpp
    ingamescriptsupport/conditions/scriptconditionvictory.cpp
    ingamescriptsupport/conditions/scriptconditionstartofturn.cpp
    ingamescriptsupport/conditions/scriptconditioneachday.cpp
    ingamescriptsupport/conditions/scriptconditionunitdestroyed.cpp
    ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.cpp
    ingamescriptsupport/conditions/scriptconditionterraindestroyed.cpp
    ingamescriptsupport/conditions/scriptconditionbuildingcaptured.cpp
    ingamescriptsupport/conditions/scriptconditionplayerdefeated.cpp
    ingamescriptsupport/conditions/scriptconditionunitsdestroyed.cpp
    ingamescriptsupport/conditions/scriptconditionbuildingsowned.cpp
    ingamescriptsupport/conditions/scriptconditionplayerreachedarea.cpp
    ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.cpp
    ingamescriptsupport/conditions/scriptconditioncheckvariable.cpp
    # event stuff
    ingamescriptsupport/events/scriptevent.cpp
    ingamescriptsupport/events/scripteventdialog.cpp
    ingamescriptsupport/events/scripteventspawnunit.cpp
    ingamescriptsupport/events/scripteventdefeatplayer.cpp
    ingamescriptsupport/events/scripteventchangebuildlist.cpp
    ingamescriptsupport/events/scripteventaddfunds.cpp
    ingamescriptsupport/events/scripteventchangeweather.cpp
    ingamescriptsupport/events/scripteventchangecobar.cpp
    ingamescriptsupport/events/scripteventmodifyunit.cpp
    ingamescriptsupport/events/scripteventmodifyterrain.cpp
    ingamescriptsupport/events/scripteventanimation.cpp
    ingamescriptsupport/events/scripteventvictoryinfo.cpp
    ingamescriptsupport/events/scripteventmodifyvariable.cpp
    ingamescriptsupport/events/scripteventgeneric.cpp
    ingamescriptsupport/events/scripteventchangeunitai.cpp
    ingamescriptsupport/events/scripteventchangebuildingowner.cpp
    ingamescriptsupport/events/scripteventchangeunitowner.cpp
    ingamescriptsupport/events/scripteventchangeplayerteam.cpp
    ingamescriptsupport/events/scripteventspawnbuilding.cpp
    ingamescriptsupport/events/scripteventcentermap.cpp
    ingamescriptsupport/events/scripteventplaysound.cpp

    # wiki stuff
    wiki/terraininfo.cpp
    wiki/unitinfo.cpp
    wiki/fieldinfo.cpp
    wiki/wikidatabase.cpp
    wiki/wikipage.cpp
    wiki/defaultwikipage.cpp
    wiki/wikiview.cpp

    # spriting support
    spritingsupport/spritecreator.cpp

    # ui
    ui_reader/uifactory.cpp
    )

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
        "general.qrc"
    )
else()
    qt_add_big_resources(${PROJECT_NAME}_Resource_SRCS
                         "general.qrc"
                         "music.qrc"
                         "sound.qrc"
    )
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
    QT_ANDROID_VERSION_NAME "0.20.0"
    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_ANDROID_VERSION_CODE "1"
    )


# link libraries
set(QT_LIBRARIES
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Network
    Qt6::Widgets
    Qt6::Xml
    Qt6::Multimedia
    Qt6::OpenGL
    )

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        # special linking of libs for windows
        Dbghelp
        )
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${QT_LIBRARIES}
        )
else() # this branch works at least for linux
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
    # set(LUPDATE_OPTIONS "-I ${CMAKE_CURRENT_SOURCE_DIR}")
    message ("Language Update Options: ${LUPDATE_OPTIONS}")
    # created translation files
    set(TS_FILES
        translation/lang_de_DE.ts
        translation/lang_en.ts
        )
    # adds the translations and a target for it
    qt_create_translation(
        QM_FILES
        ${${PROJECT_NAME}_SRCS}
        ${scripts}
        ${modScripts}
        ${GameScripts}
        ${CampaignScripts}
        ${UiXmls}
        ${TS_FILES}
        OPTIONS ${LUPDATE_OPTIONS})
    qt_add_translation(${PROJECT_NAME}_QM_FILES ${TS_FILES})
    add_custom_target(${PROJECT_NAME}_translations DEPENDS ${QM_FILES})
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_translations)
endif()

##############################################################
# end of this is just for the ide view nothing for execution #
##############################################################

##############################################################
# Qt RCC Resource section                                    #
##############################################################
if (NOT DEPLOY_RESOURCES_AS_FOLDER)
    message("Bundling resources as rcc-pack")
    # map anme to correct alias
    foreach(QM_FILE ${QM_FILES})
        get_filename_component(barename ${QM_FILE} NAME)
        set_source_files_properties(${QM_FILE}
            PROPERTIES QT_RESOURCE_ALIAS "resources/translation/${barename}"
            )
    endforeach()
    qt_add_resources(${PROJECT_NAME} "${PROJECT_NAME}"
        PREFIX "/"
        FILES
        ${QM_FILES}
    )
endif()

##############################################################
# Install section                                            #
##############################################################
# get qt dir
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${_qmake_executable}" DIRECTORY)
message(${QT_BIN_DIR})

# those libs are not the same for each platform and need to be selected based on the os
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${QT_BIN_DIR}/Qt6Qml.dll
        ${QT_BIN_DIR}/Qt6Core.dll
        ${QT_BIN_DIR}/Qt6Network.dll
        ${QT_BIN_DIR}/Qt6Multimedia.dll
        ${QT_BIN_DIR}/Qt6Gui.dll
        ${QT_BIN_DIR}/Qt6Widgets.dll
        ${QT_BIN_DIR}/Qt6Xml.dll
        ${QT_BIN_DIR}/Qt6OpenGL.dll
        ${QT_BIN_DIR}/libstdc++-6.dll
        ${QT_BIN_DIR}/libwinpthread-1.dll
        ${QT_BIN_DIR}/libgcc_s_seh-1.dll
        ${ADDR_2_LINE}
        )

    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
        ${QT_BIN_DIR}/../plugins/platforms/qwindows.dll
        )
#    set(${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS
#        ${QT_BIN_DIR}/../plugins/styles/qwindowsvistastyle.dll
#        )
else() # currently this is for linux systems -> may differ for ios # todo maybe a proper check would be nice here

    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${QT_BIN_DIR}/../lib/libQt6Qml.so
        ${QT_BIN_DIR}/../lib/libQt6Core.so
        ${QT_BIN_DIR}/../lib/libQt6Network.so
        ${QT_BIN_DIR}/../lib/libQt6Multimedia.so
        ${QT_BIN_DIR}/../lib/libQt6Gui.so
        ${QT_BIN_DIR}/../lib/libQt6Widgets.so
        ${QT_BIN_DIR}/../lib/libQt6Xml.so
        ${QT_BIN_DIR}/../lib/libQt6OpenGL.so
        ${ADDR_2_LINE}
        )

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
        )
endif()

message("install lib dir: ${CMAKE_INSTALL_PREFIX}")

# install folders and executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME         DESTINATION         "${CMAKE_BUILD_TYPE}"
    BUNDLE          DESTINATION         "${CMAKE_BUILD_TYPE}"
    LIBRARY         DESTINATION         "${CMAKE_BUILD_TYPE}")

if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
    install(DIRECTORY templates DESTINATION                                         "${CMAKE_BUILD_TYPE}")
    if (DEPLOY_RESOURCES_AS_FOLDER)
        message("Installing resources as folder")
        install(DIRECTORY system DESTINATION                "${CMAKE_BUILD_TYPE}")
        install(DIRECTORY resources DESTINATION             "${CMAKE_BUILD_TYPE}")
        install(DIRECTORY maps DESTINATION                  "${CMAKE_BUILD_TYPE}")
        install(FILES ${QM_FILES} DESTINATION               "${CMAKE_BUILD_TYPE}/resources/translation/" OPTIONAL)
        install(DIRECTORY customTerrainImages DESTINATION   "${CMAKE_BUILD_TYPE}")
        install(DIRECTORY mods DESTINATION                  "${CMAKE_BUILD_TYPE}")
        install(DIRECTORY savegames DESTINATION             "${CMAKE_BUILD_TYPE}")
        install(DIRECTORY data DESTINATION                  "${CMAKE_BUILD_TYPE}")
    endif()
    # install libs
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS} DESTINATION                    "${CMAKE_BUILD_TYPE}" OPTIONAL)
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS} DESTINATION          "${CMAKE_BUILD_TYPE}/platforms" OPTIONAL)
endif()
