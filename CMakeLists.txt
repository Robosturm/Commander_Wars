cmake_minimum_required (VERSION 3.16.0)
project (Commander_Wars)

# Information for building commander wars on other platforms
# 1. make sure the CMAKE_PREFIX_PATH targets to the correct qt installation
# 2. make sure the LIBRARY_PATH targets the library folder of qt e.g. LD_LIBRARY_PATH=$HOME/Qt/5.15.0/gcc_64/lib

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set tool chain info
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# show configuration
message("Prefix Path: ${CMAKE_PREFIX_PATH}")
message("Module Path: ${CMAKE_MODULE_PATH}")
find_program(ADDR_2_LINE addr2line)
message("Addr2Line Path: ${ADDR_2_LINE}")
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
    -DMINOR=18
    -DREVISION=1
)

message ("CXX Flags: ${CMAKE_CXX_FLAGS}")
message ("CXX System: ${CMAKE_SYSTEM_NAME}")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}") # -g for debug info
    set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}") # -g for debug info
else()
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "${QT_BIN_DIR}/../")
endif()

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
    3rd_party/oxygine-framework/oxygine/PostProcess.cpp
    3rd_party/oxygine-framework/oxygine/RenderDelegate.cpp
    3rd_party/oxygine-framework/oxygine/STDRenderDelegate.cpp
    3rd_party/oxygine-framework/oxygine/STDRenderer.cpp
    3rd_party/oxygine-framework/oxygine/VisualStyle.cpp
    #core
    3rd_party/oxygine-framework/oxygine/core/ImageData.cpp
    3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.cpp
    3rd_party/oxygine-framework/oxygine/core/NativeTexture.cpp
    3rd_party/oxygine-framework/oxygine/core/Object.cpp
    3rd_party/oxygine-framework/oxygine/core/oxygine.cpp
    3rd_party/oxygine-framework/oxygine/core/Renderer.cpp
    3rd_party/oxygine-framework/oxygine/core/Restorable.cpp
    3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.cpp
    3rd_party/oxygine-framework/oxygine/core/VideoDriver.cpp
    3rd_party/oxygine-framework/oxygine/core/gamewindow.cpp
    # core gl
    3rd_party/oxygine-framework/oxygine/core/gl/NativeTextureGLES.cpp
    3rd_party/oxygine-framework/oxygine/core/gl/ShaderProgramGL.cpp
    3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.cpp
    3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGL.cpp
    3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.cpp
    # actor
    3rd_party/oxygine-framework/oxygine/actor/Actor.cpp
    3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/Button.cpp
    3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.cpp
    3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/MaskedSprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/Polygon.cpp
    3rd_party/oxygine-framework/oxygine/actor/ProgressBar.cpp
    3rd_party/oxygine-framework/oxygine/actor/SlidingActor.cpp
    3rd_party/oxygine-framework/oxygine/actor/Sprite.cpp
    3rd_party/oxygine-framework/oxygine/actor/Stage.cpp
    3rd_party/oxygine-framework/oxygine/actor/TextField.cpp
    3rd_party/oxygine-framework/oxygine/actor/slidingsprite.cpp
    # tween
    3rd_party/oxygine-framework/oxygine/tween/Tween.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenAlphaFade.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenAnim.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenGlow.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenOutline.cpp
    3rd_party/oxygine-framework/oxygine/tween/TweenQueue.cpp
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
        wiki/wikiview.cpp

        # spriting support
        spritingsupport/spritecreator.cpp
)

#######################################################
# this is just for the ide view nothing for execution #
#######################################################
FILE(GLOB_RECURSE scripts "resources/*.js")
FILE(GLOB_RECURSE images "resources/*.png")
FILE(GLOB_RECURSE xmls "resources/*.xml")
FILE(GLOB_RECURSE inis "resources/*.ini")
add_custom_target(Resources SOURCES ${scripts} ${images} ${xmls} ${inis})

FILE(GLOB_RECURSE modScripts "mods/*.js")
FILE(GLOB_RECURSE modDescs "mods/*.txt")
FILE(GLOB_RECURSE modImages "resources/*.png")
FILE(GLOB_RECURSE modXmls "resources/*.xml")
add_custom_target(modscripts SOURCES ${modScripts} ${modImages} ${modXmls} ${modDescs})

FILE(GLOB_RECURSE TemplateScripts "templates/*.js")
add_custom_target(templatescripts SOURCES ${TemplateScripts})

FILE(GLOB_RECURSE GameScripts "maps/*.js")
add_custom_target(gamescripts SOURCES ${GameScripts})

FILE(GLOB_RECURSE CampaignScripts "maps/*.jsm")
add_custom_target(campaignscripts SOURCES ${CampaignScripts})

FILE(GLOB_RECURSE ShaderScripts "system/*.glsl")
add_custom_target(shaders SOURCES ${ShaderScripts})

