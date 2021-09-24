#include <QFile>

#include "menue/mainwindow.h"
#include "menue/campaignmenu.h"
#include "menue/gamemenue.h"
#include "menue/editormenue.h"
#include "menue/optionmenue.h"
#include "menue/mapselectionmapsmenue.h"
#include "menue/creditsmenue.h"
#include "menue/wikimenu.h"
#include "menue/costylemenu.h"
#include "menue/replaymenu.h"
#include "menue/achievementmenu.h"
#include "menue/shopmenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/unitspritemanager.h"

#include "game/campaign.h"

#include "multiplayer/lobbymenu.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogtextinput.h"
#include "objects/dialogs/folderdialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/loadingscreen.h"

#include "ui_reader/uifactory.h"

Mainwindow::Mainwindow()
    : m_cheatTimeout(this)
{
    setObjectName("Mainwindow");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    moveToThread(pApp->getWorkerthread());
    CONSOLE_PRINT("Entering Main Menue", Console::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("mainmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    qint32 buttonWidth = 170;
    qint32 btnI = 0;
    QString lastSaveGame = Settings::getLastSaveGame();

    // create the ui for the main menue here :)
    oxygine::spButton pButtonLastSaveGame = ObjectManager::createButton(tr("Continue"), buttonWidth);
    setButtonPosition(pButtonLastSaveGame, btnI);
    addChild(pButtonLastSaveGame);
    pButtonLastSaveGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigLastSaveGame();
    });
    connect(this, &Mainwindow::sigLastSaveGame, this, &Mainwindow::lastSaveGame, Qt::QueuedConnection);
    btnI++;
    if (!QFile::exists(lastSaveGame) ||
        !lastSaveGame.endsWith(".sav"))
    {
        pButtonLastSaveGame->setEnabled(false);
    }

    // create the ui for the main menue here :)
    oxygine::spButton pButtonSingleplayer = ObjectManager::createButton(tr("Singleplayer"), buttonWidth);
    setButtonPosition(pButtonSingleplayer, btnI);
    addChild(pButtonSingleplayer);
    pButtonSingleplayer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterSingleplayer();
    });
    connect(this, &Mainwindow::sigEnterSingleplayer, this, &Mainwindow::enterSingleplayer, Qt::QueuedConnection);
    btnI++;

    // create the ui for the main menue here :)
    oxygine::spButton pButtonMultiplayer = ObjectManager::createButton(tr("Multiplayer"), buttonWidth);
    setButtonPosition(pButtonMultiplayer, btnI);
    addChild(pButtonMultiplayer);
    pButtonMultiplayer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterMultiplayer();
    });
    connect(this, &Mainwindow::sigEnterMultiplayer, this, &Mainwindow::enterMultiplayer, Qt::QueuedConnection);
    btnI++;

    // load button
    oxygine::spButton pButtonLoadGame = ObjectManager::createButton(tr("Load Game"), buttonWidth);
    addChild(pButtonLoadGame);
    setButtonPosition(pButtonLoadGame, btnI);
    pButtonLoadGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterLoadGame();
    });
    connect(this, &Mainwindow::sigEnterLoadGame, this, &Mainwindow::enterLoadGame, Qt::QueuedConnection);
    btnI++;

    // load button
    oxygine::spButton pButtonLoadCampaign = ObjectManager::createButton(tr("Load Campaign"), buttonWidth);
    addChild(pButtonLoadCampaign);
    setButtonPosition(pButtonLoadCampaign, btnI);
    pButtonLoadCampaign->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterLoadCampaign();
    });
    connect(this, &Mainwindow::sigEnterLoadCampaign, this, &Mainwindow::enterLoadCampaign, Qt::QueuedConnection);
    btnI++;

    // replay button
    oxygine::spButton pButtonReplay = ObjectManager::createButton(tr("Replay Game"), buttonWidth);
    addChild(pButtonReplay);
    setButtonPosition(pButtonReplay, btnI);
    pButtonReplay->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterReplayGame();
    });
    connect(this, &Mainwindow::sigEnterReplayGame, this, &Mainwindow::enterReplayGame, Qt::QueuedConnection);
    btnI++;

    // editor button
    oxygine::spButton pButtonEditor = ObjectManager::createButton(tr("Map Editor"), buttonWidth);
    addChild(pButtonEditor);
    setButtonPosition(pButtonEditor, btnI);
    pButtonEditor->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterEditor();
    });
    connect(this, &Mainwindow::sigEnterEditor, this, &Mainwindow::enterEditor, Qt::QueuedConnection);
    btnI++;

    // option button
    oxygine::spButton pButtonOptions = ObjectManager::createButton(tr("Options"), buttonWidth);
    addChild(pButtonOptions);
    setButtonPosition(pButtonOptions, btnI);
    pButtonOptions->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterOptionmenue();
    });
    connect(this, &Mainwindow::sigEnterOptionmenue, this, &Mainwindow::enterOptionmenue, Qt::QueuedConnection);
    btnI++;

    // co style button
    oxygine::spButton pButtonCOStyle = ObjectManager::createButton(tr("CO Style"), buttonWidth);
    addChild(pButtonCOStyle);
    setButtonPosition(pButtonCOStyle, btnI);
    pButtonCOStyle->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterCOStyleMenu();
    });
    connect(this, &Mainwindow::sigEnterCOStyleMenu, this, &Mainwindow::enterCOStyleMenu, Qt::QueuedConnection);
    btnI++;

    // co style button
    oxygine::spButton pButtonShop = ObjectManager::createButton(tr("Shop"), buttonWidth);
    addChild(pButtonShop);
    setButtonPosition(pButtonShop, btnI);
    pButtonShop->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterShopMenu();
    });
    connect(this, &Mainwindow::sigEnterShopMenu, this, &Mainwindow::enterShopMenu, Qt::QueuedConnection);
    btnI++;

    // wiki button
    oxygine::spButton pButtonWiki = ObjectManager::createButton(tr("Wiki"), buttonWidth);
    addChild(pButtonWiki);
    setButtonPosition(pButtonWiki, btnI);
    pButtonWiki->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterWikimenue();
    });
    connect(this, &Mainwindow::sigEnterWikimenue, this, &Mainwindow::enterWikimenue, Qt::QueuedConnection);
    btnI++;

    // wiki button
    oxygine::spButton pButtonAchievement = ObjectManager::createButton(tr("Achievements"), buttonWidth);
    addChild(pButtonAchievement);
    setButtonPosition(pButtonAchievement, btnI);
    pButtonAchievement->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterAchievementmenue();
    });
    connect(this, &Mainwindow::sigEnterAchievementmenue, this, &Mainwindow::enterAchievementmenue, Qt::QueuedConnection);
    btnI++;

    // credits button
    oxygine::spButton pButtonCredtis = ObjectManager::createButton(tr("Credits"), buttonWidth);
    addChild(pButtonCredtis);
    setButtonPosition(pButtonCredtis, btnI);
    pButtonCredtis->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterCreditsmenue();
    });
    connect(this, &Mainwindow::sigEnterCreditsmenue, this, &Mainwindow::enterCreditsmenue, Qt::QueuedConnection);
    btnI++;

    // quit button
    oxygine::spButton pQuit = ObjectManager::createButton(tr("Quit"), buttonWidth);
    addChild(pQuit);
    setButtonPosition(pQuit, btnI);
    pQuit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigQuit();
    });
    connect(this, &Mainwindow::sigQuit, this, &Mainwindow::quitGame, Qt::QueuedConnection);
    btnI++;

    if (Settings::getUsername().isEmpty())
    {
        spDialogTextInput pDialogTextInput = spDialogTextInput::create(tr("Select Username"), false, "");
        addChild(pDialogTextInput);
        connect(pDialogTextInput.get(), &DialogTextInput::sigTextChanged, this, &Mainwindow::changeUsername, Qt::QueuedConnection);
    }

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spLabel pTextfield = spLabel::create(250);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(Mainapp::getGameVersion());
    pTextfield->setPosition(Settings::getWidth() - 10 - pTextfield->getTextRect().getWidth(), Settings::getHeight() - 10 - pTextfield->getTextRect().getHeight());
    pTextfield->addClickListener([=](oxygine::Event*)
    {
        emit sigVersionClicked();
    });
    connect(this, &Mainwindow::sigVersionClicked, this, &Mainwindow::versionClicked, Qt::QueuedConnection);
    addChild(pTextfield);

    if (!Settings::getSmallScreenDevice())
    {
        // import
        oxygine::spButton pImport = ObjectManager::createButton(tr("Import"), buttonWidth, tr("Imports all data from an other Commander Wars release to the current release."));
        addChild(pImport);
        pImport->setPosition(10, Settings::getHeight() - 10 - pImport->getHeight());
        pImport->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigImport();
        });
        connect(this, &Mainwindow::sigImport, this, &Mainwindow::import, Qt::QueuedConnection);
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue obj = pInterpreter->newQObject(this);
    pInterpreter->setGlobal("currentMenu", obj);
    UiFactory::getInstance().createUi("ui/mainmenu.xml", this);

    m_cheatTimeout.setSingleShot(true);
    connect(&m_cheatTimeout, &QTimer::timeout, this, &Mainwindow::cheatTimeout, Qt::QueuedConnection);
    pApp->continueRendering();
}

