#include <QFile>
#include <qdir.h>
#include <qcryptographichash.h>

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
#include "objects/dialogs/editor/mapeditdialog.h"
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

#include "wiki/fieldinfo.h"

#include "ui_reader/uifactory.h"

EditorMenue* EditorMenue::m_pInstance = nullptr;

EditorMenue::EditorMenue()
    : InGameMenue (20, 20, "", false)
{
    setObjectName("EditorMenue");
    Mainapp* pApp = Mainapp::getInstance();
    oxygine::Actor::addChild(GameMap::getInstance());
    loadHandling();
    changeBackground("editormenu");
    this->moveToThread(pApp->getWorkerthread());
    m_pInstance = this;

    m_autoScrollBorder = QRect(50, 50, Settings::getWidth() / 4, 50);

    m_EditorSelection = spEditorSelection::create();
    addChild(m_EditorSelection);

    m_Topbar = spTopbar::create(0, Settings::getWidth());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapeditor");
    pApp->getAudioThread()->playRandom();

    m_Topbar->addGroup(tr("Menu"));
    m_Topbar->addItem(tr("Save Map"), "SAVEMAP", 0, tr("Saves a map to a give file."));
    m_Topbar->addItem(tr("Load Map"), "LOADMAP", 0, tr("Loads a map to a give file."));
    m_Topbar->addItem(tr("Edit Script"), "EDITSCRIPT", 0, tr("Edit and create a script for any map."));
    m_Topbar->addItem(tr("Edit Campaign"), "EDITCAMPAIGN", 0, tr("Edit and create a campaign."));
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

    m_Topbar->addGroup(tr("Commands"));
    m_Topbar->addItem(tr("Place Selection"), "PLACESELECTION", 2, tr("Selects the editor mode placing the current tile"));
    m_Topbar->addItem(tr("Delete Units") + " - " + SelectKey::getKeycodeText(Settings::getKey_cancel()), "DELETEUNITS", 2, tr("Selects the editor mode deleting units"));
    m_Topbar->addItem(tr("Edit Units"), "EDITUNITS", 2, tr("Selects the editor mode modifying the stats of a unit"));
    m_Topbar->addItem(tr("Edit Terrain"), "EDITTERRAIN", 2, tr("Selects the editor mode editing the style of a terrain or building"));
    m_Topbar->addItem(tr("Edit Players"), "EDITPLAYERS", 2, tr("Edit the CO's and player start setup."));
    m_Topbar->addItem(tr("Edit Rules"), "EDITRULES", 2, tr("Selects the editor rules for the map."));
    m_Topbar->addItem(tr("Optimize Players"), "OPTIMIZEPLAYERS", 2, tr("Removes all players with no units or buildings from the map"));
    m_Topbar->addItem(tr("Copy Ctrl+C"), "COPY", 2, tr("Enters the copy mode. Hold the left mouse key and mark the fields you want to copy. Copying is based on the current placing mode"));
    m_Topbar->addItem(tr("Paste Ctrl+V"), "PASTE", 2, tr("Paste the current selected area. Based on the current placing mode. The copy and paste selection are not allowed to intersec."));
    m_Topbar->addItem(tr("Paste Ctrl+Shift+V"), "PASTEALL", 2, tr("Paste the current selected area with all terrain, buildings, units. The copy and paste selection are not allowed to intersec."));

    m_Topbar->addGroup(tr("Import/Export"));
    m_Topbar->addItem(tr("Import CoW Txt"), "IMPORTCOWTXT", 3, tr("Deletes the current map and imports an old Commander Wars Map from a file."));
    m_Topbar->addItem(tr("Import AWDS Aws"), "IMPORTAWDSAWS", 3, tr("Deletes the current map and imports an AWS Map Editor from a file."));
    m_Topbar->addItem(tr("Export AWDS Aws"), "EXPORTAWDSAWS", 3, tr("Exports the map to an AWS Map Editor file"));
    m_Topbar->addItem(tr("Import AWDC Aw4"), "IMPORTAWDCAW4", 3, tr("Deletes the current map and imports an AW DoR/DC Map Editor from a file."));
    m_Topbar->addItem(tr("Import AW by Web"), "IMPORTAWDBYWEB", 3, tr("Deletes the current map and imports an  Advance Wars by Web Map from https://awbw.amarriner.com/"));

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    if (!Settings::getsmallScreenDevice())
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

    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            static qint32 lastX = -1;
            static qint32 lastY = -1;
            if (pTouchEvent->getPointer()->isPressed(oxygine::MouseButton::MouseButton_Left))
            {
                if (m_Cursor->getMapPointX() != lastX ||
                    m_Cursor->getMapPointY() != lastY)
                {
                    lastX = m_Cursor->getMapPointX();
                    lastY = m_Cursor->getMapPointY();
                    emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
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
    connect(pApp, &Mainapp::sigKeyDown, this, &EditorMenue::KeyInput, Qt::QueuedConnection);
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

EditorMenue::~EditorMenue()
{
    cleanTemp(-1);
    m_pInstance = nullptr;
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
    Console::print(QString("createTempFile(") + (cleanUp ? "true" : "false") + ")", Console::eDEBUG);
    if (cleanUp)
    {
        cleanTemp(m_tempCounter);
    }
    QFile file("temp/temp" + QString::number(m_tempCounter) + ".tmp");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream stream(&file);
    spGameMap pMap = GameMap::getInstance();
    pMap->serializeObject(stream);
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
    Console::print("EditorMenue::editorUndo", Console::eDEBUG);
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
            GameMap::getInstance()->deserializeObject(stream);
            file.close();
            spGameMap pMap = GameMap::getInstance();
            Mainapp::getInstance()->pauseRendering();
            pMap->updateSprites();
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
    Console::print("EditorMenue::editorRedo", Console::eDEBUG);
    m_tempCounter++;
    QFile file("temp/temp" + QString::number(m_tempCounter) + ".tmp");
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QDataStream stream(&file);
        GameMap::getInstance()->deserializeObject(stream);
        file.close();
        spGameMap pMap = GameMap::getInstance();
        Mainapp::getInstance()->pauseRendering();
        pMap->updateSprites();
        Mainapp::getInstance()->continueRendering();
        m_EditorSelection->createPlayerSelection();
    }
}

void EditorMenue::clickedTopbar(QString itemID)
{    
    Console::print("clickedTopbar(" + itemID + ")", Console::eDEBUG);
    if (itemID == "EXIT")
    {
        m_Focused = false;
        spDialogMessageBox pExit = spDialogMessageBox::create(tr("Do you want to exit the map editor?"), true);
        connect(pExit.get(), &DialogMessageBox::sigOk, this, &EditorMenue::exitEditor, Qt::QueuedConnection);
        connect(pExit.get(), &DialogMessageBox::sigCancel, [=]()
        {
            m_Focused = true;
        });
        addChild(pExit);
    }
    else if (itemID == "SAVEMAP")
    {
        QVector<QString> wildcards;
        wildcards.append("*.map");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards, GameMap::getInstance()->getMapName());
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::saveMap, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);

        setFocused(false);
    }
    else if (itemID == "LOADMAP")
    {
        QVector<QString> wildcards;
        wildcards.append("*.map");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::loadMap, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "UNDO")
    {
        editorUndo();
    }
    else if (itemID == "REDO")
    {
        editorRedo();
    }
    else if (itemID == "EDITSCRIPT")
    {
        spScriptEditor scriptEditor = spScriptEditor::create();
        this->addChild(scriptEditor);
        connect(scriptEditor.get(),  &ScriptEditor::sigFinished, this, &EditorMenue::scriptFinished, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "EDITCAMPAIGN")
    {
        spCampaignEditor campaignEditor = spCampaignEditor::create();
        this->addChild(campaignEditor);
        connect(campaignEditor.get(),  &CampaignEditor::sigFinished, this, &EditorMenue::campaignFinished, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "IMPORTCOWTXT")
    {
        QVector<QString> wildcards;
        wildcards.append("*.txt");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importCoWTxTMap, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "IMPORTAWDSAWS")
    {
        QVector<QString> wildcards;
        wildcards.append("*.aws");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWDSAwsMap, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "EXPORTAWDSAWS")
    {
        QVector<QString> wildcards;
        wildcards.append("*.aws");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::exportAWDSAwsMap, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "IMPORTAWDCAW4")
    {
        QVector<QString> wildcards;
        wildcards.append("*.aw4");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWDCAw4Map, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "IMPORTAWDBYWEB")
    {
        QVector<QString> wildcards;
        wildcards.append("*.txt");
        QString path = QCoreApplication::applicationDirPath() + "/maps";
        spFileDialog fileDialog = spFileDialog::create(path, wildcards);
        this->addChild(fileDialog);
        connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &EditorMenue::importAWByWeb, Qt::QueuedConnection);
        connect(fileDialog.get(), &FileDialog::sigCancel, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "NEWMAP")
    {
        spMapEditDialog mapEditDialog = spMapEditDialog::create("", Settings::getUsername(), "", "", 20, 20, 2, 0, 0);
        connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::newMap, Qt::QueuedConnection);
        connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        this->addChild(mapEditDialog);
        setFocused(false);
    }
    else if (itemID == "EDITMAP")
    {
        GameMap* pGameMap = GameMap::getInstance();
        spMapEditDialog mapEditDialog = spMapEditDialog::create(pGameMap->getMapName(), pGameMap->getMapAuthor(), pGameMap->getMapDescription(),
                                                                pGameMap->getGameScript()->getScriptFile(), pGameMap->getMapWidth(),
                                                                pGameMap->getMapHeight(), pGameMap->getPlayerCount(),
                                                                pGameMap->getGameRecorder()->getMapTime(), pGameMap->getGameRecorder()->getDeployLimit());
        connect(mapEditDialog.get(), &MapEditDialog::editFinished, this, &EditorMenue::changeMap, Qt::QueuedConnection);
        connect(mapEditDialog.get(), &MapEditDialog::sigCanceled, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
        this->addChild(mapEditDialog);
        setFocused(false);
    }
    else if (itemID == "FLIPX")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->flipX();
    }
    else if (itemID == "FLIPY")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->flipY();
    }
    else if (itemID == "ROTATEX")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->rotateX();
    }
    else if (itemID == "ROTATEY")
    {
        GameMap* pGameMap = GameMap::getInstance();
        pGameMap->rotateY();
    }
    else if (itemID == "RANDOMMAP")
    {
        spDialogRandomMap pDialogRandomMap = spDialogRandomMap::create();
        addChild(pDialogRandomMap);
        connect(pDialogRandomMap.get(), &DialogRandomMap::sigFinished, this, &EditorMenue::createRandomMap, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "PLACESELECTION")
    {
        m_EditorMode = EditorModes::PlaceEditorSelection;
        selectionChanged();
    }
    else if (itemID == "DELETEUNITS")
    {
        m_EditorMode = EditorModes::RemoveUnits;
        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
    }
    else if (itemID == "EDITUNITS")
    {
        m_EditorMode = EditorModes::EditUnits;
        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
    }
    else if (itemID == "EDITTERRAIN")
    {
        m_EditorMode = EditorModes::EditTerrain;
        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(0, 0), CursorModes::Circle);
    }
    else if (itemID == "OPTIMIZEPLAYERS")
    {
        optimizePlayers();
    }
    else if (itemID == "EDITPLAYERS")
    {
        spPlayerSelectionDialog pDialog = spPlayerSelectionDialog::create();
        addChild(pDialog);
        connect(pDialog.get(), &PlayerSelectionDialog::sigPlayersChanged, this, &EditorMenue::playersChanged, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "EDITRULES")
    {
        spRuleSelectionDialog pDialog = spRuleSelectionDialog::create(RuleSelection::Mode::Editor);
        addChild(pDialog);
        connect(pDialog.get(), &RuleSelectionDialog::sigRulesChanged, this, &EditorMenue::rulesChanged, Qt::QueuedConnection);
        setFocused(false);
    }
    else if (itemID == "COPY")
    {
        m_EditorMode = EditorModes::CopySelection;
        m_copyRect = QRect(-1, -1, 0, 0);
    }
    else if (itemID == "PASTE")
    {
        // do nothing
    }
    else if (itemID == "PASTEALL")
    {
        // do nothing
    }
    else if (itemID == "RESIZEMAP")
    {
        showResizeMap();
    }
    
}

