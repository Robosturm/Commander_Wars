cmake_minimum_required (VERSION 3.16.0)
project (Commander_Wars)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set tool chain info
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# add qt to our project and provide qt path to package path on travis
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QT5_BASE_DIR};$ENV{QT5_BASE_DIR}/lib/cmake")
message("Module Path: ${CMAKE_PREFIX_PATH}")
message("C-Compiler Path: ${CMAKE_C_COMPILER}")
message("C++-Compiler Path: ${CMAKE_CXX_COMPILER}")

find_package(Qt5 COMPONENTS Core REQUIRED)
# Qt5LinguistTools
find_package(Qt5 COMPONENTS LinguistTools REQUIRED)
find_package(Qt5 COMPONENTS Qml REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)
find_package(Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

# build dependend defines
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("Compiling as Debug")
    add_definitions(
        -DQT_DECLARATIVE_DEBUG
        -DQT_QML_DEBUG
        -DGAMEDEBUG
    )
else("Release")
    message("Compiling as Release")
    add_definitions(
        -DGAMERELEASE
    )
endif()

# general defines
add_definitions(
    -DUSE_MEMORY_POOL
    -DMAJOR=0
    -DMINOR=17
    -DREVISION=0
)

message ("CXX Flags: ${CMAKE_CXX_FLAGS}")
message ("CXX System: ${CMAKE_SYSTEM_NAME}")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}") # -g for debug info
endif()


###################################################################################
# start oxygine stuff -> qoxygine
###################################################################################
# setup include dirs
set(OXYGINE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/oxygine-framework)
include_directories(${OXYGINE_ROOT})

set(${PROJECT_NAME}_OXYGINE_SRCS
    # top level
    ${OXYGINE_ROOT}/oxygine/AnimationFrame.cpp
    ${OXYGINE_ROOT}/oxygine/Clock.cpp
    ${OXYGINE_ROOT}/oxygine/Draggable.cpp
    ${OXYGINE_ROOT}/oxygine/EventDispatcher.cpp
    ${OXYGINE_ROOT}/oxygine/Font.cpp
    ${OXYGINE_ROOT}/oxygine/Image.cpp
    ${OXYGINE_ROOT}/oxygine/Input.cpp
    ${OXYGINE_ROOT}/oxygine/MaskedRenderer.cpp
    ${OXYGINE_ROOT}/oxygine/Material.cpp
    ${OXYGINE_ROOT}/oxygine/MaterialCache.cpp
    ${OXYGINE_ROOT}/oxygine/PointerState.cpp
    ${OXYGINE_ROOT}/oxygine/PostProcess.cpp
    ${OXYGINE_ROOT}/oxygine/RenderDelegate.cpp
    ${OXYGINE_ROOT}/oxygine/STDRenderDelegate.cpp
    ${OXYGINE_ROOT}/oxygine/STDRenderer.cpp
    ${OXYGINE_ROOT}/oxygine/VisualStyle.cpp
    #core
    ${OXYGINE_ROOT}/oxygine/core/ImageData.cpp
    ${OXYGINE_ROOT}/oxygine/core/ImageDataOperations.cpp
    ${OXYGINE_ROOT}/oxygine/core/NativeTexture.cpp
    ${OXYGINE_ROOT}/oxygine/core/Object.cpp
    ${OXYGINE_ROOT}/oxygine/core/oxygine.cpp
    ${OXYGINE_ROOT}/oxygine/core/Renderer.cpp
    ${OXYGINE_ROOT}/oxygine/core/Restorable.cpp
    ${OXYGINE_ROOT}/oxygine/core/UberShaderProgram.cpp
    ${OXYGINE_ROOT}/oxygine/core/VideoDriver.cpp
	${OXYGINE_ROOT}/oxygine/core/gamewindow.cpp
    # core gl
    ${OXYGINE_ROOT}/oxygine/core/gl/NativeTextureGLES.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/ShaderProgramGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VertexDeclarationGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VideoDriverGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VideoDriverGLES20.cpp
    # actor
    ${OXYGINE_ROOT}/oxygine/actor/Actor.cpp
    ${OXYGINE_ROOT}/oxygine/actor/Box9Sprite.cpp
    ${OXYGINE_ROOT}/oxygine/actor/Button.cpp
    ${OXYGINE_ROOT}/oxygine/actor/ClipRectActor.cpp
    ${OXYGINE_ROOT}/oxygine/actor/ColorRectSprite.cpp
    ${OXYGINE_ROOT}/oxygine/actor/MaskedSprite.cpp
    ${OXYGINE_ROOT}/oxygine/actor/Polygon.cpp
    ${OXYGINE_ROOT}/oxygine/actor/ProgressBar.cpp
    ${OXYGINE_ROOT}/oxygine/actor/SlidingActor.cpp
    ${OXYGINE_ROOT}/oxygine/actor/Sprite.cpp
    ${OXYGINE_ROOT}/oxygine/actor/Stage.cpp
    ${OXYGINE_ROOT}/oxygine/actor/TextField.cpp
    ${OXYGINE_ROOT}/oxygine/actor/slidingsprite.cpp
    # tween
    ${OXYGINE_ROOT}/oxygine/tween/Tween.cpp
    ${OXYGINE_ROOT}/oxygine/tween/TweenAlphaFade.cpp
    ${OXYGINE_ROOT}/oxygine/tween/TweenAnim.cpp
    ${OXYGINE_ROOT}/oxygine/tween/TweenGlow.cpp
    ${OXYGINE_ROOT}/oxygine/tween/TweenOutline.cpp
    ${OXYGINE_ROOT}/oxygine/tween/TweenQueue.cpp
    # res stuff
    ${OXYGINE_ROOT}/oxygine/res/CreateResourceContext.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResAnim.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResAtlas.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResAtlasGeneric.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResBuffer.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResFont.cpp
    ${OXYGINE_ROOT}/oxygine/res/ResFontBM.cpp
    ${OXYGINE_ROOT}/oxygine/res/Resource.cpp
    ${OXYGINE_ROOT}/oxygine/res/Resources.cpp
    ${OXYGINE_ROOT}/oxygine/res/SingleResAnim.cpp
    # text utils
    ${OXYGINE_ROOT}/oxygine/text_utils/Aligner.cpp
    ${OXYGINE_ROOT}/oxygine/text_utils/Node.cpp
    ${OXYGINE_ROOT}/oxygine/text_utils/TextBuilder.cpp
    # text utils
    ${OXYGINE_ROOT}/oxygine/utils/AtlasBuilder.cpp
)
###################################################################################
# end oxygine stuff -> qoxygine
###################################################################################

