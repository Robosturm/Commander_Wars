#include <QFile>
#include <QDir>
#include <QCryptographicHash>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "menue/editormenue.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiomanager.h"
#include "coreengine/globalutils.h"
#include "coreengine/pathfindingsystem.h"
#include "coreengine/gameconsole.h"

#include "resource_management/movementtablemanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/editor/dialogmodifyunit.h"
#include "objects/dialogs/editor/dialogmodifybuilding.h"
#include "objects/dialogs/rules/playerselectiondialog.h"
#include "objects/dialogs/rules/ruleselectiondialog.h"
#include "objects/dialogs/editor/dialograndommap.h"
#include "objects/dialogs/editor/dialogmodifyterrain.h"
#include "objects/dialogs/editor/dialogviewmapstats.h"
#include "objects/dialogs/editor/dialogextendmap.h"
#include "objects/dialogs/customdialog.h"

#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/label.h"
#include "objects/base/selectkey.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/campaigneditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "game/terrainfindingsystem.h"
#include "game/co.h"

#include "mapsupport/randomMapGenerator.h"

#include "wiki/fieldinfo.h"

#include "ui_reader/uifactory.h"

EditorMenue::EditorMenue()
    : BaseGamemenu (20, 20, "", false),
      m_autosaveTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("EditorMenue");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    qint32 selectionWidth = oxygine::Stage::getStage()->getWidth() / 4;
    bool smallScreen = Settings::getInstance()->getSmallScreenDevice();
    Interpreter::setCppOwnerShip(this);
    registerAtInterpreter();
    if (smallScreen)
    {
        selectionWidth = oxygine::Stage::getStage()->getWidth() * 3 / 4;
    }
    if (selectionWidth < 255)
    {
        selectionWidth = 255;
    }
    if (smallScreen)
    {
        m_autoScrollBorder = QRect(50, 135, 50, 50);
        initSlidingActor(50, 125, oxygine::Stage::getStage()->getWidth() - 100, oxygine::Stage::getStage()->getHeight() - 175);
    }
    else
    {
        m_autoScrollBorder = QRect(50, 135, selectionWidth, 50);
        initSlidingActor(50, 125, oxygine::Stage::getStage()->getWidth() - selectionWidth - 100, oxygine::Stage::getStage()->getHeight() - 175);
    }
    m_mapSlidingActor->addChild(m_pMap);
    m_mapSliding->setLocked(true);
    loadHandling();
    changeBackground("editormenu");

    m_EditorSelection = MemoryManagement::create<EditorSelection>(selectionWidth, smallScreen, m_pMap.get());
    addChild(m_EditorSelection);

    m_Topbar = MemoryManagement::create<Topbar>(0, oxygine::Stage::getStage()->getWidth());

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/mapeditor");
    pApp->getAudioManager()->playRandom();

    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addItem(tr("View map stats"),     "VIEWMAPSTATS", 0, tr("Shows the general information about the map."));
    m_Topbar->addItem(tr("Save map"),           "SAVEMAP",      0, tr("Saves a map to a give file."));
    m_Topbar->addItem(tr("Load map"),           "LOADMAP",      0, tr("Loads a map to a give file."));
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        m_Topbar->addItem(tr("Edit script"),    "EDITSCRIPT",   0, tr("Edit and create a script for any map."));
        m_Topbar->addItem(tr("Edit campaign"),  "EDITCAMPAIGN", 0, tr("Edit and create a campaign."));
    }
    m_Topbar->addItem(tr("Undo Ctrl+Z"),        "UNDO",         0, tr("Undo the last map modification."));
    m_Topbar->addItem(tr("Redo Ctrl+Y"),        "REDO",         0, tr("Redo the last undo command."));
    m_Topbar->addItem(tr("Exit editor"),        "EXIT",         0, tr("Exits the editor"));

    m_Topbar->addGroup(tr("Map info"));
    m_Topbar->addItem(tr("New map"),            "NEWMAP",       1, tr("Create a new map"));
    m_Topbar->addItem(tr("Edit map"),           "EDITMAP",      1, tr("Edit the information for a map"));
    m_Topbar->addItem(tr("Resize map"),         "RESIZEMAP",    1, tr("Resizes the map using left, top, right and bottom size changes."));
    m_Topbar->addItem(tr("Extend map"),         "EXTENDMAP",    1, tr("Extends this map with another map"));
    m_Topbar->addItem(tr("Flip map X"),         "FLIPX",        1, tr("Flips the map at the x-axis. Flipping the left half of the map. The right half of the map is changed."));
    m_Topbar->addItem(tr("Flip map Y"),         "FLIPY",        1, tr("Flips the map at the y-axis. Flipping the top half of the map. The bottom half of the map is changed."));
    m_Topbar->addItem(tr("Rotate map X 180°"),  "ROTATEX",      1, tr("Flips and rotates the map at the x-axis. Using the left half of the map. The right half of the map is changed."));
    m_Topbar->addItem(tr("Rotate map X 90°"),   "ROTATEX90",    1, tr("Rotates the upper left quarter into the upper right quarter of the map and rotates the lower right quarter into the lower left quarter of the map "));
    m_Topbar->addItem(tr("Rotate map Y 180°"),  "ROTATEY",      1, tr("Flips and rotates the map at the y-axis. Using the top half of the map. The bottom half of the map is changed."));
    m_Topbar->addItem(tr("Rotate map Y 90°"),   "ROTATEY90",    1, tr("Rotates the upper left quarter into the lower left quarter of the map and rotates the lower right quarter into the upper right quarter of the map "));
    m_Topbar->addItem(tr("Random map"),         "RANDOMMAP",    1, tr("Creates a new random map."));
    m_Topbar->addItem(tr("Toggle grid ") + SelectKey::getKeycodeText(Settings::getInstance()->getKey_toggleGridLayout()), "TOGGLEGRID",   1, tr("Shows or hides a grid layout."));
    m_Topbar->addItem(tr("Toggle cross Strg+M"), "TOGGLEMIDDLECROSS", 1, tr("Shows or hides the cross marking the middle of the map."));
    m_Topbar->addItem(tr("Update sprites"),     "UPDATESPRITES", 1, tr("Reloads the map sprites in case something didn't load correctly."));

    m_Topbar->addGroup(tr("Commands"));
    m_Topbar->addItem(tr("Place selection"), "PLACESELECTION", 2, tr("Selects the editor mode placing the current tile"));
    m_Topbar->addItem(tr("Delete units") + " - " + SelectKey::getKeycodeText(Settings::getInstance()->getKey_cancel()), "DELETEUNITS", 2, tr("Selects the editor mode deleting units"));
    m_Topbar->addItem(tr("Edit units"), "EDITUNITS", 2, tr("Selects the editor mode modifying the stats of a unit"));
    m_Topbar->addItem(tr("Edit terrain"), "EDITTERRAIN", 2, tr("Selects the editor mode editing the style of a terrain or building"));
    m_Topbar->addItem(tr("Edit players"), "EDITPLAYERS", 2, tr("Edit the CO's and player start setup."));
    m_Topbar->addItem(tr("Edit rules"), "EDITRULES", 2, tr("Selects the editor rules for the map."));
    m_Topbar->addItem(tr("Mass edit terrain"), "EDITBIOMES", 2, tr("Changes the biome or palettes of an area."));

    m_Topbar->addItem(tr("Optimize players"), "OPTIMIZEPLAYERS", 2, tr("Removes all players with no units or buildings from the map"));
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        m_Topbar->addItem(tr("Copy Ctrl+C"), "COPY", 2, tr("Enters the copy mode. Hold the left mouse key and mark the fields you want to copy. Copying is based on the current placing mode"));
        m_Topbar->addItem(tr("Paste Ctrl+V"), "PASTE", 2, tr("Paste the current selected area. Based on the current placing mode. The copy and paste selection are not allowed to intersec."));
        m_Topbar->addItem(tr("Paste Ctrl+Shift+V"), "PASTEALL", 2, tr("Paste the current selected area with all terrain, buildings, units. The copy and paste selection are not allowed to intersec."));
    }

    m_Topbar->addGroup(tr("Import/Export"));
    m_Topbar->addItem(tr("Import CoW Txt"), "IMPORTCOWTXT", 3, tr("Deletes the current map and imports an old Commander Wars Map from a file."));
    m_Topbar->addItem(tr("Import AWDS Aws"), "IMPORTAWDSAWS", 3, tr("Deletes the current map and imports an AWS Map Editor from a file."));
    m_Topbar->addItem(tr("Export AWDS Aws"), "EXPORTAWDSAWS", 3, tr("Exports the map to an AWS Map Editor file"));
    m_Topbar->addItem(tr("Import AW4 Aw4"), "IMPORTAW4AW4", 3, tr("Deletes the current map and imports an AW 4 map editor file."));
    m_Topbar->addItem(tr("Import AW by Web"), "IMPORTAWBYWEB", 3, tr("Deletes the current map and imports an  Advance Wars by Web Map from https://awbw.amarriner.com/"));
    m_Topbar->addItem(tr("Import AW by Web ID"), "IMPORTAWBYWEBBYMAPID", 3, tr("Deletes the current map and imports an Advance Wars by Web Map from https://awbw.amarriner.com/ via the map id"));

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pButtonBox->setResAnim(pAnim);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        m_xyTextInfo = MemoryManagement::create<Label>(180);
        m_xyTextInfo->setStyle(style);
        m_xyTextInfo->setHtmlText("X: 0 Y: 0");
        m_xyTextInfo->setPosition(8, 8);
        pButtonBox->addChild(m_xyTextInfo);
        pButtonBox->setSize(200, 50);
        pButtonBox->setPosition((oxygine::Stage::getStage()->getWidth() - m_EditorSelection->getScaledWidth()) - pButtonBox->getScaledWidth(), -4 + m_Topbar->getScaledHeight());
        pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        addChild(pButtonBox);
    }

    Cursor* pCursor = m_Cursor.get();
    m_pMap->addEventListener(oxygine::TouchEvent::MOVE, [this, pCursor](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            static qint32 lastX = -1;
            static qint32 lastY = -1;
            if (pTouchEvent->getPointer()->isPressed(oxygine::MouseButton::MouseButton_Left))
            {
                if (pCursor->getMapPointX() != lastX ||
                    pCursor->getMapPointY() != lastY)
                {
                    lastX = pCursor->getMapPointX();
                    lastY = pCursor->getMapPointY();
                    emit sigLeftClick(pCursor->getMapPointX(), pCursor->getMapPointY());
                }
            }
            else
            {
                lastX = -1;
                lastY = -1;
                // ignore case
            }
        }
    });

    m_Topbar->finishCreation();
    addChild(m_Topbar);

    // connecting stuff
    connect(this, &EditorMenue::sigLeftClick, this, &EditorMenue::onMapClickedLeft, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigLeftClickDown, this, &EditorMenue::onMapClickedLeftDown, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigLeftClickUp, this, &EditorMenue::onMapClickedLeftUp, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigRightClick, this, &EditorMenue::onMapClickedRight, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, this, &EditorMenue::cursorMoved, Qt::QueuedConnection);
    connect(m_Topbar.get(), &Topbar::sigItemClicked, this, &EditorMenue::clickedTopbar, Qt::QueuedConnection);
    connect(m_EditorSelection.get(), &EditorSelection::sigSelectionChanged, this, &EditorMenue::selectionChanged, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigResizeMap, this, &EditorMenue::resizeMap, Qt::QueuedConnection);
    connect(&m_awbwMapDownloader, &AwbwMapDownloader::sigDownloadResult, this, &EditorMenue::onAwbwMapDownloadResult, Qt::QueuedConnection);

    m_HumanInput = MemoryManagement::create<HumanPlayerInput>(m_pMap.get());
    m_HumanInput->init(this);
    m_HumanInput->setLeftClickEnabled(false);

    // clean up temp folder
    QDir dir = m_tempDir.path();
    dir.removeRecursively();
    dir.mkpath(".");

    m_autosaveTimer.setSingleShot(false);
    connect(&m_autosaveTimer, &QTimer::timeout, this, &EditorMenue::autosave, Qt::QueuedConnection);
    m_autosaveTimer.start(60 * 1000);
    UiFactory::getInstance().createUi("ui/editormenu.xml", this);
    pApp->continueRendering();
}