void EditorMenue::showResizeMap()
{
    Console::print("showResizeMap()", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    spGenericBox pBox = spGenericBox::create(true);

    oxygine::TextStyle style = FontManager::getMainFont24();
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
    spSpinBox leftBox = spSpinBox::create(150, -pMap->getMapWidth() + 1, 9999);
    leftBox->setTooltipText(tr("Change of the map size on the left map border."));
    leftBox->setPosition(width, y);
    leftBox->setInfinityValue(-pMap->getMapWidth());
    leftBox->setCurrentValue(0);
    pBox->addItem(leftBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Top: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox topBox = spSpinBox::create(150, -pMap->getMapHeight() + 1, 999999);
    topBox->setTooltipText(tr("Change of the map size on the top map border."));
    topBox->setPosition(width, y);
    topBox->setInfinityValue(-pMap->getMapHeight());
    topBox->setCurrentValue(0);
    pBox->addItem(topBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Right: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox rightBox = spSpinBox::create(150, -pMap->getMapWidth() + 1, 999999);
    rightBox->setTooltipText(tr("Change of the map size on the right map border."));
    rightBox->setPosition(width, y);
    rightBox->setInfinityValue(-pMap->getMapWidth());
    rightBox->setCurrentValue(0);
    pBox->addItem(rightBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Bottom: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox bottomBox = spSpinBox::create(150, -pMap->getMapHeight() + 1, 999999);
    bottomBox->setTooltipText(tr("Change of the map size on the bottom map border."));
    bottomBox->setPosition(width, y);
    bottomBox->setInfinityValue(-pMap->getMapHeight());
    bottomBox->setCurrentValue(0);
    pBox->addItem(bottomBox);
    y += 40;

    addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, this, [=]
    {
        emit sigResizeMap(leftBox->getCurrentValue(), topBox->getCurrentValue(),
                          rightBox->getCurrentValue(), bottomBox->getCurrentValue());
    });
    
}

void EditorMenue::resizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom)
{    
    Console::print("EditorMenue::resizeMap", Console::eDEBUG);
    GameMap::getInstance()->resizeMap(left, top, right, bottom);    
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
                                  bool unitsDistributed)
{
    Console::print("EditorMenue::createRandomMap", Console::eDEBUG);
    cleanTemp(-1);
    GameMap* pGameMap = GameMap::getInstance();
    pGameMap->randomMap(width, heigth, playerCount, roadSupport, seed,
                        terrains, buildings, ownedBaseSize, startBaseSize / 100.0f,
                        units, unitCount, startBaseUnitSize / 100.0f, unitDistribution, unitsDistributed);
    pGameMap->setMapName(mapName);
    pGameMap->setMapAuthor(author);
    pGameMap->setMapDescription(description);
    Mainapp::getInstance()->pauseRendering();
    pGameMap->updateSprites();
    Mainapp::getInstance()->continueRendering();
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    
}

void EditorMenue::playersChanged()
{    
    Console::print("EditorMenue::playersChanged", Console::eDEBUG);
    m_EditorSelection->createPlayerSelection();
    spGameMap pMap = GameMap::getInstance();
    Mainapp::getInstance()->pauseRendering();
    pMap->updateSprites();
    Mainapp::getInstance()->continueRendering();
    setFocused(true);
}

void EditorMenue::rulesChanged()
{
    Console::print("EditorMenue::rulesChanged", Console::eDEBUG);
    setFocused(true);
}

void EditorMenue::optimizePlayers()
{
    Console::print("EditorMenue::optimizePlayers", Console::eDEBUG);
    createTempFile();
    spGameMap pMap = GameMap::getInstance();
    QVector<bool> foundPlayers(pMap->getPlayerCount(), false);
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
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
        if (pMap->getPlayerCount() > 2)
        {
            if (foundPlayers[i] == false)
            {
                pMap->removePlayer(i);
            }
        }
    }
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (pMap->getPlayer(i)->getTeam() >= pMap->getPlayerCount())
        {
            pMap->getPlayer(i)->setTeam(i);
        }
    }
    m_EditorSelection->createPlayerSelection();
    
}