void Mainwindow::import()
{
    QString path = Settings::getUserPath();
    spFolderDialog folderDialog = spFolderDialog::create(path);
    addChild(folderDialog);
    connect(folderDialog.get(), &FolderDialog::sigFolderSelected, this, &Mainwindow::importFromDirectory, Qt::QueuedConnection);
}

void Mainwindow::importFromDirectory(QString folder)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    QStringList filter;
    filter << "*.*";
    pLoadingScreen->setProgress("Importing data", 0);
    GlobalUtils::importFilesFromDirectory(folder + "/data", "data", filter, false);
    pLoadingScreen->setProgress("Importing maps", 20);
    GlobalUtils::importFilesFromDirectory(folder + "/maps", "maps", filter, false);
    pLoadingScreen->setProgress("Importing mods", 40);
    QStringList modExcludes;
    modExcludes << "aw_styled_icons" << "aw_unloading" << "aw2_damage_formula" << "aw2_image"
                << "awdc_co" << "awdc_flare" << "awdc_powergain" << "awdc_terrain"
                << "awdc_unit" << "awdc_unit_image" << "awdc_weather" << "awds_co"
                << "awds_unit" << "awds_weather" << "cow_debuff_perks" << "cow_resell"
                << "cow_transfer" << "cow_triangle_weapons" << "map_creator" << "coop_mod";
    GlobalUtils::importFilesFromDirectory(folder + "/mods", "mods", filter, false, modExcludes);
    pLoadingScreen->setProgress("Importing custom terrain images", 60);
    GlobalUtils::importFilesFromDirectory(folder + "/customTerrainImages", "customTerrainImages", filter, false);
    pLoadingScreen->setProgress("Importing ini file", 80);
    filter.clear();
    filter << "Commander_Wars.ini";
    GlobalUtils::importFilesFromDirectory(folder + "/", "", filter, true);
    Settings::loadSettings();
    pLoadingScreen->setProgress("Importing userdata", 90);
    filter.clear();
    filter << "*.dat";
    GlobalUtils::importFilesFromDirectory(folder + "/", "", filter, false);
    Userdata::getInstance()->changeUser();
    pLoadingScreen->hide();
}