void EditorMenue::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "mapEditorMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

EditorMenue::~EditorMenue()
{
    cleanTemp(-1);

}

void EditorMenue::cleanTemp(qint32 step)
{
    QDir dir = m_tempDir.path();
    if (step < 0)
    {
        m_tempCounter = 0;
        dir.removeRecursively();
        dir.mkpath(".");
    }
    else
    {
        for (qint32 i = step; i < std::numeric_limits<qint32>::max(); i++)
        {
            QFile file(m_tempDir.path() + "/temp" + QString::number(i) + ".tmp");
            if (file.exists())
            {
                file.remove();
            }
            else
            {
                break;
            }
        }
    }
}

void EditorMenue::createTempFile(bool cleanUp)
{
    CONSOLE_PRINT(QString("createTempFile(") + (cleanUp ? "true" : "false") + ")", GameConsole::eDEBUG);
    if (cleanUp)
    {
        cleanTemp(m_tempCounter);
    }
    QFile file(m_tempDir.path() + "/temp" + QString::number(m_tempCounter) + ".tmp");
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    
    m_pMap->serializeObject(stream);
    file.close();

    QFile previous(m_tempDir.path() + "/temp" + QString::number(m_tempCounter - 1) + ".tmp");
    if (previous.exists())
    {
        file.open(QIODevice::ReadOnly);
        QCryptographicHash myHash(QCryptographicHash::Sha512);
        myHash.addData(file.readAll());
        QByteArray hash = myHash.result();
        file.close();

        previous.open(QIODevice::ReadOnly);
        QCryptographicHash myHash1(QCryptographicHash::Sha512);
        myHash1.addData(previous.readAll());
        QByteArray hash1 = myHash1.result();
        previous.close();
        // no changes don't save map and don't increase counters
        if (hash1 == hash)
        {
            file.remove();
        }
        else
        {
            m_tempCounter++;
        }
    }
    else
    {
        m_tempCounter++;
    }
}

void EditorMenue::editorUndo()
{
    CONSOLE_PRINT("EditorMenue::editorUndo", GameConsole::eDEBUG);
    m_tempCounter--;
    if (m_tempCounter >= 0)
    {
        QFile file(m_tempDir.path() + "/temp" + QString::number(m_tempCounter) + ".tmp");
        if (file.exists())
        {
            m_tempCounter++;
            createTempFile(false);
            m_tempCounter -= 2;
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            m_pMap->deserializeObject(stream);
            file.close();
            
            Mainapp::getInstance()->pauseRendering();
            m_pMap->updateSprites(-1, -1, true);
            Mainapp::getInstance()->continueRendering();
            m_EditorSelection->createPlayerSelection();
        }
    }
    else
    {
        m_tempCounter = 0;
    }
}

void EditorMenue::editorRedo()
{
    CONSOLE_PRINT("EditorMenue::editorRedo", GameConsole::eDEBUG);
    m_tempCounter++;
    QFile file(m_tempDir.path() + "/temp" + QString::number(m_tempCounter) + ".tmp");
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        m_pMap->deserializeObject(stream);
        file.close();
        
        Mainapp::getInstance()->pauseRendering();
        m_pMap->updateSprites(-1, -1, true);
        Mainapp::getInstance()->continueRendering();
        m_EditorSelection->createPlayerSelection();
    }
}

