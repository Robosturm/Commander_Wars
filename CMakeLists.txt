cmake_minimum_required (VERSION 3.1.0)
project (Commander_Wars)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# add qt to our project and provide qt path to package path on travis
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QT5_BASE_DIR};$ENV{QT5_BASE_DIR}/lib/cmake")
message("Module Path: ${CMAKE_PREFIX_PATH}")
message("C-Compiler Path: ${CMAKE_C_COMPILER}")
message("C++-Compiler Path: ${CMAKE_CXX_COMPILER}")


find_package(Qt5Core REQUIRED)
# Qt5LinguistTools
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(OpenGL REQUIRED)

# build dependend defines
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

# general defines
add_definitions(
    -DUSE_MEMORY_POOL
    -DMAJOR=0
    -DMINOR=8
    -DREVISION=0
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

###################################################################################
# start oxygine stuff -> qoxygine
###################################################################################
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
    ${OXYGINE_ROOT}/oxygine/core/Texture.cpp
    ${OXYGINE_ROOT}/oxygine/core/UberShaderProgram.cpp
    ${OXYGINE_ROOT}/oxygine/core/VideoDriver.cpp
	${OXYGINE_ROOT}/oxygine/core/gamewindow.cpp
    # core gl
    ${OXYGINE_ROOT}/oxygine/core/gl/NativeTextureGLES.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/ShaderProgramGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VertexDeclarationGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VideoDriverGL.cpp
    ${OXYGINE_ROOT}/oxygine/core/gl/VideoDriverGLES20.cpp
    # math
    ${OXYGINE_ROOT}/oxygine/math/AffineTransform.cpp
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

# source list we start slow here with the main cpp
set(${PROJECT_NAME}_SRCS
        main.cpp

        # objects
        objects/cursor.cpp
        objects/EditorSelection.cpp
        objects/topbar.cpp
        objects/textbox.cpp
        objects/h_scrollbar.cpp
        objects/v_scrollbar.cpp
        objects/dropdownmenu.cpp
        objects/dropdownmenucolor.cpp
        objects/panel.cpp
        objects/filedialog.cpp
        objects/mapeditdialog.cpp
        objects/spinbox.cpp
        objects/minimap.cpp
        objects/mapselection.cpp
        objects/coselection.cpp
        objects/coselectiondialog.cpp
        objects/checkbox.cpp
        objects/slider.cpp
        objects/multislider.cpp
        objects/ingameinfobar.cpp
        objects/rotatingsprite.cpp
        objects/colorselectiondialog.cpp
        objects/buildlistdialog.cpp
        objects/coinfodialog.cpp
        objects/dialogmodifyunit.cpp
        objects/playerselection.cpp
        objects/playerselectiondialog.cpp
        objects/selectkey.cpp
        objects/dialogtextinput.cpp
        objects/chat.cpp
        objects/dialogvictoryconditions.cpp
        objects/dialogconnecting.cpp
        objects/dialogmessagebox.cpp
        objects/dialograndommap.cpp
        objects/dialogmodifyterrain.cpp
        objects/mapselectionview.cpp
        objects/timespinbox.cpp
        objects/ruleselection.cpp
        objects/ruleselectiondialog.cpp
        objects/folderdialog.cpp
        objects/coinfoactor.cpp
        objects/tableview.cpp
        objects/dialogcostyle.cpp
        objects/colorselector.cpp
        objects/cobannlistdialog.cpp

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
        coreengine/dijkstramap.cpp
        coreengine/timer.cpp
        coreengine/userdata.cpp
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
        game/playerinfo.cpp
        game/GameEnums.cpp
        game/gamerules.cpp
        game/victoryrule.cpp
        game/weather.cpp
        # cool ingame recording
        game/gamerecording/daytodayrecord.cpp
        game/gamerecording/playerrecord.cpp
        game/gamerecording/SpecialEvent.cpp
        game/gamerecording/gamerecorder.cpp
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

        # game input
        gameinput/basegameinputif.cpp
        gameinput/humanplayerinput.cpp
        gameinput/humanplayerinputmenu.cpp
        gameinput/menudata.cpp
        gameinput/markedfielddata.cpp
        gameinput/cursordata.cpp

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

        # multiplayer
        multiplayer/lobbymenu.cpp
        multiplayer/multiplayermenu.cpp
        multiplayer/gameserver.cpp
        multiplayer/gamedata.cpp

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
        ingamescriptsupport/conditions/scriptconditionbuildingcaptured.cpp
        ingamescriptsupport/conditions/scriptconditionplayerdefeated.cpp
        ingamescriptsupport/conditions/scriptconditionunitsdestroyed.cpp
        ingamescriptsupport/conditions/scriptconditionbuildingsowned.cpp
        # event stuff
        ingamescriptsupport/events/scriptevent.cpp
        ingamescriptsupport/events/scripteventdialog.cpp
        ingamescriptsupport/events/scripteventspawnunit.cpp
        ingamescriptsupport/events/scripteventdefeatplayer.cpp
        ingamescriptsupport/events/scripteventchangebuildlist.cpp
        ingamescriptsupport/events/scripteventaddfunds.cpp
        ingamescriptsupport/events/scripteventchangeweather.cpp
        ingamescriptsupport/events/scripteventchangecobar.cpp

        # wiki stuff
        wiki/terraininfo.cpp
        wiki/unitinfo.cpp
        wiki/fieldinfo.cpp
        wiki/wikidatabase.cpp
        wiki/wikipage.cpp

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

# create translation here :)
set(LUPDATE_OPTIONS "" CACHE STRING "specifies options passed to lupdate")
# created translation files
set(TS_FILES
    translation/lang_de_DE.ts
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



qt5_use_modules(${PROJECT_NAME}
    Core
    LinguistTools
    Qml
    Multimedia
    Network
    Gui
    Widgets
    Xml
)

# link libraries
target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

# install section
set(CMAKE_INSTALL_PREFIX "../install/${CMAKE_BUILD_TYPE}")

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${_qmake_executable}" DIRECTORY)

#find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

message(${QT_BIN_DIR})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
         ${QT_BIN_DIR}/d3dcompiler_47.dll
         ${QT_BIN_DIR}/libEGLd.dll
         ${QT_BIN_DIR}/libgcc_s_dw2-1.dll
         ${QT_BIN_DIR}/libGLESv2d.dll
         ${QT_BIN_DIR}/libstdc++-6.dll
         ${QT_BIN_DIR}/libwinpthread-1.dll
         ${QT_BIN_DIR}/Qt5Cored.dll
         ${QT_BIN_DIR}/Qt5Guid.dll
         ${QT_BIN_DIR}/Qt5Multimediad.dll
         ${QT_BIN_DIR}/Qt5Networkd.dll
         ${QT_BIN_DIR}/Qt5Qmld.dll
         ${QT_BIN_DIR}/Qt5Svgd.dll
         ${QT_BIN_DIR}/Qt5Widgetsd.dll
         ${QT_BIN_DIR}/Qt5Xmld.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
         ${QT_BIN_DIR}/../plugins/mediaservice/dsengined.dll
         ${QT_BIN_DIR}/../plugins/mediaservice/qtmedia_audioengined.dll
    )
    set(${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS
        ${QT_BIN_DIR}/../plugins/audio/qtaudio_windowsd.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS
        ${QT_BIN_DIR}/../plugins/bearer/qgenericbearerd.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
        ${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS
        ${QT_BIN_DIR}/../plugins/playlistformats/qtmultimedia_m3ud.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS
        ${QT_BIN_DIR}/../plugins/styles/qwindowsvistastyled.dll
    )
else("Release")
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
    )

    set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
         ${QT_BIN_DIR}/../plugins/mediaservice/dsengine.dll
         ${QT_BIN_DIR}/../plugins/mediaservice/qtmedia_audioengine.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_AUDIO_LIBS
         ${QT_BIN_DIR}/../plugins/audio/qtaudio_windows.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_BEARER_LIBS
         ${QT_BIN_DIR}/../plugins/bearer/qgenericbearer.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_PLATFORMS_LIBS
         ${QT_BIN_DIR}/../plugins/platforms/qwindows.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_PLAYLISTFORAMTS_LIBS
         ${QT_BIN_DIR}/../plugins/playlistformats/qtmultimedia_m3u.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_STYLES_LIBS
         ${QT_BIN_DIR}/../plugins/styles/qwindowsvistastyle.dll
    )
endif()

# install libs
install(DIRECTORY system DESTINATION ".")
install(DIRECTORY resources DESTINATION ".")
install(DIRECTORY maps DESTINATION ".")
install(DIRECTORY customTerrainImages DESTINATION ".")
install(DIRECTORY mods DESTINATION ".")
install(DIRECTORY templates DESTINATION ".")
install(DIRECTORY savegames DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION ".")
# install exe
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

# execute_process (
#    COMMAND ${QT_BIN_DIR}/windeployqt.exe "D:/Programmierung/Commander_Wars/builds/install/Release/Commander_Wars.exe")
