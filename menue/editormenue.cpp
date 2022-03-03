#include <QFile>
#include <QDir>
#include <QCryptographicHash>

#include "menue/editormenue.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"
#include "coreengine/pathfindingsystem.h"
#include "coreengine/console.h"

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
    : InGameMenue (20, 20, "", false),
      m_autosaveTimer(this)
{
    setObjectName("EditorMenue");
    Mainapp* pApp = Mainapp::getInstance();
    qint32 selectionWidth = Settings::getWidth() / 4;
    bool smallScreen = Settings::getSmallScreenDevice();
    Interpreter::setCppOwnerShip(this);
    if (smallScreen)
    {
        selectionWidth = Settings::getWidth() * 3 / 4;
    }
    if (selectionWidth < 255)
    {
        selectionWidth = 255;
    }
    if (smallScreen)
    {
        m_autoScrollBorder = QRect(50, 135, 50, 50);
        initSlidingActor(50, 125, Settings::getWidth() - 100, Settings::getHeight() - 175);
    }
    else
    {
        m_autoScrollBorder = QRect(50, 135, selectionWidth, 50);
        initSlidingActor(50, 125, Settings::getWidth() - selectionWidth - 100, Settings::getHeight() - 175);
    }
    m_mapSlidingActor->addChild(m_pMap);
    m_mapSliding->setLocked(true);
    loadHandling();
    changeBackground("editormenu");
    moveToThread(pApp->getWorkerthread());

    m_EditorSelection = spEditorSelection::create(selectionWidth, smallScreen, m_pMap.get());
    addChild(m_EditorSelection);

    m_Topbar = spTopbar::create(0, Settings::getWidth());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapeditor");
    pApp->getAudioThread()->playRandom();

    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addItem(tr("Save Map"), "SAVEMAP", 0, tr("Saves a map to a give file."));
    m_Topbar->addItem(tr("Load Map"), "LOADMAP", 0, tr("Loads a map to a give file."));
    if (!Settings::getSmallScreenDevice())
    {
        m_Topbar->addItem(tr("Edit Script"), "EDITSCRIPT", 0, tr("Edit and create a script for any map."));
        m_Topbar->addItem(tr("Edit Campaign"), "EDITCAMPAIGN", 0, tr("Edit and create a campaign."));
    }
    m_Topbar->addItem(tr("Undo Ctrl+Z"), "UNDO", 0, tr("Undo the last map modification."));
    m_Topbar->addItem(tr("Redo Ctrl+Y"), "REDO", 0, tr("Redo the last undo command."));
    m_Topbar->addItem(tr("Exit Editor"), "EXIT", 0, tr("Exits the editor"));

    m_Topbar->addGroup(tr("Map Info"));
    m_Topbar->addItem(tr("New Map"), "NEWMAP", 1, tr("Create a new map"));
    m_Topbar->addItem(tr("Edit Map"), "EDITMAP", 1, tr("Edit the information for a map"));
    m_Topbar->addItem(tr("Resize Map"), "RESIZEMAP", 1, tr("Resizes the map using left, top, right and bottom size changes."));
    m_Topbar->addItem(tr("Flip Map X"), "FLIPX", 1, tr("Flips the map at the x-axis. Flipping the left half of the map. The right half of the map is changed."));
    m_Topbar->addItem(tr("Flip Map Y"), "FLIPY", 1, tr("Flips the map at the y-axis. Flipping the top half of the map. The bottom half of the map is changed."));
    m_Topbar->addItem(tr("Rotate Map X"), "ROTATEX", 1, tr("Flips and rotates the map at the x-axis. Using the left half of the map. The right half of the map is changed."));
    m_Topbar->addItem(tr("Rotate Map Y"), "ROTATEY", 1, tr("Flips and rotates the map at the y-axis. Using the top half of the map. The bottom half of the map is changed."));
    m_Topbar->addItem(tr("Random Map"), "RANDOMMAP", 1, tr("Creates a new random map."));
    m_Topbar->addItem(tr("Toggle Grid Strg+G"), "TOGGLEGRID", 1, tr("Shows or hides a grid layout."));
    m_Topbar->addItem(tr("Toggle Cross Strg+M"), "TOGGLEMIDDLECROSS", 1, tr("Shows or hides the cross marking the middle of the map."));

    m_Topbar->addGroup(tr("Commands"));
    m_Topbar->addItem(tr("Place Selection"), "PLACESELECTION", 2, tr("Selects the editor mode placing the current tile"));
    m_Topbar->addItem(tr("Delete Units") + " - " + SelectKey::getKeycodeText(Settings::getKey_cancel()), "DELETEUNITS", 2, tr("Selects the editor mode deleting units"));
    m_Topbar->addItem(tr("Edit Units"), "EDITUNITS", 2, tr("Selects the editor mode modifying the stats of a unit"));
    m_Topbar->addItem(tr("Edit Terrain"), "EDITTERRAIN", 2, tr("Selects the editor mode editing the style of a terrain or building"));
    m_Topbar->addItem(tr("Edit Players"), "EDITPLAYERS", 2, tr("Edit the CO's and player start setup."));
    m_Topbar->addItem(tr("Edit Rules"), "EDITRULES", 2, tr("Selects the editor rules for the map."));
    m_Topbar->addItem(tr("Optimize Players"), "OPTIMIZEPLAYERS", 2, tr("Removes all players with no units or buildings from the map"));
    if (!Settings::getSmallScreenDevice())
    {
        m_Topbar->addItem(tr("Copy Ctrl+C"), "COPY", 2, tr("Enters the copy mode. Hold the left mouse key and mark the fields you want to copy. Copying is based on the current placing mode"));
        m_Topbar->addItem(tr("Paste Ctrl+V"), "PASTE", 2, tr("Paste the current selected area. Based on the current placing mode. The copy and paste selection are not allowed to intersec."));
        m_Topbar->addItem(tr("Paste Ctrl+Shift+V"), "PASTEALL", 2, tr("Paste the current selected area with all terrain, buildings, units. The copy and paste selection are not allowed to intersec."));
    }

    m_Topbar->addGroup(tr("Import/Export"));
    m_Topbar->addItem(tr("Import CoW Txt"), "IMPORTCOWTXT", 3, tr("Deletes the current map and imports an old Commander Wars Map from a file."));
    m_Topbar->addItem(tr("Import AWDS Aws"), "IMPORTAWDSAWS", 3, tr("Deletes the current map and imports an AWS Map Editor from a file."));
    m_Topbar->addItem(tr("Export AWDS Aws"), "EXPORTAWDSAWS", 3, tr("Exports the map to an AWS Map Editor file"));
    m_Topbar->addItem(tr("Import AWDC Aw4"), "IMPORTAWDCAW4", 3, tr("Deletes the current map and imports an AW DoR/DC Map Editor from a file."));
    m_Topbar->addItem(tr("Import AW by Web"), "IMPORTAWBYWEB", 3, tr("Deletes the current map and imports an  Advance Wars by Web Map from https://awbw.amarriner.com/"));
    m_Topbar->finishCreation();

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setResAnim(pAnim);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    if (!Settings::getSmallScreenDevice())
    {
        m_xyTextInfo = spLabel::create(180);
        m_xyTextInfo->setStyle(style);
        m_xyTextInfo->setHtmlText("X: 0 Y: 0");
        m_xyTextInfo->setPosition(8, 8);
        pButtonBox->addChild(m_xyTextInfo);
        pButtonBox->setSize(200, 50);
        pButtonBox->setPosition((Settings::getWidth() - m_EditorSelection->getWidth())  - pButtonBox->getWidth(), -4 + m_Topbar->getHeight());
        pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        addChild(pButtonBox);
    }
    addChild(m_Topbar);

    Cursor* pCursor = m_Cursor.get();
    m_pMap->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
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

    // connecting stuff
    connect(this, &EditorMenue::sigLeftClick, this, &EditorMenue::onMapClickedLeft, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigLeftClickDown, this, &EditorMenue::onMapClickedLeftDown, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigLeftClickUp, this, &EditorMenue::onMapClickedLeftUp, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigRightClick, this, &EditorMenue::onMapClickedRight, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, this, &EditorMenue::cursorMoved, Qt::QueuedConnection);
    connect(m_Topbar.get(), &Topbar::sigItemClicked, this, &EditorMenue::clickedTopbar, Qt::QueuedConnection);
    connect(m_EditorSelection.get(), &EditorSelection::sigSelectionChanged, this, &EditorMenue::selectionChanged, Qt::QueuedConnection);
    connect(this, &EditorMenue::sigResizeMap, this, &EditorMenue::resizeMap, Qt::QueuedConnection);

    // clean up temp folder
    QDir dir("temp/");
    dir.removeRecursively();
    dir.mkpath(".");

    m_autosaveTimer.setSingleShot(false);
    connect(&m_autosaveTimer, &QTimer::timeout, this, &EditorMenue::autosave, Qt::QueuedConnection);
    m_autosaveTimer.start(60 * 1000);

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue obj = pInterpreter->newQObject(this);
    pInterpreter->setGlobal("currentMenu", obj);
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
        CONSOLE_PRINT("Executing:" + object + "." + func, Console::eDEBUG);
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
    QDir dir("temp/");
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
            QFile file("temp/temp" + QString::number(i) + ".tmp");
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
    CONSOLE_PRINT(QString("createTempFile(") + (cleanUp ? "true" : "false") + ")", Console::eDEBUG);
    if (cleanUp)
    {
        cleanTemp(m_tempCounter);
    }
    QFile file("temp/temp" + QString::number(m_tempCounter) + ".tmp");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream stream(&file);
    
    m_pMap->serializeObject(stream);
    file.close();

    QFile previous("temp/temp" + QString::number(m_tempCounter - 1) + ".tmp");
    if (previous.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QCryptographicHash myHash(QCryptographicHash::Sha3_512);
        myHash.addData(&file);
        QByteArray hash = myHash.result();
        file.close();

        previous.open(QIODevice::ReadOnly | QIODevice::Truncate);
        QCryptographicHash myHash1(QCryptographicHash::Sha3_512);
        myHash1.addData(&previous);
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
    CONSOLE_PRINT("EditorMenue::editorUndo", Console::eDEBUG);
    m_tempCounter--;
    if (m_tempCounter >= 0)
    {
        QFile file("temp/temp" + QString::number(m_tempCounter) + ".tmp");
        if (file.exists())
        {
            m_tempCounter++;
            createTempFile(false);
            m_tempCounter -= 2;
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            m_pMap->deserializeObject(stream);
            file.close();
            
            Mainapp::getInstance()->pauseRendering();
            m_pMap->updateSprites();
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
    CONSOLE_PRINT("EditorMenue::editorRedo", Console::eDEBUG);
    m_tempCounter++;
    QFile file("temp/temp" + QString::number(m_tempCounter) + ".tmp");
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QDataStream stream(&file);
        m_pMap->deserializeObject(stream);
        file.close();
        
        Mainapp::getInstance()->pauseRendering();
        m_pMap->updateSprites();
        Mainapp::getInstance()->continueRendering();
        m_EditorSelection->createPlayerSelection();
    }
}

void EditorMenue::clickedTopbar(QString itemID)
{    
    CONSOLE_PRINT("clickedTopbar(" + itemID + ")", Console::eDEBUG);
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
        MenuItem("IMPORTAWDCAW4",       &EditorMenue::showImportAwdsAw4),
        MenuItem("IMPORTAWBYWEB",       &EditorMenue::showImportAwByWeb),
        MenuItem("NEWMAP",              &EditorMenue::showNewMap),
        MenuItem("EDITMAP",             &EditorMenue::showEditMap),
        MenuItem("FLIPX",               &EditorMenue::flipX),
        MenuItem("FLIPY",               &EditorMenue::flipY),
        MenuItem("ROTATEX",             &EditorMenue::rotateX),
        MenuItem("ROTATEY",             &EditorMenue::rotateY),
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
    };
    for (auto & item : qAsConst(items))
    {
        if (item.m_Id == itemID &&
            item.m_func != nullptr)
        {
            (this->*(item.m_func))();
        }
    }
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
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, m_pMap->getMapName());
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::saveMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showLoadMap()
{
    QStringList wildcards;
    wildcards.append("*.map");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::loadMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showResizeMap()
{
    CONSOLE_PRINT("showResizeMap()", Console::eDEBUG);
    
    spGenericBox pBox = spGenericBox::create(true);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    qint32 y = 30;

    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Left: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox leftBox = spSpinBox::create(150, -m_pMap->getMapWidth() + 1, 9999);
    leftBox->setTooltipText(tr("Change of the map size on the left map border."));
    leftBox->setPosition(width, y);
    leftBox->setInfinityValue(-m_pMap->getMapWidth());
    leftBox->setCurrentValue(0);
    pBox->addItem(leftBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Top: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox topBox = spSpinBox::create(150, -m_pMap->getMapHeight() + 1, 999999);
    topBox->setTooltipText(tr("Change of the map size on the top map border."));
    topBox->setPosition(width, y);
    topBox->setInfinityValue(-m_pMap->getMapHeight());
    topBox->setCurrentValue(0);
    pBox->addItem(topBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Right: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox rightBox = spSpinBox::create(150, -m_pMap->getMapWidth() + 1, 999999);
    rightBox->setTooltipText(tr("Change of the map size on the right map border."));
    rightBox->setPosition(width, y);
    rightBox->setInfinityValue(-m_pMap->getMapWidth());
    rightBox->setCurrentValue(0);
    pBox->addItem(rightBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Bottom: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox bottomBox = spSpinBox::create(150, -m_pMap->getMapHeight() + 1, 999999);
    bottomBox->setTooltipText(tr("Change of the map size on the bottom map border."));
    bottomBox->setPosition(width, y);
    bottomBox->setInfinityValue(-m_pMap->getMapHeight());
    bottomBox->setCurrentValue(0);
    pBox->addItem(bottomBox);
    y += 40;

    addChild(pBox);
    auto* pLeftBox = leftBox.get();
    auto* pRightBox = rightBox.get();
    auto* pTopBox = topBox.get();
    auto* pBottomBox = bottomBox.get();

    connect(pBox.get(), &GenericBox::sigFinished, this, [=]
    {
        emit sigResizeMap(pLeftBox->getCurrentValue(), pTopBox->getCurrentValue(),
                          pRightBox->getCurrentValue(), pBottomBox->getCurrentValue());
    });
    
}

void EditorMenue::showEditScript()
{
    spScriptEditor scriptEditor = spScriptEditor::create(m_pMap.get());
    addChild(scriptEditor);
    connect(scriptEditor.get(),  &ScriptEditor::sigFinished, this, &EditorMenue::scriptFinished, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showEditCampaign()
{
    spCampaignEditor campaignEditor = spCampaignEditor::create();
    addChild(campaignEditor);
    connect(campaignEditor.get(),  &CampaignEditor::sigFinished, this, &EditorMenue::campaignFinished, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportCoWTxTMap()
{
    QStringList wildcards;
    wildcards.append("*.txt");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importCoWTxTMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwdsAws()
{
    QStringList wildcards;
    wildcards.append("*.aws");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWDSAwsMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showExportAwdsAws()
{
    QStringList wildcards;
    wildcards.append("*.aws");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::exportAWDSAwsMap, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwdsAw4()
{
    QStringList wildcards;
    wildcards.append("*.aw4");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWDCAw4Map, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showImportAwByWeb()
{
    QStringList wildcards;
    wildcards.append("*.txt");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWByWeb, Qt::QueuedConnection);
    connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showNewMap()
{
    MapEditDialog::MapEditInfo info;
    info.author = Settings::getUsername();
    info.mapWidth = 20;
    info.mapHeigth = 20;
    info.playerCount = 4;
    spMapEditDialog mapEditDialog = spMapEditDialog::create(info);
    connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::newMap, Qt::QueuedConnection);
    connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    addChild(mapEditDialog);
    setFocused(false);
}

void EditorMenue::showEditMap()
{
    spGameMap pGameMap = m_pMap;
    MapEditDialog::MapEditInfo info;
    info.mapName = pGameMap->getMapName();
    info.author = pGameMap->getMapAuthor();
    info.description = pGameMap->getMapDescription();
    info.scriptFile = pGameMap->getGameScript()->getScriptFile();
    info.mapWidth = pGameMap->getMapWidth();
    info.mapHeigth = pGameMap->getMapHeight();
    info.playerCount = pGameMap->getPlayerCount();
    info.turnLimit = pGameMap->getGameRecorder()->getMapTime();
    info.deployLimit = pGameMap->getGameRecorder()->getDeployLimit();
    info.mapFlags = pGameMap->getMapFlags();
    spMapEditDialog mapEditDialog = spMapEditDialog::create(info);
    connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::changeMap, Qt::QueuedConnection);
    connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
    addChild(mapEditDialog);
    setFocused(false);
}

void EditorMenue::flipX()
{
    spGameMap pGameMap = m_pMap;
    pGameMap->flipX();
}

void EditorMenue::flipY()
{
    spGameMap  pGameMap = m_pMap;
    pGameMap->flipY();
}

void EditorMenue::rotateX()
{
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateX();
}

void EditorMenue::rotateY()
{
    spGameMap  pGameMap = m_pMap;
    pGameMap->rotateY();
}

void EditorMenue::showRandomMap()
{
    spDialogRandomMap pDialogRandomMap = spDialogRandomMap::create();
    addChild(pDialogRandomMap);
    connect(pDialogRandomMap.get(), &DialogRandomMap::sigFinished, this, &EditorMenue::createRandomMap, Qt::QueuedConnection);
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
    spPlayerSelectionDialog pDialog = spPlayerSelectionDialog::create(m_pMap.get());
    addChild(pDialog);
    connect(pDialog.get(), &PlayerSelectionDialog::sigPlayersChanged, this, &EditorMenue::playersChanged, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showEditRules()
{
    spRuleSelectionDialog pDialog = spRuleSelectionDialog::create(m_pMap.get(), RuleSelection::Mode::Editor);
    addChild(pDialog);
    connect(pDialog.get(), &RuleSelectionDialog::sigRulesChanged, this, &EditorMenue::rulesChanged, Qt::QueuedConnection);
    setFocused(false);
}

void EditorMenue::showExit()
{
    m_Focused = false;
    spDialogMessageBox pExit = spDialogMessageBox::create(tr("Do you want to exit the map editor?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &EditorMenue::exitEditor, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, this, [=]()
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
    CONSOLE_PRINT("EditorMenue::resizeMap", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::createRandomMap", Console::eDEBUG);
    cleanTemp(-1);
    RandomMapGenerator::randomMap(m_pMap.get(), width, heigth, playerCount, roadSupport, seed,
                        terrains, buildings, ownedBaseSize, startBaseSize / 100.0f,
                        units, unitCount, startBaseUnitSize / 100.0f, unitDistribution, unitsDistributed,
                        mirrored);
    m_pMap->setMapName(mapName);
    m_pMap->setMapAuthor(author);
    m_pMap->setMapDescription(description);
    Mainapp::getInstance()->pauseRendering();
    m_pMap->updateSprites();
    Mainapp::getInstance()->continueRendering();
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    updateGrids();
}

void EditorMenue::playersChanged()
{    
    CONSOLE_PRINT("EditorMenue::playersChanged", Console::eDEBUG);
    m_EditorSelection->createPlayerSelection();
    
    Mainapp::getInstance()->pauseRendering();
    m_pMap->updateSprites();
    Mainapp::getInstance()->continueRendering();
    setFocused(true);
}

void EditorMenue::rulesChanged()
{
    CONSOLE_PRINT("EditorMenue::rulesChanged", Console::eDEBUG);
    setFocused(true);
}

void EditorMenue::optimizePlayers()
{
    CONSOLE_PRINT("EditorMenue::optimizePlayers", Console::eDEBUG);
    createTempFile();
    
    QVector<bool> foundPlayers(m_pMap->getPlayerCount(), false);
    qint32 mapWidth = m_pMap->getMapWidth();
    qint32 mapHeigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
            if (pBuilding != nullptr && pBuilding->getOwner() != nullptr)
            {
                foundPlayers[pBuilding->getOwner()->getPlayerID()] = true;
            }
            if (pUnit != nullptr)
            {
                foundPlayers[pUnit->getOwner()->getPlayerID()] = true;
            }
        }
    }
    for (qint32 i = foundPlayers.size() - 1; i >= 0; i--)
    {
        if (m_pMap->getPlayerCount() > 2)
        {
            if (foundPlayers[i] == false)
            {
                m_pMap->removePlayer(i);
            }
        }
    }
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (m_pMap->getPlayer(i)->getTeam() >= m_pMap->getPlayerCount())
        {
            m_pMap->getPlayer(i)->setTeam(i);
        }
    }
    m_EditorSelection->createPlayerSelection();
    
}

void EditorMenue::keyInput(oxygine::KeyEvent event)
{
    if (!event.getContinousPress())
    {
        Qt::Key cur = event.getKey();
        if (m_Focused)
        {
            if ((event.getModifiers() & Qt::KeyboardModifier::ControlModifier) > 0)
            {
                switch (cur)
                {
                    case Qt::Key_Escape:
                    {
                        break;
                    }
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
                    case Qt::Key_G:
                    {
                        toggleGridLayout();
                        break;
                    }
                    case Qt::Key_M:
                    {
                        toggleMiddleCrossGrid();
                        break;
                    }
                    default:
                    {
                        // do nothing
                        break;
                    }
                }                
            }
            else if (cur == Settings::getKey_information() ||
                     cur == Settings::getKey_information2())
            {
                
                if (m_pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
                {
                    Terrain* pTerrain = m_pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    spFieldInfo fieldinfo = spFieldInfo::create(pTerrain, pTerrain->getUnit());
                    addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, this, [=]
                    {
                        setFocused(true);
                    });
                    setFocused(false);
                }
            }
            else if (cur == Settings::getKey_cancel() ||
                     cur == Settings::getKey_cancel2())
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
        }
    }
    InGameMenue::keyInput(event);
}

void EditorMenue::cursorMoved(qint32 x, qint32 y)
{
    m_Topbar->hide();
    if (m_xyTextInfo.get() != nullptr)
    {
        m_xyTextInfo->setHtmlText("X: " + QString::number(x) + " Y: " + QString::number(y));
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
    
}

void EditorMenue::onMapClickedRight(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::onMapClickedRight", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::onMapClickedLeftDown", Console::eDEBUG);
    // resolve click
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
    CONSOLE_PRINT("EditorMenue::onMapClickedLeftUp", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::onMapClickedLeft", Console::eDEBUG);
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
                spDialogModifyUnit pDialog = spDialogModifyUnit::create(m_pMap.get(), pUnit);
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
                spDialogModifyTerrain pDialog = spDialogModifyTerrain::create(m_pMap.get(), pTerrain);
                addChild(pDialog);
                connect(pDialog.get(), &DialogModifyTerrain::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                setFocused(false);
            }
            else
            {
                createTempFile();
                spDialogModifyBuilding pDialog = spDialogModifyBuilding::create(m_pMap.get(), pTerrain->getBuilding());
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
    
}

void EditorMenue::editFinishedCanceled()
{
    setFocused(true);
}

void EditorMenue::scriptFinished()
{
    CONSOLE_PRINT("EditorMenue::scriptFinished", Console::eDEBUG);
    setFocused(true);
}

void EditorMenue::campaignFinished()
{
    CONSOLE_PRINT("EditorMenue::campaignFinished", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::placeTerrain", Console::eDEBUG);
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    for (qint32 i = 0; i < points.size(); i++)
    {
        // nice we can place the terrain
        if (canTerrainBePlaced(points.at(i).x(), points.at(i).y()))
        {
            QString terrainID = m_EditorSelection->getCurrentTerrainID();
            spUnit pUnit;
            m_pMap->getTerrain(points.at(i).x(), points.at(i).y())->setUnit(pUnit);
            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValue useTerrainAsBaseTerrain = pInterpreter->doFunction(terrainID, function1);
            m_pMap->replaceTerrain(terrainID, points.at(i).x(), points.at(i).y(), useTerrainAsBaseTerrain.toBool(), false);
        }
    }
    if (points.size() > 10)
    {
        m_pMap->updateSprites(-1, -1, true);
    }
    else
    {
        m_pMap->updateSpritesOfTiles(points, true);
    }
    pApp->continueRendering();
}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::placeBuilding", Console::eDEBUG);    
    
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
    if (pCurrentBuilding->getBuildingWidth() > 1 ||
        pCurrentBuilding->getBuildingHeigth() > 1)
    {
        points = PathFindingSystem::getFields(x, y, 0, 0);
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        // point still on the map great :)
        qint32 curX = points.at(i).x();
        qint32 curY = points.at(i).y();

        if (!canBuildingBePlaced(curX, curY) &&
            pCurrentBuilding->getBuildingWidth() == 1 &&
            pCurrentBuilding->getBuildingHeigth() == 1)
        {
            QString baseTerrain = pCurrentBuilding->getBaseTerrain()[0];
            m_pMap->replaceTerrain(baseTerrain, curX, curY, false, false);
        }
        if (canBuildingBePlaced(curX, curY))
        {
            if (pCurrentBuilding->getBuildingID() != m_pMap->getTerrain(curX, curY)->getTerrainID())
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID(), m_pMap.get());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                m_pMap->getTerrain(curX, curY)->setBuilding(pBuilding);
            }
        }
    }
    if (points.size() > 10)
    {
        m_pMap->updateSprites(-1, -1, true);
    }
    else
    {
        m_pMap->updateSpritesOfTiles(points, true);
    }
    if (Settings::getSyncAnimations())
    {
        m_pMap->syncUnitsAndBuildingAnimations();
    }
    pApp->continueRendering();
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
    CONSOLE_PRINT("EditorMenue::placeUnit", Console::eDEBUG);
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
    for (qint32 i = 0; i < points.size(); i++)
    {
        // point still on the map great :)
        qint32 curX = points.at(i).x();
        qint32 curY = points.at(i).y();
        if (canUnitBePlaced(curX, curY))
        {
            spUnit pCurrentUnit = m_EditorSelection->getCurrentSpUnit();
            spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, m_pMap.get());
            pUnit->setAiMode(GameEnums::GameAi::GameAi_Normal);
            
            m_pMap->getTerrain(curX, curY)->setUnit(pUnit);
            
        }
    }
    if (Settings::getSyncAnimations())
    {
        m_pMap->syncUnitsAndBuildingAnimations();
    }
}

void EditorMenue::saveMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::saveMap " + filename, Console::eDEBUG);
    if (filename.startsWith(oxygine::Resource::RCC_PREFIX_PATH))
    {
        filename.replace(oxygine::Resource::RCC_PREFIX_PATH, Settings::getUserPath());
    }
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        
        m_pMap->serializeObject(stream);
        file.close();
    }
    setFocused(true);    
}

void EditorMenue::loadMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::loadMap " + filename, Console::eDEBUG);
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
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

void EditorMenue::importAWDCAw4Map(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importAWDCAw4Map " + filename, Console::eDEBUG);
    if (filename.endsWith(".aw4"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importAWDCMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::importAWByWeb(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importAWByWeb " + filename, Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::importAWDSAwsMap " + filename, Console::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            m_pMap->importAWDSMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
    updateGrids();
}

void EditorMenue::exportAWDSAwsMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::exportAWDSAwsMap", Console::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        m_pMap->exportAWDSMap(filename);
    }
    setFocused(true);
}

void EditorMenue::importCoWTxTMap(QString filename)
{
    CONSOLE_PRINT("EditorMenue::importCoWTxTMap", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::newMap", Console::eDEBUG);
    cleanTemp(-1);
    
    m_pMap->setMapName(info.mapName);
    m_pMap->setMapAuthor(info.author);
    m_pMap->setMapDescription(info.description);
    m_pMap->getGameScript()->setScriptFile(info.scriptFile);
    m_pMap->newMap(info.mapWidth, info.mapHeigth, info.playerCount);
    m_pMap->setMapFlags(info.mapFlags);
    m_pMap->getGameRecorder()->setDeployLimit(info.deployLimit);
    m_pMap->getGameRecorder()->setMapTime(info.turnLimit);
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    updateGrids();
}

void EditorMenue::changeMap(MapEditDialog::MapEditInfo info)
{    
    CONSOLE_PRINT("EditorMenue::changeMap", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::selectionChanged", Console::eDEBUG);
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
    CONSOLE_PRINT("EditorMenue::createMarkedArea", Console::eDEBUG);
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
                oxygine::spSprite pSprite = oxygine::spSprite::create();
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
                pSprite = oxygine::spSprite::create();
                pAnim = pObjectManager->getResAnim("cursor+border+bottom");
                pSprite->setResAnim(pAnim);
                pSprite->setColor(color);
                pSprite->setPosition(GameMap::getImageSize() * i,
                                     GameMap::getImageSize() * y);
                pActor->addChild(pSprite);
            }
            for (qint32 i = p1.y(); i != p2.y() + yDir; i += yDir)
            {
                oxygine::spSprite pSprite = oxygine::spSprite::create();
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
                pSprite = oxygine::spSprite::create();
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
                spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(p2.x(), p2.x()));
                ObjectManager* pObjectManager = ObjectManager::getInstance();
                for (qint32 i = 0; i < pPoints->size(); i++)
                {
                    QPoint point = pPoints->at(i);
                    if (point.x() >= 0)
                    {
                        oxygine::spSprite pSprite = oxygine::spSprite::create();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+right");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.x() <= 0)
                    {
                        oxygine::spSprite pSprite = oxygine::spSprite::create();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+left");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.y() >= 0)
                    {
                        oxygine::spSprite pSprite = oxygine::spSprite::create();
                        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("cursor+border+bottom");
                        pSprite->setResAnim(pAnim);
                        pSprite->setColor(color);
                        pSprite->setPosition(GameMap::getImageSize() * (p1.x() + point.x()),
                                             GameMap::getImageSize() * (p1.y() + point.y()));
                        pActor->addChild(pSprite);
                    }
                    if (point.y() <= 0)
                    {
                        oxygine::spSprite pSprite = oxygine::spSprite::create();
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
    CONSOLE_PRINT("EditorMenue::pasteSelection", Console::eDEBUG);
    
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
                                pTerrain->setFixedSprite(!id.isEmpty());
                                pTerrain->setTerrainSpriteName(id);
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
                                    spBuilding pCopyBuilding = spBuilding::create(pBuilding->getBuildingID(), m_pMap.get());
                                    pCopyBuilding->setOwner(pBuilding->getOwner());
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
                                        spUnit pCopyUnit = spUnit::create(pUnit->getUnitID(), pUnit->getOwner(), false, m_pMap.get());
                                        spUnit pUnit;
                                        m_pMap->getTerrain(x + xPos, y + yPos)->setUnit(pUnit);
                                        m_pMap->getTerrain(x + xPos, y + yPos)->setUnit(pCopyUnit);
                                        pCopyUnit->setHp(pUnit->getHp());
                                        pCopyUnit->setAmmo1(pUnit->getAmmo1());
                                        pCopyUnit->setAmmo2(pUnit->getAmmo2());
                                        pCopyUnit->setFuel(pUnit->getFuel());
                                        pCopyUnit->setAiMode(pUnit->getAiMode());
                                        pCopyUnit->setUnitRank(pUnit->getUnitRank());
                                        pCopyUnit->setModdingFlags(pCopyUnit->getModdingFlags());
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
                m_pMap->updateSprites();
            }
        }
    }
}

void EditorMenue::exitEditor()
{    
    CONSOLE_PRINT("Leaving Editor Menue", Console::eDEBUG);
    auto window = spMainwindow::create();
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
    deleteMenu();
}

void EditorMenue::autosave()
{
    CONSOLE_PRINT("EditorMenue::autosave", Console::eDEBUG);
    QString filename = "maps/autosave.map";
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        
        m_pMap->serializeObject(stream);
        file.close();
    }    
}