void EditorMenue::clickedTopbar(QString itemID)
{    
    CONSOLE_PRINT("clickedTopbar(" + itemID + ")", GameConsole::eDEBUG);
    struct MenuItem
    {
        MenuItem(const char* const id, void (EditorMenue::*func)())
            : m_Id(id),
              m_func(func)
        {
        }
        const char* const m_Id;
        void (EditorMenue::*m_func)();
    };
    QVector<MenuItem> items =
    {
        MenuItem("EXIT",                &EditorMenue::showExit),
        MenuItem("SAVEMAP",             &EditorMenue::showSaveMap),
        MenuItem("LOADMAP",             &EditorMenue::showLoadMap),
        MenuItem("UNDO",                &EditorMenue::editorUndo),
        MenuItem("REDO",                &EditorMenue::editorRedo),
        MenuItem("EDITSCRIPT",          &EditorMenue::showEditScript),
        MenuItem("EDITCAMPAIGN",        &EditorMenue::showEditCampaign),
        MenuItem("IMPORTCOWTXT",        &EditorMenue::showImportCoWTxTMap),
        MenuItem("IMPORTAWDSAWS",       &EditorMenue::showImportAwdsAws),
        MenuItem("EXPORTAWDSAWS",       &EditorMenue::showExportAwdsAws),
        MenuItem("IMPORTAW4AW4",        &EditorMenue::showImportAwdsAw4),
        MenuItem("IMPORTAWBYWEB",       &EditorMenue::showImportAwByWeb),
        MenuItem("IMPORTAWBYWEBBYMAPID",&EditorMenue::showImportAwByWebByMapId),
        MenuItem("NEWMAP",              &EditorMenue::showNewMap),
        MenuItem("EDITMAP",             &EditorMenue::showEditMap),
        MenuItem("EXTENDMAP",           &EditorMenue::showExtendMap),
        MenuItem("FLIPX",               &EditorMenue::flipX),
        MenuItem("FLIPY",               &EditorMenue::flipY),
        MenuItem("ROTATEX",             &EditorMenue::rotateX),
        MenuItem("ROTATEY",             &EditorMenue::rotateY),        
        MenuItem("ROTATEX90",           &EditorMenue::rotateX90),
        MenuItem("ROTATEY90",           &EditorMenue::rotateY90),
        MenuItem("RANDOMMAP",           &EditorMenue::showRandomMap),
        MenuItem("PLACESELECTION",      &EditorMenue::changePlaceSelection),
        MenuItem("DELETEUNITS",         &EditorMenue::deleteUnits),
        MenuItem("EDITUNITS",           &EditorMenue::editUnits),
        MenuItem("EDITTERRAIN",         &EditorMenue::editTerrains),
        MenuItem("OPTIMIZEPLAYERS",     &EditorMenue::optimizePlayers),
        MenuItem("EDITPLAYERS",         &EditorMenue::showEditPlayers),
        MenuItem("EDITRULES",           &EditorMenue::showEditRules),
        MenuItem("COPY",                &EditorMenue::copy),
        MenuItem("PASTE",               nullptr),
        MenuItem("PASTEALL",            nullptr),
        MenuItem("RESIZEMAP",           &EditorMenue::showResizeMap),
        MenuItem("TOGGLEGRID",          &EditorMenue::toggleGridLayout),
        MenuItem("TOGGLEMIDDLECROSS",   &EditorMenue::toggleMiddleCrossGrid),
        MenuItem("VIEWMAPSTATS",        &EditorMenue::viewMapStats),
        MenuItem("EDITBIOMES",          &EditorMenue::showEditBiomes),
        MenuItem("UPDATESPRITES",       &EditorMenue::updateSprites),
    };
    for (auto & item : std::as_const(items))
    {
        if (item.m_Id == itemID &&
            item.m_func != nullptr)
        {
            (this->*(item.m_func))();
            break;
        }
    }
}

void EditorMenue::updateSprites()
{
    Mainapp::getInstance()->pauseRendering();
    m_pMap->updateSprites(-1, -1, true);
    Mainapp::getInstance()->continueRendering();
}