void Mainwindow::changeUsername(QString name)
{
    Settings::setUsername(name);
    Userdata::getInstance()->changeUser();
    Settings::saveSettings();
}

void Mainwindow::setButtonPosition(oxygine::spButton pButton, qint32 btnI)
{
    static const qint32 buttonCount = 7;
    float buttonHeigth = pButton->getHeight() + 5;
    qint32 col = btnI % 3;
    if (col == 0)
    {
        pButton->setX(Settings::getWidth() / 2.0f - pButton->getWidth() * 1.5f - 10);
    }
    else if (col == 1)
    {
        pButton->setX(Settings::getWidth() / 2.0f - pButton->getWidth() * 0.5f);
    }
    else if (col == 2)
    {
        pButton->setX(Settings::getWidth() / 2.0f + pButton->getWidth() * 0.5f + 10);
    }
    btnI = btnI / 3;
    pButton->setY(Settings::getHeight() / 2.0f - buttonCount  / 2 * buttonHeigth + buttonHeigth * btnI);
}

void Mainwindow::enterSingleplayer()
{    
    Mainapp::getInstance()->pauseRendering();
    auto window = spMapSelectionMapsMenue::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterMultiplayer()
{
    Mainapp::getInstance()->pauseRendering();
    oxygine::Stage::getStage()->addChild(spLobbyMenu::create());
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterEditor()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spEditorMenue::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterOptionmenue()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spOptionMenue::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterWikimenue()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spWikimenu::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterCreditsmenue()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spCreditsMenue::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterAchievementmenue()
{    
    Mainapp::getInstance()->pauseRendering();
    auto window = spAchievementmenu::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterShopMenu()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spShopmenu::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterLoadGame()
{    
    QVector<QString> wildcards;
    wildcards.append("*.sav");
    QString path = Settings::getUserPath() + "savegames";
    spFileDialog saveDialog = spFileDialog::create(path, wildcards);
    addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Mainwindow::loadGame, Qt::QueuedConnection);
}

void Mainwindow::enterLoadCampaign()
{    
    QVector<QString> wildcards;
    wildcards.append("*.camp");
    QString path = Settings::getUserPath() + "savegames";
    spFileDialog saveDialog = spFileDialog::create(path, wildcards);
    addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Mainwindow::loadCampaign, Qt::QueuedConnection);
}

void Mainwindow::loadCampaign(QString filename)
{
    Mainapp::getInstance()->pauseRendering();
    if (filename.endsWith(".camp"))
    {
        QFile file(filename);
        if (file.exists())
        {
            spCampaign pCampaign = spCampaign::create();
            QDataStream stream(&file);
            file.open(QIODevice::ReadOnly);
            pCampaign->deserializeObject(stream);
            spCampaignMenu pMenu = spCampaignMenu::create(pCampaign, false);
            oxygine::Stage::getStage()->addChild(pMenu);
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::loadCampaign not existing savefile was selected");
        }
    }
    else
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::loadCampaign illegal savefile was selected");
    }
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::enterReplayGame()
{
    Mainapp::getInstance()->pauseRendering();
    QVector<QString> wildcards;
    wildcards.append("*.rec");
    QString path = Settings::getUserPath() + "data/records";
    spFileDialog saveDialog = spFileDialog::create(path, wildcards);
    addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Mainwindow::replayGame, Qt::QueuedConnection);
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::lastSaveGame()
{
    loadGame(Settings::getLastSaveGame());
}

void Mainwindow::loadGame(QString filename)
{
    Mainapp::getInstance()->pauseRendering();
    if (filename.endsWith(".sav"))
    {
        QFile file(filename);
        if (file.exists())
        {
            spGameMenue pMenu = spGameMenue::create(filename, true);
            oxygine::Stage::getStage()->addChild(pMenu);
            Mainapp* pApp = Mainapp::getInstance();
            pApp->getAudioThread()->clearPlayList();
            pMenu->startGame();
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::loadGame not existing savefile was selected");
        }
    }
    else
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::loadGame illegal savefile was selected");
    }
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::replayGame(QString filename)
{
    Mainapp::getInstance()->pauseRendering();
    if (filename.endsWith(".rec"))
    {
        QFile file(filename);
        if (file.exists())
        {
            CONSOLE_PRINT("Leaving Main Menue", Console::eDEBUG);
            Mainapp* pApp = Mainapp::getInstance();
            pApp->getAudioThread()->clearPlayList();
            spReplayMenu pMenu = spReplayMenu::create(filename);
            oxygine::Stage::getStage()->addChild(pMenu);
            if (!pMenu->getValid())
            {
                emit pMenu->sigShowRecordInvalid();
            }
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::replayGame not existing record file was selected");
        }
    }
    else
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "Mainwindow::replayGame illegal record file was selected");
    }
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::leaveMenue()
{    
    CONSOLE_PRINT("Leaving Main Menue", Console::eDEBUG);
    oxygine::Actor::detach();
}

