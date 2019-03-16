cmake_minimum_required (VERSION 2.8.11)
project (Commander_Wars)

# add oxygen as our engine
add_subdirectory(3rd_party/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_subdirectory(3rd_party/oxygine-flow oxygine-flow)
include_directories(${OXYGINE_FLOW_INCLUDE_DIRS})


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# add qt to our project
find_package(Qt5Core REQUIRED)
# Qt5LinguistTools
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5QML REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5Network REQUIRED)

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

# disbale cxx 1 future for qt.
add_definitions(
    -DQT_FEATURE_cxx11_future=-1
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS} -DCMAKE_C_FLAGS=\"-mwindows\"")

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

        #menues
        menue/mainwindow.cpp
        menue/ingamemenue.cpp
        menue/editormenue.cpp
        menue/gamemenue.cpp
        menue/mapselectionmapsmenue.cpp

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

        # core engine
        coreengine/mainapp.cpp
        coreengine/settings.cpp
        coreengine/interpreter.cpp
        coreengine/console.cpp
        coreengine/audiothread.cpp
        coreengine/pathfindingsystem.cpp
        coreengine/tweentogglevisibility.cpp
        coreengine/tweenwait.cpp
        coreengine/qmlvector.cpp

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
        game/gameanimation.cpp
        game/gameanimationfactory.cpp
        game/gameanimationwalk.cpp
        game/gameanimationcapture.cpp
        game/playerinfo.cpp
        game/GameEnums.cpp

        # game input
        gameinput/basegameinputif.cpp
        gameinput/humanplayerinput.cpp
        gameinput/humanplayerinputmenu.cpp
        gameinput/menudata.cpp
        gameinput/markedfielddata.cpp
        gameinput/cursordata.cpp

        # map importing\exporting support
        # and resizing etc.
        mapsupport/importcowtxt.cpp
        mapsupport/refactorMap.cpp
        mapsupport/randomMap.cpp

        # ai
        ai/leaf.cpp
        ai/decisionnode.cpp
        ai/question.cpp
        ai/decisiontree.cpp
)


FILE(GLOB_RECURSE JavaScripts "resources/*.js")
add_custom_target(scripts SOURCES ${JavaScripts})

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
        ${${PROJECT_NAME}_SRCS}
        ICON.rc
)
# link libraries
target_link_libraries(
        ${PROJECT_NAME}
        oxygine-flow
	${OXYGINE_CORE_LIBS}
        Qt5::Core
        Qt5::Qml
        Qt5::Multimedia
        Qt5::Network
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

#find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
         ${QT_BIN_DIR}/Qt5Qmld.dll
         ${QT_BIN_DIR}/Qt5Cored.dll
         ${QT_BIN_DIR}/Qt5Networkd.dll
         ${QT_BIN_DIR}/Qt5Multimediad.dll
         ${QT_BIN_DIR}/Qt5Guid.dll
         ${QT_BIN_DIR}/Qt5Widgetsd.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
         ${QT_BIN_DIR}/../plugins/mediaservice/dsengined.dll
         ${QT_BIN_DIR}/../plugins/mediaservice/qtmedia_audioengined.dll
    )
else("Release")
    set(${PROJECT_NAME}_INSTALL_QT_LIBS
        ${QT_BIN_DIR}/Qt5Qml.dll
        ${QT_BIN_DIR}/Qt5Core.dll
        ${QT_BIN_DIR}/Qt5Network.dll
        ${QT_BIN_DIR}/Qt5Multimedia.dll
        ${QT_BIN_DIR}/Qt5Gui.dll
        ${QT_BIN_DIR}/Qt5Widgets.dll
    )

    set(${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS
         ${QT_BIN_DIR}/../plugins/mediaservice/dsengine.dll
         ${QT_BIN_DIR}/../plugins/mediaservice/qtmedia_audioengine.dll
    )
endif()

# install libs
install(DIRECTORY resources DESTINATION ".")
install(DIRECTORY maps DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_LIBS} DESTINATION ".")
# install exe
install(TARGETS ${PROJECT_NAME} DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_LIBS} DESTINATION ".")
install(FILES ${${PROJECT_NAME}_INSTALL_QT_MULTIMEDIA_LIBS} DESTINATION "mediaservice_dis")
# install translations
install(FILES ${QM_FILES} DESTINATION "resources/translation/")