# source list of commander wars specific files
set(${PROJECT_NAME}_SRCS
        main.cpp

        #objects
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
        # objects dialogs
        objects/dialogs/colorselectiondialog.cpp
        objects/dialogs/dialogconnecting.cpp
        objects/dialogs/filedialog.cpp
        objects/dialogs/dialogcostyle.cpp
        objects/dialogs/dialogmessagebox.cpp
        objects/dialogs/dialogtextinput.cpp
        objects/dialogs/folderdialog.cpp
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
        # tweens
        coreengine/tweentogglevisibility.cpp
        coreengine/tweenwait.cpp
        coreengine/tweenaddcolorall.cpp

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
        game/battleanimationsprite.cpp
        # animation stuff
        game/gameanimation.cpp
        game/gameanimationfactory.cpp
        game/gameanimationwalk.cpp
        game/gameanimationcapture.cpp
        game/gameanimationdialog.cpp
        game/gameanimationpower.cpp
        game/gameanimationnextday.cpp
        game/battleanimation.cpp
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
        ai/influencefrontmap.cpp

        ai/neuralnetwork/neural/edge.cpp
        ai/neuralnetwork/neural/layer.cpp
        ai/neuralnetwork/neural/neuralnetwork.cpp
        ai/neuralnetwork/neural/neuron.cpp
        ai/neuralnetwork/dataset/dataset.cpp
        ai/neuralnetwork/optimizer/optimizer.cpp

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

        # spriting support
        spritingsupport/spritecreator.cpp
)


FILE(GLOB_RECURSE JavaScripts "resources/*.js")
add_custom_target(scripts SOURCES ${JavaScripts})

FILE(GLOB_RECURSE ModScripts "mods/*.js")
add_custom_target(modscripts SOURCES ${ModScripts})

FILE(GLOB_RECURSE TemplateScripts "templates/*.js")
add_custom_target(templatescripts SOURCES ${TemplateScripts})

FILE(GLOB_RECURSE GameScripts "maps/*.js")
add_custom_target(gamescripts SOURCES ${GameScripts})

FILE(GLOB_RECURSE CampaignScripts "maps/*.jsm")
add_custom_target(campaignscripts SOURCES ${CampaignScripts})

FILE(GLOB_RECURSE ShaderScripts "system/*.glsl")
add_custom_target(shaders SOURCES ${ShaderScripts})
# create translation here :)
set(LUPDATE_OPTIONS "-I ${CMAKE_CURRENT_SOURCE_DIR}")
message ("Language Update Options: ${LUPDATE_OPTIONS}")

# created translation files
set(TS_FILES
    translation/lang_de_DE.ts
    translation/lang_en.ts
)
# adds the translations and a target for it
qt5_create_translation(
        QM_FILES
        ${${PROJECT_NAME}_SRCS}
        ${JavaScripts}
        ${TS_FILES}
        OPTIONS ${LUPDATE_OPTIONS}
)