FILE(GLOB_RECURSE DataScripts "data/*.js")
add_custom_target(data SOURCES ${DataScripts})
##############################################################
# end of this is just for the ide view nothing for execution #
##############################################################

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

# those libs are not the same for each platform and need to be selected based on the os
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${QT_BIN_DIR}/Qt5Qml.dll
        ${QT_BIN_DIR}/Qt5Core.dll
        ${QT_BIN_DIR}/Qt5Network.dll
        ${QT_BIN_DIR}/Qt5Multimedia.dll
        ${QT_BIN_DIR}/Qt5Gui.dll
        ${QT_BIN_DIR}/Qt5Widgets.dll
        ${QT_BIN_DIR}/Qt5Svg.dll
        ${QT_BIN_DIR}/Qt5Xml.dll
        ${QT_BIN_DIR}/libgcc_s_dw2-1.dll
        ${QT_BIN_DIR}/libstdc++-6.dll
        ${QT_BIN_DIR}/libwinpthread-1.dll
        ${QT_BIN_DIR}/d3dcompiler_47.dll
        ${QT_BIN_DIR}/libEGL.dll
        ${QT_BIN_DIR}/libGLESv2.dll
        ${ADDR_2_LINE}
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

    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${QT_BIN_DIR}/../lib/libQt5Qml.so
        ${QT_BIN_DIR}/../lib/libQt5Core.so
        ${QT_BIN_DIR}/../lib/libQt5Network.so
        ${QT_BIN_DIR}/../lib/libQt5Multimedia.so
        ${QT_BIN_DIR}/../lib/libQt5Gui.so
        ${QT_BIN_DIR}/../lib/libQt5Widgets.so
        ${QT_BIN_DIR}/../lib/libQt5Svg.so
        ${QT_BIN_DIR}/../lib/libQt5Xml.so
        ${ADDR_2_LINE}
    )
    set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
         ${QT_BIN_DIR}/../plugins/mediaservice/libgstaudiodecoder.so
         ${QT_BIN_DIR}/../plugins/mediaservice/libgstcamerabin.so
         ${QT_BIN_DIR}/../plugins/mediaservice/libgstmediaplayer.so
         ${QT_BIN_DIR}/../plugins/mediaservice/libgstmediacapture.so
    )
    set(${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS
         ${QT_BIN_DIR}/../plugins/bearer/libqgenericbearer.so
    )
    set(${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS
         ${QT_BIN_DIR}/../plugins/playlistformats/libqtmultimedia_m3u.so
    )
    set(${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS
         ${QT_BIN_DIR}/../plugins/audio/libqtmedia_pulse.so
         ${QT_BIN_DIR}/../plugins/audio/libqtaudio_alsa.so
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
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(${PROJECT_NAME}_INSTALL_QT_QMLTOOLING_LIBS
            ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_server.dll
            ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_debugger.dll
            ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_inspector.dll
            ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_messages.dll
            ${QT_BIN_DIR}/../plugins/qmltooling/qmldbg_tcp.dll
        )
    else()
        set(${PROJECT_NAME}_INSTALL_QT_QMLTOOLING_LIBS
            ${QT_BIN_DIR}/../plugins/qmltooling/libqmldbg_server.so
            ${QT_BIN_DIR}/../plugins/qmltooling/libqmldbg_debugger.so
            ${QT_BIN_DIR}/../plugins/qmltooling/libqmldbg_inspector.so
            ${QT_BIN_DIR}/../plugins/qmltooling/libqmldbg_messages.so
            ${QT_BIN_DIR}/../plugins/qmltooling/libqmldbg_tcp.so
        )
    endif()
    install(FILES ${${PROJECT_NAME}_INSTALL_QT_QMLTOOLING_LIBS} DESTINATION "qmltooling")
endif()

message("install lib dir: ${CMAKE_SYSTEM_PREFIX_PATH}")
# install folders and executable
install(DIRECTORY system DESTINATION                ".")
install(DIRECTORY resources DESTINATION             ".")
install(DIRECTORY maps DESTINATION                  ".")
install(FILES ${QM_FILES} DESTINATION "resources/translation/" OPTIONAL)
install(DIRECTORY customTerrainImages DESTINATION   ".")
install(DIRECTORY mods DESTINATION                  ".")
install(DIRECTORY templates DESTINATION             ".")
install(DIRECTORY savegames DESTINATION             ".")
install(DIRECTORY data DESTINATION                  ".")
install(TARGETS ${PROJECT_NAME} DESTINATION         "."
                RUNTIME         DESTINATION         "."
                ARCHIVE         DESTINATION         "."
                LIBRARY         DESTINATION         ".")
# install libs
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION                       "." OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS} DESTINATION                    "." OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS} DESTINATION         "mediaservice" OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS} DESTINATION              "audio" OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS} DESTINATION             "bearer" OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS} DESTINATION          "platforms" OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS} DESTINATION    "playlistformats" OPTIONAL)
install(FILES ${${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS} DESTINATION             "styles" OPTIONAL)