void EditorMenue::KeyInput(oxygine::KeyEvent event)
{    
    if (!event.getContinousPress())
    {
        InGameMenue::keyInput(event);
        // for debugging
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
                spGameMap pMap = GameMap::getInstance();
                if (pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
                {
                    Terrain* pTerrain = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    spFieldInfo fieldinfo = spFieldInfo::create(pTerrain, pTerrain->getUnit());
                    this->addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, [=]
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
            m_EditorSelection->KeyInput(cur);
        }
    }
    
}

void EditorMenue::cursorMoved(qint32 x, qint32 y)
{
    m_Topbar->hide();
    if (m_xyTextInfo.get() != nullptr)
    {
        m_xyTextInfo->setHtmlText("X: " + QString::number(x) + " Y: " + QString::number(y));
    }
    spGameMap pMap = GameMap::getInstance();
    m_copyRectActor->detach();
    if (pMap->onMap(x, y))
    {
        pMap->addChild(m_cursorActor);
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
            m_Cursor->changeCursor("cursor+edit");
            break;
        }
        case EditorModes::EditTerrain:
        {
            m_Cursor->changeCursor("cursor+edit");
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
            pMap->addChild(m_copyRectActor);
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
    Console::print("EditorMenue::onMapClickedRight", Console::eDEBUG);
    // resolve click
    spGameMap pMap = GameMap::getInstance();
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
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
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
                        QString terrainID = pMap->getTerrain(x, y)->getTerrainID();
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
    Console::print("EditorMenue::onMapClickedLeftDown", Console::eDEBUG);
    // resolve click
    switch (m_EditorMode)
    {
        case EditorModes::CopySelection:
        {
            if (m_copyRect.x() < 0)
            {
                pasteSelection(x, y, true, m_EditorSelection->getCurrentMode());
                GameMap::getInstance()->addChild(m_copyRectActor);
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
    Console::print("EditorMenue::onMapClickedLeftUp", Console::eDEBUG);
    // resolve click
    switch (m_EditorMode)
    {
        case EditorModes::CopySelection:
        {
            if (m_copyRect.x() >= 0)
            {
                pasteSelection(x, y, true, m_EditorSelection->getCurrentMode());
                GameMap::getInstance()->addChild(m_copyRectActor);
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
    Console::print("EditorMenue::onMapClickedLeft", Console::eDEBUG);
    // resolve click
    switch (m_EditorMode)
    {
        case EditorModes::RemoveUnits:
        {
            Unit* pUnit = GameMap::getInstance()->getTerrain(x, y)->getUnit();
            if (pUnit != nullptr)
            {
                createTempFile();
                pUnit->killUnit();
            }
            break;
        }
        case EditorModes::EditUnits:
        {
            Unit* pUnit = GameMap::getInstance()->getTerrain(x, y)->getUnit();
            if (pUnit != nullptr)
            {
                createTempFile();
                spDialogModifyUnit pDialog = spDialogModifyUnit::create(pUnit);
                addChild(pDialog);
                connect(pDialog.get(), &DialogModifyUnit::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                setFocused(false);
            }
            break;
        }
        case EditorModes::EditTerrain:
        {
            Terrain* pTerrain  = GameMap::getInstance()->getTerrain(x, y);
            if (pTerrain->getBuilding() == nullptr)
            {
                createTempFile();
                spDialogModifyTerrain pDialog = spDialogModifyTerrain::create(pTerrain);
                addChild(pDialog);
                connect(pDialog.get(), &DialogModifyTerrain::sigFinished, this, &EditorMenue::editFinishedCanceled, Qt::QueuedConnection);
                setFocused(false);
            }
            else
            {
                createTempFile();
                spDialogModifyBuilding pDialog = spDialogModifyBuilding::create(pTerrain->getBuilding());
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
    Console::print("EditorMenue::scriptFinished", Console::eDEBUG);
    setFocused(true);
}

void EditorMenue::campaignFinished()
{
    Console::print("EditorMenue::campaignFinished", Console::eDEBUG);
    setFocused(true);
}

bool EditorMenue::canTerrainBePlaced(qint32 x, qint32 y)
{
    bool ret = false;
    QString terrainID = m_EditorSelection->getCurrentTerrainID();
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        if (pMap->canBePlaced(terrainID, x, y))
        {
            if (pMap->getTerrain(x, y)->getTerrainID() != terrainID ||
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
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        spBuilding pCurrentBuilding = m_EditorSelection->getCurrentSpBuilding();
        ret = pCurrentBuilding->canBuildingBePlaced(pMap->getTerrain(x, y));
    }
    return ret;
}

bool EditorMenue::canUnitBePlaced(qint32 x, qint32 y)
{
    bool ret = false;
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = m_EditorSelection->getCurrentSpUnit()->getMovementType();
        if (pMovementTableManager->getBaseMovementPoints(movementType, pMap->getTerrain(x, y), pMap->getTerrain(x, y), m_EditorSelection->getCurrentSpUnit().get()) > 0)
        {
            ret = true;
        }
    }
    return ret;
}

void EditorMenue::placeTerrain(qint32 x, qint32 y)
{
    Console::print("EditorMenue::placeTerrain", Console::eDEBUG);
    QVector<QPoint> points;
    spGameMap pMap = GameMap::getInstance();
    switch (m_EditorSelection->getSizeMode())
    {
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
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        // nice we can place the terrain
        if (canTerrainBePlaced(points.at(i).x(), points.at(i).y()))
        {
            QString terrainID = m_EditorSelection->getCurrentTerrainID();

            pMap->getTerrain(points.at(i).x(), points.at(i).y())->setUnit(nullptr);

            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValueList args1;
            QJSValue useTerrainAsBaseTerrain = pInterpreter->doFunction(terrainID, function1, args1);
            if (points.size() < 14)
            {
                pMap->replaceTerrain(terrainID, points.at(i).x(), points.at(i).y(), useTerrainAsBaseTerrain.toBool(), true);
            }
            else
            {
                pMap->replaceTerrain(terrainID, points.at(i).x(), points.at(i).y(), useTerrainAsBaseTerrain.toBool(), false);
            }
            
        }
    }
    if (points.size() >= 14)
    {
        pMap->updateSprites();
    }

}

void EditorMenue::placeBuilding(qint32 x, qint32 y)
{
    Console::print("EditorMenue::placeBuilding", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
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
        case EditorSelection::PlacementSize::Big:
        {
            points = PathFindingSystem::getFields(x, y, 0, 2);
            break;
        }
        case EditorSelection::PlacementSize::Fill:
        {
            TerrainFindingSystem Pfs(pMap->getTerrain(x, y)->getID(),x , y);
            Pfs.explore();
            points = Pfs.getAllNodePoints();
            break;
        }
    }
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
            pMap->replaceTerrain(baseTerrain, curX, curY, false, false);
        }
        if (canBuildingBePlaced(curX, curY))
        {
            if (pCurrentBuilding->getBuildingID() != pMap->getTerrain(curX, curY)->getTerrainID())
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pMap->getTerrain(curX, curY)->setBuilding(pBuilding.get());
                if (points.size() < 14)
                {
                    pMap->updateTerrain(points.at(i).x(), points.at(i).y());
                    pMap->updateSprites(points.at(i).x(), points.at(i).y());
                }
                
            }
        }
    }
    if (points.size() >= 14)
    {
        pMap->updateSprites();
    }
    if (Settings::getSyncAnimations())
    {
        GameMap::getInstance()->syncUnitsAndBuildingAnimations();
    }
}

void EditorMenue::placeUnit(qint32 x, qint32 y)
{
    Console::print("EditorMenue::placeUnit", Console::eDEBUG);
    QVector<QPoint> points;
    switch (m_EditorSelection->getSizeMode())
    {
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
            spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
            pUnit->setAiMode(GameEnums::GameAi::GameAi_Normal);
            spGameMap pMap = GameMap::getInstance();
            pMap->getTerrain(curX, curY)->setUnit(pUnit);
            
        }
    }
    if (Settings::getSyncAnimations())
    {
        GameMap::getInstance()->syncUnitsAndBuildingAnimations();
    }
}

void EditorMenue::saveMap(QString filename)
{
    Console::print("EditorMenue::saveMap", Console::eDEBUG);
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        spGameMap pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
    }
    setFocused(true);    
}

void EditorMenue::loadMap(QString filename)
{
    Console::print("EditorMenue::loadMap", Console::eDEBUG);
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            GameMap::getInstance()->deserializeObject(stream);
            file.close();
            spGameMap pMap = GameMap::getInstance();
            Mainapp::getInstance()->pauseRendering();
            pMap->updateSprites(-1, -1, true);
            pMap->centerMap(pMap->getMapWidth() / 2, pMap->getMapHeight() / 2);
            m_EditorSelection->createPlayerSelection();
            Mainapp::getInstance()->continueRendering();
        }
    }
    setFocused(true);
    
}

void EditorMenue::importAWDCAw4Map(QString filename)
{
    Console::print("EditorMenue::importAWDCAw4Map", Console::eDEBUG);
    if (filename.endsWith(".aw4"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            GameMap::getInstance()->importAWDCMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
}

void EditorMenue::importAWByWeb(QString filename)
{
    Console::print("EditorMenue::importAWByWeb", Console::eDEBUG);
    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            GameMap::getInstance()->importAWByWebMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
}

void EditorMenue::importAWDSAwsMap(QString filename)
{
    Console::print("EditorMenue::importAWDSAwsMap", Console::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            GameMap::getInstance()->importAWDSMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
}

void EditorMenue::exportAWDSAwsMap(QString filename)
{
    Console::print("EditorMenue::exportAWDSAwsMap", Console::eDEBUG);
    if (filename.endsWith(".aws"))
    {
        GameMap::getInstance()->exportAWDSMap(filename);
    }
    setFocused(true);
}

void EditorMenue::importCoWTxTMap(QString filename)
{
    Console::print("EditorMenue::importCoWTxTMap", Console::eDEBUG);
    if (filename.endsWith(".txt"))
    {
        QFile file(filename);
        if (file.exists())
        {
            cleanTemp(-1);
            GameMap::getInstance()->importTxtMap(filename);
            m_EditorSelection->createPlayerSelection();
        }
    }
    setFocused(true);
}

void EditorMenue::newMap(QString mapName, QString author, QString description, QString scriptFile,
                         qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                         qint32 turnLimit, quint32 buildLimit)
{
    Console::print("EditorMenue::newMap", Console::eDEBUG);
    cleanTemp(-1);
    spGameMap pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->setMapAuthor(author);
    pMap->setMapDescription(description);
    pMap->getGameScript()->setScriptFile(scriptFile);
    pMap->newMap(mapWidth, mapHeigth, playerCount);
    pMap->getGameRecorder()->setDeployLimit(buildLimit);
    pMap->getGameRecorder()->setMapTime(turnLimit);

    m_EditorSelection->createPlayerSelection();
    setFocused(true);
    
}

void EditorMenue::changeMap(QString mapName, QString author, QString description, QString scriptFile,
                            qint32 mapWidth, qint32 mapHeigth, qint32 playerCount,
                            qint32 turnLimit, quint32 buildLimit)
{    
    Console::print("EditorMenue::changeMap", Console::eDEBUG);
    createTempFile();
    spGameMap pMap = GameMap::getInstance();
    pMap->setMapName(mapName);
    pMap->setMapAuthor(author);
    pMap->setMapDescription(description);
    pMap->getGameScript()->setScriptFile(scriptFile);
    pMap->changeMap(mapWidth, mapHeigth, playerCount);
    pMap->getGameRecorder()->setDeployLimit(buildLimit);
    pMap->getGameRecorder()->setMapTime(turnLimit);
    m_EditorSelection->createPlayerSelection();
    setFocused(true);
}

void EditorMenue::selectionChanged()
{
    Console::print("EditorMenue::selectionChanged", Console::eDEBUG);
    if (m_EditorMode == EditorModes::PlaceEditorSelection)
    {
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
            case EditorSelection::PlacementSize::Medium:
            {
                createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(1, -1), CursorModes::Circle);
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
        createMarkedArea(m_cursorActor, QPoint(0, 0), QPoint(m_copyRect.width() - 1, m_copyRect.height() - 1), CursorModes::Rect);
    }
}

void EditorMenue::createMarkedArea(oxygine::spActor pActor, QPoint p1, QPoint p2, CursorModes mode, QColor color)
{
    Console::print("EditorMenue::createMarkedArea", Console::eDEBUG);
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
                spQmlVectorPoint pPoints = GlobalUtils::getCircle(p2.x(), p2.x());
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
    Console::print("EditorMenue::pasteSelection", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
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
            if (pMap->onMap(rect.x(), rect.y()) &&
                pMap->onMap(rect.right(), rect.bottom()) &&
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
                                Terrain* pCopyTerrain = pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos);
                                pMap->replaceTerrain(pCopyTerrain->getBaseTerrainIDOfLevel(1), x + xPos, y + yPos, false, false);
                                pMap->replaceTerrain(pCopyTerrain->getBaseTerrainIDOfLevel(0), x + xPos, y + yPos, true, false);
                                Terrain* pTerrain = pMap->getTerrain(x + xPos, y + yPos);
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
                                Building* pBuilding = pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos)->getBuilding();
                                if (pBuilding != nullptr &&
                                    pBuilding->getBuildingWidth() == 1 &&
                                    pBuilding->getBuildingHeigth() == 1)
                                {
                                    if (pBuilding->canBuildingBePlaced(pMap->getTerrain(x + xPos, y + yPos)))
                                    {
                                        auto baseTerrains = pBuilding->getBaseTerrain();
                                        QString baseTerrain = baseTerrains[0];
                                        pMap->replaceTerrain(baseTerrain, x + xPos, y + yPos, false, false);
                                    }
                                    spBuilding pCopyBuilding = spBuilding::create(pBuilding->getBuildingID());
                                    pCopyBuilding->setOwner(pBuilding->getOwner());
                                    pMap->getTerrain(x + xPos, y + yPos)->setBuilding(pCopyBuilding.get());
                                }
                                if (selection != EditorSelection::EditorMode::All)
                                {
                                    break;
                                }
                            }
                            case EditorSelection::EditorMode::Unit:
                            {
                                Unit* pUnit = pMap->getTerrain(m_copyRect.x() + xPos, m_copyRect.y() + yPos)->getUnit();
                                if (pUnit != nullptr)
                                {
                                    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
                                    QString movementType = pUnit->getMovementType();
                                    if (pMovementTableManager->getBaseMovementPoints(movementType, pMap->getTerrain(x + xPos, y + yPos), pMap->getTerrain(x + xPos, y + yPos), pUnit) > 0)
                                    {
                                        spUnit pCopyUnit = spUnit::create(pUnit->getUnitID(), pUnit->getOwner(), false);
                                        pMap->getTerrain(x + xPos, y + yPos)->setUnit(nullptr);
                                        pMap->getTerrain(x + xPos, y + yPos)->setUnit(pCopyUnit);
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
                pMap->updateSprites();
            }
        }
    }
}

void EditorMenue::exitEditor()
{    
    Console::print("Leaving Editor Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(spMainwindow::create());
    oxygine::Actor::detach();
}

void EditorMenue::autosave()
{
    Console::print("EditorMenue::autosave", Console::eDEBUG);
    QString filename = "maps/autosave.map";
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        spGameMap pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
    }    
}