add_custom_target(${PROJECT_NAME}_translations DEPENDS ${QM_FILES})

# create executable
add_executable(
        ${PROJECT_NAME}
        ${${PROJECT_NAME}_OXYGINE_SRCS}
        ${${PROJECT_NAME}_SRCS}
        ICON.rc
)

if (WIN32) #disable console mode for VC++
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)


# link libraries

set(QT_LIBRARIES
    Qt5::Core
    Qt5::Qml
    Qt5::Network
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Multimedia
    Qt5::OpenGL
)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
        ${OPENGL_LIBRARIES}
        Dbghelp
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
        ${OPENGL_LIBRARIES}
        # currently no crash reporting supported on other os systems
    )
endif()


# install section
set(CMAKE_INSTALL_PREFIX "../install/${CMAKE_BUILD_TYPE}")

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${_qmake_executable}" DIRECTORY)


message(${QT_BIN_DIR})

set(${PROJECT_NAME}_INSTALL_QT_LIBS
    ${QT_BIN_DIR}/Qt5Qml.dll
    ${QT_BIN_DIR}/Qt5Core.dll
    ${QT_BIN_DIR}/Qt5Network.dll
    ${QT_BIN_DIR}/Qt5Multimedia.dll
    ${QT_BIN_DIR}/Qt5Gui.dll
    ${QT_BIN_DIR}/Qt5Widgets.dll
    ${QT_BIN_DIR}/libgcc_s_dw2-1.dll
    ${QT_BIN_DIR}/libstdc++-6.dll
    ${QT_BIN_DIR}/libwinpthread-1.dll
    ${QT_BIN_DIR}/d3dcompiler_47.dll
    ${QT_BIN_DIR}/libEGL.dll
    ${QT_BIN_DIR}/libGLESv2.dll
    ${QT_BIN_DIR}/Qt5Svg.dll
    ${QT_BIN_DIR}/Qt5Xml.dll
    ${QT_BIN_DIR}/../../../tools/mingw810_32/bin/addr2line.exe
    ${QT_BIN_DIR}/libGLESv2.dll
    ${QT_BIN_DIR}/libEGL.dll
)

set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
     ${QT_BIN_DIR}/../plugins/mediaservice/dsengine.dll
     ${QT_BIN_DIR}/../plugins/mediaservice/qtmedia_audioengine.dll
)

set(${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS
     ${QT_BIN_DIR}/../plugins/bearer/qgenericbearer.dll
)

set(${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS
     ${QT_BIN_DIR}/../plugins/playlistformats/qtmultimedia_m3u.dll
)

# those libs are not the same for each platform and need to be selected based on the os
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS
         ${QT_BIN_DIR}/../plugins/audio/qtaudio_windows.dll
    )
    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
         ${QT_BIN_DIR}/../plugins/platforms/qwindows.dll
    )
    set(${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS
         ${QT_BIN_DIR}/../plugins/styles/qwindowsvistastyle.dll
    )
else() # currently this is for linux systems -> may differ for ios # todo maybe a proper check would be nice here
    set(${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS
         ${QT_BIN_DIR}/../plugins/audio/libqtmedia_pulse.so
         ${QT_BIN_DIR}/../plugins/audio/libqtaudio_alsa.so
    )
    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
        # todo install the proper platforms library for linux here
        # ${QT_BIN_DIR}/../plugins/platforms/
    )
    set(${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS
        # todo install the proper  linux style lib to be used
        # ${QT_BIN_DIR}/../plugins/styles/
    )
endif()
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(${PROJECT_NAME}_INSTALL_QT_QMLTOOLING_LIBS
        ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_server.dll
        ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_debugger.dll
        ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_inspector.dll
        ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_messages.dll
        ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_tcp.dll
    )
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_QMLTOOLING_LIBS} DESTINATION "qmltooling")
endif()


# install  folders
install(DIRECTORY system DESTINATION ".")
install(DIRECTORY resources DESTINATION ".")
install(DIRECTORY maps DESTINATION ".")
install(DIRECTORY customTerrainImages DESTINATION ".")
install(DIRECTORY mods DESTINATION ".")
install(DIRECTORY templates DESTINATION ".")
install(DIRECTORY savegames DESTINATION ".")
install(DIRECTORY data DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION ".")
# install libs and exe
install(TARGETS ${PROJECT_NAME} DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS} DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS} DESTINATION "mediaservice")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS} DESTINATION "audio")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS} DESTINATION "bearer")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS} DESTINATION "platforms")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS} DESTINATION "playlistformats")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS} DESTINATION "styles")

# install translations
install(FILES ${QM_FILES} DESTINATION "resources/translation/")