void Mainwindow::enterCOStyleMenu()
{
    Mainapp::getInstance()->pauseRendering();
    auto window = spCOStyleMenu::create();
    oxygine::Stage::getStage()->addChild(window);
    leaveMenue();
    Mainapp::getInstance()->continueRendering();
}

void Mainwindow::quitGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->quitGame();
}

void Mainwindow::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "main";
    if (pInterpreter->exists(object, func))
    {
        QJSValueList args;
        QJSValue value = pInterpreter->newQObject(this);
        args << value;
        pInterpreter->doFunction(object, func, args);
    }
}

void Mainwindow::cheatTimeout()
{
    m_cheatCounter = 0;
}

void Mainwindow::versionClicked()
{
    CONSOLE_PRINT("Mainwindow::versionClicked", Console::eDEBUG);
    ++m_cheatCounter;
    m_cheatTimeout.stop();
    if (m_cheatCounter >= 10)
    {
        m_cheatCounter = 0;
        spDialogMessageBox pDialogMessageBox = spDialogMessageBox::create(tr("Do you want to enable all current items in the shop?"), true);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Mainwindow::unlockAllShopItems, Qt::QueuedConnection);
        addChild(pDialogMessageBox);
    }
    else
    {
        m_cheatTimeout.start(400);
    }
}

void Mainwindow::unlockAllShopItems()
{
    Userdata::getInstance()->unlockAllShopItems(true);
}