void EditorMenue::showEditBiomes()
{
    spCustomDialog dialog = MemoryManagement::create<CustomDialog>("", "ui/editor/mapEditBiomes.xml", this);
    addChild(dialog);
    connect(dialog.get(), &CustomDialog::sigFinished, this, [this]()
    {
        setFocused(true);
    }, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::viewMapStats()
{
    setFocused(false);
    spDialogViewMapStats pViewMapStats = MemoryManagement::create<DialogViewMapStats>(m_pMap.get());
    addChild(pViewMapStats);
    connect(pViewMapStats.get(),  &DialogViewMapStats::sigClosed, this, &EditorMenue::focusEditor, Qt::QueuedConnection);
    pViewMapStats->init();
}

void EditorMenue::focusEditor()
{
    setFocused(true);
}

void EditorMenue::toggleGridLayout()
{    
    if (m_pMap.get() != nullptr)
    {
        m_gridVisible = !m_gridVisible;
        m_pMap->showGrid(m_gridVisible);
    }
}

void EditorMenue::toggleMiddleCrossGrid()
{    
    if (m_pMap.get() != nullptr)
    {
        m_middleCrossGridVisible = !m_middleCrossGridVisible;
        m_pMap->showMiddleCrossGrid(m_middleCrossGridVisible);
    }
}

void EditorMenue::updateGrids()
{    
    if (m_pMap.get() != nullptr)
    {
        m_pMap->showMiddleCrossGrid(m_middleCrossGridVisible);
        m_pMap->showGrid(m_gridVisible);
    }
}

void EditorMenue::showSaveMap()
{
    QStringList wildcards;
    wildcards.append("*.map");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, true, m_pMap->getMapName(), false, tr("Save"));
    addChild(fileDialog);
    connect(fileDialog.get(), &FileDialog::sigFileSelected, this, &EditorMenue::saveMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showLoadMap()
{
    QStringList wildcards;
    wildcards.append("*.map");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::loadMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showResizeMap()
{
    CONSOLE_PRINT("showResizeMap()", GameConsole::eDEBUG);
    
    spGenericBox pBox = MemoryManagement::create<GenericBox>(true);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    qint32 y = 30;

    spLabel pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Left: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox leftBox = MemoryManagement::create<SpinBox>(150, -m_pMap->getMapWidth() + 1, 999999);
    leftBox->setTooltipText(tr("Change of the map size on the left map border."));
    leftBox->setPosition(width, y);
    leftBox->setInfinityValue(-m_pMap->getMapWidth());
    leftBox->setCurrentValue(0);
    pBox->addItem(leftBox);
    y += pText->getHeight() + 10;

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Top: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox topBox = MemoryManagement::create<SpinBox>(150, -m_pMap->getMapHeight() + 1, 999999);
    topBox->setTooltipText(tr("Change of the map size on the top map border."));
    topBox->setPosition(width, y);
    topBox->setInfinityValue(-m_pMap->getMapHeight());
    topBox->setCurrentValue(0);
    pBox->addItem(topBox);
    y += pText->getHeight() + 10;

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Right: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox rightBox = MemoryManagement::create<SpinBox>(150, -m_pMap->getMapWidth() + 1, 999999);
    rightBox->setTooltipText(tr("Change of the map size on the right map border."));
    rightBox->setPosition(width, y);
    rightBox->setInfinityValue(-m_pMap->getMapWidth());
    rightBox->setCurrentValue(0);
    pBox->addItem(rightBox);
    y += pText->getHeight() + 10;

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Bottom: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox bottomBox = MemoryManagement::create<SpinBox>(150, -m_pMap->getMapHeight() + 1, 999999);
    bottomBox->setTooltipText(tr("Change of the map size on the bottom map border."));
    bottomBox->setPosition(width, y);
    bottomBox->setInfinityValue(-m_pMap->getMapHeight());
    bottomBox->setCurrentValue(0);
    pBox->addItem(bottomBox);
    y += pText->getHeight() + 10;

    addChild(pBox);
    auto* pLeftBox = leftBox.get();
    auto* pRightBox = rightBox.get();
    auto* pTopBox = topBox.get();
    auto* pBottomBox = bottomBox.get();

    connect(pBox.get(), &GenericBox::sigFinished, this, [this, pLeftBox, pTopBox, pRightBox, pBottomBox]
    {
        emit sigResizeMap(pLeftBox->getCurrentValue(), pTopBox->getCurrentValue(),
                          pRightBox->getCurrentValue(), pBottomBox->getCurrentValue());
    });
    
}

void EditorMenue::showEditScript()
{
    spScriptEditor scriptEditor = MemoryManagement::create<ScriptEditor>(m_pMap.get());
    addChild(scriptEditor);
    connect(scriptEditor.get(),  &ScriptEditor::sigFinished, this, &EditorMenue::scriptFinished, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showEditCampaign()
{
    spCampaignEditor campaignEditor = MemoryManagement::create<CampaignEditor>();
    addChild(campaignEditor);
    connect(campaignEditor.get(),  &CampaignEditor::sigFinished, this, &EditorMenue::campaignFinished, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportCoWTxTMap()
{
    QStringList wildcards;
    wildcards.append("*.txt");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Import"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importCoWTxTMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwdsAws()
{
    QStringList wildcards;
    wildcards.append("*.aws");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Import"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWDSAwsMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showExportAwdsAws()
{
    QStringList wildcards;
    wildcards.append("*.aws");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, true, "", false, tr("Export"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::exportAWDSAwsMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwdsAw4()
{
    QStringList wildcards;
    wildcards.append("*.aw4");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Import"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAW4Aw4Map, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwByWeb()
{
    QStringList wildcards;
    wildcards.append("*.txt");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Import"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWByWeb, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwByWebByMapId()
{
    spCustomDialog pDialog = MemoryManagement::create<CustomDialog>("", "ui/editor/importAwbwmapById.xml", this);
    addChild(pDialog);
    setFocused(false);
}

void EditorMenue::downloadAwbwMapById(quint32 mapId)
{
    m_awbwMapDownloader.startMapDownload(mapId);
}

void EditorMenue::onAwbwMapDownloadResult(bool success)
{
    if (success)
    {
        cleanTemp(-1);
        m_awbwMapDownloader.loadMap(m_pMap.get());
        editFinishedCanceled();
        m_EditorSelection->createPlayerSelection();
    }
    else
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Failed to download map."));
        addChild(pDialogMessageBox);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    }
}

void EditorMenue::showNewMap()
{
    MapEditDialog::MapEditInfo info;
    info.author = Settings::getInstance()->getUsername();
    info.mapWidth = 20;
    info.mapHeigth = 20;
    info.playerCount = 4;
    spMapEditDialog mapEditDialog = MemoryManagement::create<MapEditDialog>(info, tr("Do you want to discard all current changes and create a new map?"));
    connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::newMap, Qt::QueuedConnection);
    connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    addChild(mapEditDialog);
    setFocused(false);
}

void EditorMenue::showEditMap()
{
    MapEditDialog::MapEditInfo info;
    info.mapName = m_pMap->getMapName();
    info.author = m_pMap->getMapAuthor();
    info.description = m_pMap->getMapDescription();
    info.scriptFile = m_pMap->getGameScript()->getScriptFile();
    info.mapWidth = m_pMap->getMapWidth();
    info.mapHeigth = m_pMap->getMapHeight();
    info.playerCount = m_pMap->getPlayerCount();
    info.turnLimit = m_pMap->getGameRecorder()->getMapTime();
    info.deployLimit = m_pMap->getGameRecorder()->getDeployLimit();
    info.mapFlags = m_pMap->getMapFlags();
    spMapEditDialog mapEditDialog = MemoryManagement::create<MapEditDialog>(info, tr("Do you want to apply the map changes?"));
    connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::changeMap, Qt::QueuedConnection);
    connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    addChild(mapEditDialog);
    setFocused(false);
}

void EditorMenue::showExtendMap()
{
    spDialogExtendMap pDialogExtendMap = MemoryManagement::create<DialogExtendMap>();
    connect(pDialogExtendMap.get(), &DialogExtendMap::sigOk, this, &EditorMenue::extendMap, Qt::QueuedConnection);
    addChild(pDialogExtendMap);
    setFocused(false);
}

void EditorMenue::extendMap(const QString & mapFile, GameEnums::Directions direction)
{
    createTempFile();
    m_pMap->extendMap(mapFile, direction);
    setFocused(true);
}

void EditorMenue::flipX()
{
    createTempFile();
    spGameMap pGameMap = m_pMap;
    pGameMap->flipX();
}

void EditorMenue::flipY()
{
    createTempFile();
    spGameMap  pGameMap = m_pMap;
    pGameMap->flipY();
}

void EditorMenue::rotateX()
{
    createTempFile();
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateX();
}

void EditorMenue::rotateY()
{
    createTempFile();
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateY();
}

void EditorMenue::rotateX90()
{
    createTempFile();
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateX90();
}

void EditorMenue::rotateY90()
{
    createTempFile();
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateY90();
}

void EditorMenue::showRandomMap()
{
    spDialogRandomMap pDialogRandomMap = MemoryManagement::create<DialogRandomMap>(tr("Do you want to create a random map and discard all current changes?"));
    addChild(pDialogRandomMap);
    connect(pDialogRandomMap.get(), &DialogRandomMap::sigFinished, this, &EditorMenue::createRandomMap, Qt::QueuedConnection);
    connect(pDialogRandomMap.get(), &DialogRandomMap::sigCancel, this, [this]()
    {
        setFocused(true);
    }, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::changePlaceSelection()
{
    m_EditorMode = EditorModes::PlaceEditorSelection;
    selectionChanged();
}

void EditorMenue::deleteUnits()
{
    m_EditorMode = EditorModes::RemoveUnits;
    createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
}

void EditorMenue::editUnits()
{
    m_EditorMode = EditorModes::EditUnits;
    createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
}

void EditorMenue::editTerrains()
{
    m_EditorMode = EditorModes::EditTerrain;
    createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
}

void EditorMenue::showEditPlayers()
{
    spPlayerSelectionDialog pDialog = MemoryManagement::create<PlayerSelectionDialog>(m_pMap.get());
    addChild(pDialog);
    connect(pDialog.get(), &PlayerSelectionDialog::sigPlayersChanged, this, &EditorMenue::playersChanged, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showEditRules()
{
    spRuleSelectionDialog pDialog = MemoryManagement::create<RuleSelectionDialog>(m_pMap.get(), RuleSelection::Mode::Editor);
    addChild(pDialog);
    connect(pDialog.get(), &RuleSelectionDialog::sigRulesChanged, this, &EditorMenue::rulesChanged, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showExit()
{
    m_Focused = false;
    spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Do you want to exit the map editor?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &EditorMenue::exitEditor, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, this, [this]()
    {
        m_Focused = true;
    });
    addChild(pExit);
}

void EditorMenue::copy()
{
    m_EditorMode = EditorModes::CopySelection;
    m_copyRect = QRect(-1, -1, 0, 0);
}

void EditorMenue::resizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom)
{
    createTempFile();
    CONSOLE_PRINT("EditorMenue::resizeMap", GameConsole::eDEBUG);
    m_pMap->resizeMap(left, top, right, bottom);    
    updateGrids();
}

void EditorMenue::createRandomMap(QString mapName, QString author, QString description,
                                  qint32 width, qint32 heigth, qint32 playerCount,
                                  bool roadSupport, qint32 seed,
                                  QVector<std::tuple<QString, float>> terrains,
                                  QVector<std::tuple<QString, float>> buildings,
                                  QVector<float> ownedBaseSize,
                                  float startBaseSize,
                                  QVector<std::tuple<QString, float>> units,
                                  qint32 unitCount,
                                  float startBaseUnitSize,
                                  QVector<float> unitDistribution,
                                  bool unitsDistributed,
                                  bool mirrored)
{
    CONSOLE_PRINT("EditorMenue::createRandomMap", GameConsole::eDEBUG);
    cleanTemp(-1);
    RandomMapGenerator::randomMap(m_pMap, width, heigth, playerCount, roadSupport, seed,
                        terrains, buildings, ownedBaseSize, startBaseSize / 100.0f,
                        units, unitCount, startBaseUnitSize / 100.0f, unitDistribution, unitsDistributed,
                        mirrored);
    m_pMap->setMapName(mapName);
    m_pMap->setMapAuthor(author);
    m_pMap->setMapDescription(description);
    Mainapp::getInstance()->pauseRendering();
    m_pMap->updateSprites(-1, -1, true);
    Mainapp::getInstance()->continueRendering();
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    updateGrids();
}

void EditorMenue::playersChanged()
{    
    CONSOLE_PRINT("EditorMenue::playersChanged", GameConsole::eDEBUG);
    m_EditorSelection->createPlayerSelection();
    
    Mainapp::getInstance()->pauseRendering();
    m_pMap->updateSprites(-1, -1, true);
    Mainapp::getInstance()->continueRendering();
    setFocused(true);
}

void EditorMenue::rulesChanged()
{
    CONSOLE_PRINT("EditorMenue::rulesChanged", GameConsole::eDEBUG);
    setFocused(true);
}

void EditorMenue::optimizePlayers()
{
    CONSOLE_PRINT("EditorMenue::optimizePlayers", GameConsole::eDEBUG);
    createTempFile();
    m_pMap->optimizePlayers();
    m_EditorSelection->createPlayerSelection();
    
}

void EditorMenue::keyInput(oxygine::KeyEvent event)
{
    Qt::Key cur = event.getKey();
    if (m_Focused)
    {
        if (!event.getContinousPress())
        {
            if ((event.getModifiers() & Qt::KeyboardModifier::ControlModifier) > 0)
            {
                switch (cur)
                {
                    case Qt::Key_Y:
                    {
                        editorRedo();
                        break;
                    }
                    case Qt::Key_Z:
                    {
                        editorUndo();
                        break;
                    }
                    case Qt::Key_C:
                    {
                        m_EditorMode = EditorModes::CopySelection;
                        m_copyRect = QRect(-1, -1, 0, 0);
                        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle, Qt::white);
                        createMarkedArea(m_copyRectActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle, Qt::white);
                        break;
                    }
                    case Qt::Key_V:
                    {
                        if (m_copyRect.x() >= 0 && m_copyRect.y() >= 0 &&
                            m_copyRect.width() != 0 && m_copyRect.height() != 0)
                        {
                            if ((event.getModifiers() & Qt::KeyboardModifier::ShiftModifier) > 0)
                            {
                                pasteSelection(m_Cursor->getMapPointX(), m_Cursor->getMapPointY(), false, EditorSelection::EditorMode::All);
                            }
                            else
                            {
                                pasteSelection(m_Cursor->getMapPointX(), m_Cursor->getMapPointY(), false, m_EditorSelection->getCurrentMode());
                            }
                        }
                        break;
                    }
                    case Qt::Key_M:
                    {
                        toggleMiddleCrossGrid();
                        break;
                    }
                    default:
                    {                        
                        break;
                    }
                }
            }
            else if (cur == Settings::getInstance()->getKey_information() ||
                     cur == Settings::getInstance()->getKey_information2())
            {
                
                if (m_pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
                {
                    spTerrain pTerrain = m_pMap->getSpTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    spFieldInfo fieldinfo = MemoryManagement::create<FieldInfo>(pTerrain, pTerrain->getSpUnit());
                    addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, this, [this]
                    {
                        setFocused(true);
                    });
                    setFocused(false);
                }
            }
            else if (cur == Settings::getInstance()->getKey_cancel() ||
                     cur == Settings::getInstance()->getKey_cancel2())
            {
                if (m_EditorMode == EditorModes::RemoveUnits)
                {
                    m_EditorMode = EditorModes::PlaceEditorSelection;
                }
                else
                {
                    m_EditorMode = EditorModes::RemoveUnits;
                }
            }
            else if (cur == Settings::getInstance()->getKey_confirm() ||
                     cur == Settings::getInstance()->getKey_confirm2())
            {
                m_placingState.active = true;
            }
            else if (cur == Settings::getInstance()->getKey_toggleGridLayout())
            {
                toggleGridLayout();
            }
            else
            {
                m_EditorSelection->KeyInput(cur);
            }
        }
    }
    BaseGamemenu::keyInput(event);
}

void EditorMenue::cursorMoved(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::cursorMoved x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
    m_Topbar->hide();
    if (m_xyTextInfo.get() != nullptr)
    {
        m_xyTextInfo->setHtmlText("X: " + QString::number(x) + " Y: " + QString::number(y));
    }
    if (m_placingState.active)
    {
        onMapClickedLeft(x, y);
    }
    m_copyRectActor->detach();
    if (m_pMap->onMap(x, y))
    {
        m_pMap->addChild(m_cursorActor);
        m_cursorActor->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
    }

    switch (m_EditorMode)
    {
        case EditorModes::RemoveUnits:
        {
            m_Cursor->changeCursor("cursor+delete");
            break;
        }
        case EditorModes::EditUnits:
        {
            m_Cursor->changeCursor("cursor+edit", 0, 0, 1.0f);
            break;
        }
        case EditorModes::EditTerrain:
        {
            m_Cursor->changeCursor("cursor+edit", 0, 0, 1.0f);
            break;
        }
        case EditorModes::PlaceEditorSelection:
        {
            // resolve cursor move
            switch (m_EditorSelection->getCurrentMode())
            {
                case EditorSelection::EditorMode::Terrain:
                {
                    if (canTerrainBePlaced(x, y))
                    {
                        if (m_EditorSelection->getSizeMode() == EditorSelection::PlacementSize::Fill)
                        {
                            m_Cursor->changeCursor("cursor+fill");
                        }
                        else
                        {
                            m_Cursor->changeCursor("cursor+default");
                        }
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
                case EditorSelection::EditorMode::Building:
                {
                    spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
                    if (canBuildingBePlaced(x, y) ||
                        (pCurrentBuilding->getBuildingWidth() == 1 &&
                         pCurrentBuilding->getBuildingHeigth() == 1))
                    {
                        if (m_EditorSelection->getSizeMode() == EditorSelection::PlacementSize::Fill)
                        {
                            m_Cursor->changeCursor("cursor+fill");
                        }
                        else
                        {
                            m_Cursor->changeCursor("cursor+default");
                        }
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
                case EditorSelection::EditorMode::Unit:
                {
                    if (canUnitBePlaced(x, y))
                    {
                        if (m_EditorSelection->getSizeMode() == EditorSelection::PlacementSize::Fill)
                        {
                            m_Cursor->changeCursor("cursor+fill");
                        }
                        else
                        {
                            m_Cursor->changeCursor("cursor+default");
                        }
                    }
                    else
                    {
                        m_Cursor->changeCursor("cursor+unable");
                    }
                    break;
                }
                case EditorSelection::EditorMode::All:
                {
                    break;
                }
            }
            break;
        }
        case EditorModes::CopySelection:
        {
            m_Cursor->changeCursor("cursor+default");
            m_pMap->addChild(m_copyRectActor);
            if (m_copyRect.x() >= 0 && m_copyRect.y() >= 0 &&
                m_copyRect.width() == 0 && m_copyRect.height() == 0)
            {
                createMarkedArea(m_copyRectActor, m_copyRect.topLeft(), QPoint(x, y), CursorModes::Rect, QColor(255, 0, 0));
            }
            else if (m_copyRect.x() < 0 || m_copyRect.y() < 0)
            {
                m_copyRectActor->removeChildren();
            }
            break;
        }
    }
    if (!m_placingState.active &&
        (m_placingState.x != x || m_placingState.y != y))
    {
        m_placingState.x = -1;
        m_placingState.y = -1;
    }
}

void EditorMenue::onMapClickedRight(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::onMapClickedRight x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
    // resolve click    
    switch (m_EditorMode)
    {
        case EditorModes::CopySelection:
        case EditorModes::EditUnits:
        case EditorModes::EditTerrain:
        case EditorModes::RemoveUnits:
        {
            m_copyRectActor->detach();
            m_EditorMode = EditorModes::PlaceEditorSelection;
            selectionChanged();
            break;
        }
        case EditorModes::PlaceEditorSelection:
        {
            switch (m_EditorSelection->getCurrentMode())
            {
                case EditorSelection::EditorMode::All:
                case EditorSelection::EditorMode::Unit:
                case EditorSelection::EditorMode::Terrain:
                case EditorSelection::EditorMode::Building:
                {
                    Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
                    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                    if (pUnit != nullptr)
                    {
                        m_EditorSelection->selectUnit(pUnit->getUnitID());
                    }
                    else if (pBuilding != nullptr)
                    {
                        m_EditorSelection->selectBuilding(pBuilding->getBuildingID());
                    }
                    else
                    {
                        QString terrainID = m_pMap->getTerrain(x, y)->getTerrainID();
                        m_EditorSelection->selectTerrain(terrainID);
                    }
                    selectionChanged();
                    break;
                }
            }
        }
    }
    m_EditorMode = EditorModes::PlaceEditorSelection;
    
}

void EditorMenue::onMapClickedLeftDown(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::onMapClickedLeftDown x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
    // resolve click
    m_placingState.active = true;
    switch (m_EditorMode)
    {
        case EditorModes::CopySelection:
        {
            if (m_copyRect.x() < 0)
            {
                pasteSelection(x, y, true, m_EditorSelection->getCurrentMode());
                m_pMap->addChild(m_copyRectActor);
                if (m_copyRect.width() == 0)
                {
                    createMarkedArea(m_copyRectActor, m_copyRect.topLeft(), QPoint(x, y), CursorModes::Rect, QColor(255, 0, 0));
                }
                else
                {
                    createMarkedArea(m_copyRectActor, m_copyRect.topLeft(), m_copyRect.bottomRight(), CursorModes::Rect, QColor(255, 0, 0));
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    
}

void EditorMenue::onMapClickedLeftUp(qint32 x, qint32 y)
{
    m_placingState.active = false;
    CONSOLE_PRINT("EditorMenue::onMapClickedLeftUp x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
    // resolve click
    switch (m_EditorMode)
    {
        case EditorModes::CopySelection:
        {
            if (m_copyRect.x() >= 0)
            {
                pasteSelection(x, y, true, m_EditorSelection->getCurrentMode());
                m_pMap->addChild(m_copyRectActor);
                if (m_copyRect.width() == 0)
                {
                    createMarkedArea(m_copyRectActor, m_copyRect.topLeft(), QPoint(x, y), CursorModes::Rect, QColor(255, 0, 0));
                }
                else
                {
                    createMarkedArea(m_copyRectActor, m_copyRect.topLeft(), m_copyRect.bottomRight(), CursorModes::Rect, QColor(255, 0, 0));
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    
}

void EditorMenue::onMapClickedLeft(qint32 x, qint32 y)
{
    if (m_placingState.x != x || m_placingState.y != y)
    {
        Mainapp::getInstance()->pauseRendering();
        CONSOLE_PRINT("EditorMenue::onMapClickedLeft x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
        // resolve click
        switch (m_EditorMode)
        {
            case EditorModes::RemoveUnits:
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr)
                {
                    createTempFile();
                    pUnit->killUnit();
                }
                break;
            }
            case EditorModes::EditUnits:
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr)
                {
                    createTempFile();
                    spDialogModifyUnit pDialog = MemoryManagement::create<DialogModifyUnit>(m_pMap.get(), pUnit);
                    addChild(pDialog);
                    connect(pDialog.get(), &DialogModifyUnit::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                    setFocused(false);
                }
                break;
            }
            case EditorModes::EditTerrain:
            {
                Terrain* pTerrain  = m_pMap->getTerrain(x, y);
                if (pTerrain->getBuilding() == nullptr)
                {
                    createTempFile();
                    spDialogModifyTerrain pDialog = MemoryManagement::create<DialogModifyTerrain>(m_pMap.get(), pTerrain);
                    addChild(pDialog);
                    connect(pDialog.get(), &DialogModifyTerrain::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                    setFocused(false);
                }
                else
                {
                    createTempFile();
                    spDialogModifyBuilding pDialog = MemoryManagement::create<DialogModifyBuilding>(m_pMap.get(), pTerrain->getBuilding());
                    addChild(pDialog);
                    connect(pDialog.get(), &DialogModifyBuilding::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                    setFocused(false);
                }
                break;
            }
            case EditorModes::PlaceEditorSelection:
            {
                switch (m_EditorSelection->getCurrentMode())
                {
                    case EditorSelection::EditorMode::Terrain:
                    {
                        createTempFile();
                        placeTerrain(x, y);
                        break;
                    }
                    case EditorSelection::EditorMode::Building:
                    {
                        createTempFile();
                        placeBuilding(x, y);
                        break;
                    }
                    case EditorSelection::EditorMode::Unit:
                    {
                        createTempFile();
                        placeUnit(x, y);
                        break;
                    }
                    case EditorSelection::EditorMode::All:
                    {
                        break;
                    }
                }
                break;
            }
            case EditorModes::CopySelection:
            {
                break;
            }
        }
        Mainapp::getInstance()->continueRendering();
    }
    m_placingState.x = x;
    m_placingState.y = y;
}

void EditorMenue::editFinishedCanceled()
{
    setFocused(true);
}

void EditorMenue::scriptFinished()
{
    CONSOLE_PRINT("EditorMenue::scriptFinished", GameConsole::eDEBUG);
    setFocused(true);
}

void EditorMenue::campaignFinished()
{
    CONSOLE_PRINT("EditorMenue::campaignFinished", GameConsole::eDEBUG);
    setFocused(true);
}

bool EditorMenue::canTerrainBePlaced(qint32 x, qint32 y)
{
    bool ret = false;
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    
    if (m_pMap->onMap(x, y))
    {
        if (m_pMap->canBePlaced(terrainID, x, y))
        {
            Terrain* pTerrain = m_pMap->getTerrain(x, y);
            if (pTerrain->getTerrainID() != terrainID ||
                pTerrain->getPalette() != Terrain::getPaletteId(m_EditorSelection->getActivePalette(), pTerrain->getTerrainID()) ||
                pTerrain->getBuilding() != nullptr ||
                m_EditorSelection->getSizeMode() == EditorSelection::PlacementSize::Small)
            {
                ret = true;
            }
        }
    }
    return ret;
}

bool EditorMenue::canBuildingBePlaced(qint32 x, qint32 y)
{
    bool ret = false;
    
    if (m_pMap->onMap(x, y))
    {
        spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
        ret = pCurrentBuilding->canBuildingBePlaced(m_pMap->getTerrain(x, y));
    }
    return ret;
}

bool EditorMenue::canUnitBePlaced(qint32 x, qint32 y)
{
    bool ret = false;
    
    if (m_pMap->onMap(x, y))
    {
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = m_EditorSelection->getCurrentSpUnit()->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, m_pMap->getTerrain(x, y), m_pMap->getTerrain(x, y), m_EditorSelection->getCurrentSpUnit().get()) > 0)
        {
            ret = true;
        }
    }
    return ret;
}

void EditorMenue::getSquareTiles(QVector<QPoint> & points, QPoint start, QPoint end, QPoint currentPos)
{
    for (qint32 x = start.x(); x <= end.x(); ++x)
    {
        for (qint32 y = start.y(); y <= end.y(); ++y)
        {
            points.append(QPoint(x, y) + currentPos);
        }
    }
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::placeTerrain", GameConsole::eDEBUG);
    QVector<QPoint> points;
    Mainapp* pApp = Mainapp::getInstance();
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::None:
        {
            return;
        }
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::BigSquare:
        {
            getSquareTiles(points, QPoint(-1, -1), QPoint(1, 1), QPoint(x, y));
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(m_pMap.get(), m_pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
    pApp->pauseRendering();
    bool placed = false;
    const QString palette = Terrain::getPaletteId(m_EditorSelection->getActivePalette(), terrainID);
    QVector<QPoint> placedPoints;
    for (auto point : points)
    {
        if (m_pMap->onMap(point.x(), point.y()))
        {
            // nice we can place the terrain
            if (canTerrainBePlaced(point.x(), point.y()) &&
                       terrainID != m_pMap->getTerrain(point.x(), point.y())->getID())
            {
                placedPoints.append(point);
                spUnit pUnit;
                m_pMap->getTerrain(point.x(), point.y())->setUnit(pUnit);
                Interpreter* pInterpreter = Interpreter::getInstance();
                QString function1 = "useTerrainAsBaseTerrain";
                QJSValue useTerrainAsBaseTerrain = pInterpreter->doFunction(terrainID, function1);
                m_pMap->replaceTerrain(terrainID, point.x(), point.y(), useTerrainAsBaseTerrain.toBool(), false, true, palette, true, false);
                placed = true;
            }
        }
    }
    if (placed)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue sound = pInterpreter->doFunction(terrainID, "getEditorPlacementSound");
        if (sound.isString())
        {
            QString soundName = sound.toString();
            if (!soundName.isEmpty())
            {
                pApp->getAudioManager()->playSound(soundName);
            }
        }
    }
    else
    {
        pApp->getAudioManager()->playSound("impossible.wav");
    }
    if (placedPoints.size() > 30)
    {
        m_pMap->updateSprites(-1, -1, true);
    }
    else
    {
        m_pMap->updateSpritesOfTiles(placedPoints, true);
    }
    pApp->continueRendering();
}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::placeBuilding", GameConsole::eDEBUG);    
    Mainapp* pApp = Mainapp::getInstance();
    spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::None:
        {
            return;
        }
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::BigSquare:
        {
            getSquareTiles(points, QPoint(-1, -1), QPoint(1, 1), QPoint(x, y));
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(m_pMap.get(), m_pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
    pApp->pauseRendering();
    QVector<QPoint> placedPoints;
    if (pCurrentBuilding->getBuildingWidth() > 1 ||
        pCurrentBuilding->getBuildingHeigth() > 1)
    {
        points = PathFindingSystem::getFields(x, y, 0, 0);
    }
    bool placed = false;
    for (qint32 i = 0; i < points.size(); i++)
    {
        // point still on the map great :)
        QPoint pos = points.at(i);
        if (!canBuildingBePlaced(pos.x(), pos.y()) &&
            pCurrentBuilding->getBuildingWidth() == 1 &&
            pCurrentBuilding->getBuildingHeigth() == 1)
        {
            QString baseTerrain = pCurrentBuilding->getBaseTerrain()[0];
            m_pMap->replaceTerrain(baseTerrain, pos.x(), pos.y(), false, false);
        }
        if (canBuildingBePlaced(pos.x(), pos.y()))
        {
            if (pCurrentBuilding->getBuildingID() != m_pMap->getTerrain(pos.x(), pos.y())->getTerrainID())
            {
                placedPoints.append(pos);
                spBuilding pBuilding = MemoryManagement::create<Building>(pCurrentBuilding->getBuildingID(), m_pMap.get());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                m_pMap->getTerrain(pos.x(), pos.y())->setBuilding(pBuilding);
                placed = true;
            }
        }
    }
    if (placed)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue sound = pInterpreter->doFunction(pCurrentBuilding->getBuildingID(), "getEditorPlacementSound");
        if (sound.isString() )
        {
            QString soundName = sound.toString();
            if (!soundName.isEmpty())
            {
                pApp->getAudioManager()->playSound(soundName);
            }
        }
    }
    else
    {
        pApp->getAudioManager()->playSound("impossible.wav");
    }
    if (placedPoints.size() > 30)
    {
        m_pMap->updateSprites(-1, -1, true);
    }
    else
    {
        m_pMap->updateSpritesOfTiles(placedPoints, true);
    }
    if (Settings::getInstance()->getSyncAnimations())
    {
        m_pMap->syncUnitsAndBuildingAnimations();
    }
    pApp->continueRendering();
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::placeUnit", GameConsole::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    spUnit pCurrentUnit = m_EditorSelection->getCurrentSpUnit();
    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
        case EditorSelection::PlacementSize::None:
        {
            return;
        }
        case EditorSelection::PlacementSize::Small:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
        case EditorSelection::PlacementSize::Medium:
        {
            points = PathFindingSystem::getFields(x, y, 0, 1);
            break;
        }
        case EditorSelection::PlacementSize::BigSquare:
        {
            getSquareTiles(points, QPoint(-1, -1), QPoint(1, 1), QPoint(x, y));
            break;
        }
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            points = PathFindingSystem::getFields(x, y, 0, 0);
            break;
        }
    }
    bool placed = false;
    for (auto & point : points)
    {
        // point still on the map great :)
        qint32 curX = point.x();
        qint32 curY = point.y();
        if (canUnitBePlaced(curX, curY))
        {
            spUnit pUnit = MemoryManagement::create<Unit>(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, m_pMap.get());
            pUnit->setAiMode(GameEnums::GameAi::GameAi_Normal);
            m_pMap->getTerrain(curX, curY)->setUnit(pUnit);
            placed = true;
        }
    }
    if (placed)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue sound = pInterpreter->doFunction(pCurrentUnit->getUnitID(), "getEditorPlacementSound");
        if (sound.isString() )
        {
            QString soundName = sound.toString();
            if (!soundName.isEmpty())
            {
                pApp->getAudioManager()->playSound(soundName, 1, 0, 1.0f, false, 250);
            }
        }
    }
    else
    {
        pApp->getAudioManager()->playSound("impossible.wav");
    }
    if (Settings::getInstance()->getSyncAnimations())
    {
        m_pMap->syncUnitsAndBuildingAnimations();
    }
}

void EditorMenue::saveMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::saveMap " + filename, GameConsole::eDEBUG);
    if (filename.startsWith(oxygine::Resource::RCC_PREFIX_PATH))
    {
        filename.replace(oxygine::Resource::RCC_PREFIX_PATH, Settings::userPath());
    }
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        
        m_pMap->serializeObject(stream);
        file.close();
    }
    setFocused(true);    
}

void EditorMenue::loadMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::loadMap " + filename, GameConsole::eDEBUG);
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            m_pMap->deserializeObject(stream);
            file.close();
            
            Mainapp::getInstance()->pauseRendering();
            m_pMap->updateSprites(-1, -1, true);
            m_pMap->centerMap(m_pMap->getMapWidth() / 2, m_pMap->getMapHeight() / 2);
            m_EditorSelection->createPlayerSelection();
            Mainapp::getInstance()->continueRendering();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::importAW4Aw4Map(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importAW4Aw4Map " + filename, GameConsole::eDEBUG);
    if (filename.endsWith(".aw4"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importAW4Map(filename, this);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::importAWByWeb(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importAWByWeb " + filename, GameConsole::eDEBUG);
    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importAWByWebMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::importAWDSAwsMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importAWDSAwsMap " + filename, GameConsole::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importAWDSMap(filename, this);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::exportAWDSAwsMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::exportAWDSAwsMap", GameConsole::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        m_pMap->exportAWDSMap(filename);
    }
    setFocused(true);
}

void EditorMenue::importCoWTxTMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importCoWTxTMap", GameConsole::eDEBUG);
    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importTxtMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::newMap(MapEditDialog::MapEditInfo info)
{
    CONSOLE_PRINT("EditorMenue::newMap", GameConsole::eDEBUG);
    cleanTemp(-1);

    m_pMap->newMap(info.mapWidth, info.mapHeigth, info.playerCount);
    m_pMap->setMapName(info.mapName);
    m_pMap->setMapAuthor(info.author);
    m_pMap->setMapDescription(info.description);
    m_pMap->getGameScript()->setScriptFile(info.scriptFile);
    m_pMap->setMapFlags(info.mapFlags);
    m_pMap->getGameRecorder()->setDeployLimit(info.deployLimit);
    m_pMap->getGameRecorder()->setMapTime(info.turnLimit);
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    updateGrids();
}

void EditorMenue::changeMap(MapEditDialog::MapEditInfo info)
{    
    CONSOLE_PRINT("EditorMenue::changeMap", GameConsole::eDEBUG);
    createTempFile();
    
    m_pMap->setMapName(info.mapName);
    m_pMap->setMapAuthor(info.author);
    m_pMap->setMapDescription(info.description);
    m_pMap->getGameScript()->setScriptFile(info.scriptFile);
    m_pMap->changeMap(info.mapWidth, info.mapHeigth, info.playerCount);
    m_pMap->getGameRecorder()->setDeployLimit(info.deployLimit);
    m_pMap->getGameRecorder()->setMapTime(info.turnLimit);
    m_pMap->setMapFlags(info.mapFlags);
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    updateGrids();
}

void EditorMenue::selectionChanged()
{
    CONSOLE_PRINT("EditorMenue::selectionChanged", GameConsole::eDEBUG);
    if (m_EditorMode == EditorModes::PlaceEditorSelection)
    {
        m_mapSliding->setLocked(true);
        if (m_EditorSelection->getCurrentMode() == EditorSelection::EditorMode::Building)
        {
            spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
            if (pCurrentBuilding->getBuildingWidth() > 1 ||
                pCurrentBuilding->getBuildingHeigth() > 1)
            {
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(-pCurrentBuilding->getBuildingWidth() + 1, -pCurrentBuilding->getBuildingHeigth() + 1), CursorModes::Rect);
                return;
            }
        }
        switch (m_EditorSelection->getSizeMode())
        {
            case EditorSelection::PlacementSize::None:
            {
                m_mapSliding->setLocked(false);
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(-1, -1), CursorModes::Circle);
                break;
            }
            case EditorSelection::PlacementSize::Medium:
            {
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(1, -1), CursorModes::Circle);
                break;
            }
            case EditorSelection::PlacementSize::BigSquare:
            {
                createMarkedArea(m_cursorActor, QPoint(-1, -1), QPoint(1, 1), CursorModes::Rect);
                break;
            }
            case EditorSelection::PlacementSize::Big:
            {
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(2, -1), CursorModes::Circle);
                break;
            }
            case EditorSelection::PlacementSize::Small:
            case EditorSelection::PlacementSize::Fill:
            {
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(-1, -1), CursorModes::Circle);
                break;
            }
        }
    }
    else if (m_EditorMode == EditorModes::CopySelection)
    {
        m_mapSliding->setLocked(false);
        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(m_copyRect.width() - 1, m_copyRect.height() - 1), CursorModes::Rect);
    }
}

void EditorMenue::createMarkedArea(oxygine::spActor pActor, QPoint p1, QPoint p2, CursorModes mode, QColor color)
{
    CONSOLE_PRINT("EditorMenue::createMarkedArea", GameConsole::eDEBUG);
    pActor->removeChildren();
    switch (mode)
    {
        case CursorModes::Rect:
        {
            ObjectManager* pObjectManager = ObjectManager::getInstance();
            qint32 xDir = 1;
            if (p1.x() != p2.x())
            {
                xDir = (p2.x() - p1.x()) / qAbs(p1.x() - p2.x());
            }
            qint32 yDir = 1;
            if (p1.y() != p2.y())
            {
                yDir = (p2.y() - p1.y()) / qAbs(p1.y() - p2.y());
            }
            for (qint32 i = p1.x(); i != p2.x() + xDir; i += xDir)
            {
                oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                qint32 y = p1.y();
                if (p2.y() < p1.y())
                {
                    y = p2.y();
                }
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+top");
                pSprite->setResAnim(pAnim);
                pSprite->setColor(color);
                pSprite->setPosition(GameMap::getImageSize() * i,
                                     GameMap::getImageSize() * y);
                pActor->addChild(pSprite);
                y = p2.y();
                if (p2.y() < p1.y())
                {
                    y = p1.y();
                }
                pSprite = MemoryManagement::create<oxygine::Sprite>();
                pAnim = pObjectManager->getResAnim("cursor+border+bottom");
                pSprite->setResAnim(pAnim);
                pSprite->setColor(color);
                pSprite->setPosition(GameMap::getImageSize() * i,
                                     GameMap::getImageSize() * y);
                pActor->addChild(pSprite);
            }
            for (qint32 i = p1.y(); i != p2.y() + yDir; i += yDir)
            {
                oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                qint32 x = p1.x();
                if (p2.x() < p1.x())
                {
                    x = p2.x();
                }
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+left");
                pSprite->setResAnim(pAnim);
                pSprite->setColor(color);
                pSprite->setPosition(GameMap::getImageSize() * x,
                                     GameMap::getImageSize() * i);
                pActor->addChild(pSprite);
                x = p2.x();
                if (p2.x() < p1.x())
                {
                    x = p1.x();
                }
                pSprite = MemoryManagement::create<oxygine::Sprite>();
                pAnim = pObjectManager->getResAnim("cursor+border+right");
                pSprite->setResAnim(pAnim);
                pSprite->setColor(color);
                pSprite->setPosition(GameMap::getImageSize() * x,
                                     GameMap::getImageSize() * i);
                pActor->addChild(pSprite);
            }
            break;
        }
        case CursorModes::Circle:
        {
            if (p2.x() > 0)
            {
                spQmlVectorPoint pPoints = GlobalUtils::getSpCircle(p2.x(), p2.x());
                ObjectManager* pObjectManager = ObjectManager::getInstance();
                for (qint32 i = 0; i < pPoints->size(); i++)
                {
                    QPoint point = pPoints->at(i);
                    if (point.x() >= 0)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+right");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.x() <= 0)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+left");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.y() >= 0)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+bottom");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.y() <= 0)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+top");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                }
            }
            break;
        }
    }
}

void EditorMenue::pasteSelection(qint32 x, qint32 y, bool click, EditorSelection::EditorMode selection)
{
    CONSOLE_PRINT("EditorMenue::pasteSelection", GameConsole::eDEBUG);
    
    if (m_pMap->onMap(x, y))
    {
        if (m_copyRect.x() < 0)
        {
            m_copyRect.setX(x);
            m_copyRect.setY(y);
            m_copyRect.setWidth(0);
            m_copyRect.setHeight(0);
        }
        else if (m_copyRect.width() == 0)
        {
            if (m_copyRect.x() < x)
            {
                m_copyRect.setRight(x);
            }
            else
            {
                qint32 copy = m_copyRect.x();
                m_copyRect.setX(x);
                m_copyRect.setRight(copy);
            }
            if (m_copyRect.y() < y)
            {
                m_copyRect.setBottom(y);
            }
            else
            {
                qint32 copy = m_copyRect.y();
                m_copyRect.setY(y);
                m_copyRect.setBottom(copy);
            }
            createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(m_copyRect.width() - 1, m_copyRect.height() - 1), CursorModes::Rect);
        }
        else if (!click)
        {
            QRect rect(x, y, m_copyRect.width(), m_copyRect.height());
            if (m_pMap->onMap(rect.x(), rect.y()) &&
                m_pMap->onMap(rect.right(), rect.bottom()) &&
                !m_copyRect.intersects(QRect(x, y, m_copyRect.width(), m_copyRect.height())))
            {
                createTempFile();
                qint32 xDir = m_copyRect.width() / qAbs(m_copyRect.width());
                qint32 yDir = m_copyRect.height() / qAbs(m_copyRect.height());


                for (qint32 xPos = 0; xPos != m_copyRect.width(); xPos += xDir)
                {
                    for (qint32 yPos = 0; yPos != m_copyRect.height(); yPos += yDir)
                    {
                        switch (selection)
                        {
                            case EditorSelection::EditorMode::All:
                            case EditorSelection::EditorMode::Terrain:
                            {
                                Terrain* pCopyTerrain = m_pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos);
                                m_pMap->replaceTerrain(pCopyTerrain->getBaseTerrainIDOfLevel(1), x + xPos, y + yPos, false, false);
                                m_pMap->replaceTerrain(pCopyTerrain->getBaseTerrainIDOfLevel(0), x + xPos, y + yPos, true, false);
                                Terrain* pTerrain = m_pMap->getTerrain(x + xPos, y + yPos);
                                QString id = pCopyTerrain->getTerrainSpriteName();
                                pTerrain->setFixedSprite(pCopyTerrain->getFixedSprite());
                                pTerrain->setTerrainSpriteName(id);
                                pTerrain->setFixedOverlaySprites(pCopyTerrain->getFixedOverlaySprites());
                                pTerrain->setCustomOverlays(pCopyTerrain->getCustomOverlays());
                                pTerrain->setHp(pCopyTerrain->getHp());
                                if (selection != EditorSelection::EditorMode::All)
                                {
                                    break;
                                }
                            }
                            case EditorSelection::EditorMode::Building:
                            {
                                Building* pBuilding = m_pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos)->getBuilding();
                                if (pBuilding != nullptr &&
                                    pBuilding->getBuildingWidth() == 1 &&
                                    pBuilding->getBuildingHeigth() == 1)
                                {
                                    if (pBuilding->canBuildingBePlaced(m_pMap->getTerrain(x + xPos, y + yPos)))
                                    {
                                        auto baseTerrains = pBuilding->getBaseTerrain();
                                        QString baseTerrain = baseTerrains[0];
                                        m_pMap->replaceTerrain(baseTerrain, x + xPos, y + yPos, false, false);
                                    }
                                    spBuilding pCopyBuilding = MemoryManagement::create<Building>(pBuilding->getBuildingID(), m_pMap.get());
                                    pCopyBuilding->setOwner(pBuilding->getOwner());
                                    pCopyBuilding->setHp(pBuilding->getHp());
                                    m_pMap->getTerrain(x + xPos, y + yPos)->setBuilding(pCopyBuilding);
                                }
                                if (selection != EditorSelection::EditorMode::All)
                                {
                                    break;
                                }
                            }
                            case EditorSelection::EditorMode::Unit:
                            {
                                Unit* pUnit = m_pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos)->getUnit();
                                if (pUnit != nullptr)
                                {
                                    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
                                    QString movementType = pUnit->getMovementType();
                                    if (pMovementTableManager->getBaseMovementPoints(movementType, m_pMap->getTerrain(x + xPos, y + yPos), m_pMap->getTerrain(x + xPos, y + yPos), pUnit) > 0)
                                    {
                                        spUnit pCopyUnit = MemoryManagement::create<Unit>(pUnit->getUnitID(), pUnit->getOwner(), false, m_pMap.get());
                                        m_pMap->getTerrain(x + xPos, y + yPos)->setUnit(spUnit());
                                        m_pMap->getTerrain(x + xPos, y + yPos)->setUnit(pCopyUnit);
                                        pCopyUnit->setHp(pUnit->getHp());
                                        pCopyUnit->setAmmo1(pUnit->getAmmo1());
                                        pCopyUnit->setAmmo2(pUnit->getAmmo2());
                                        pCopyUnit->setFuel(pUnit->getFuel());
                                        pCopyUnit->setAiMode(pUnit->getAiMode());
                                        pCopyUnit->setUnitRank(pUnit->getUnitRank());
                                        pCopyUnit->setModdingFlags(pUnit->getModdingFlags());
                                    }
                                }
                                if (selection != EditorSelection::EditorMode::All)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                m_pMap->updateSprites(-1, -1, true);
            }
        }
    }
}

void EditorMenue::exitEditor()
{    
    CONSOLE_PRINT("Leaving Editor Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = MemoryManagement::create<Mainwindow>("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void EditorMenue::autosave()
{
    CONSOLE_PRINT("EditorMenue::autosave", GameConsole::eDEBUG);
    QString filename = Settings::userPath() + "maps/autosave.map";
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        
        m_pMap->serializeObject(stream);
        file.close();
    }    
}
