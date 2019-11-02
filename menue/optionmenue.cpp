#include "menue/optionmenue.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include "game/gamemap.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/checkbox.h"
#include "objects/slider.h"
#include "objects/dropdownmenu.h"
#include "objects/selectkey.h"
#include "objects/spinbox.h"
#include "objects/textbox.h"

#include <QDir>
#include <QFileInfoList>
#include <QTextStream>
#include <QProcess>

#include <qguiapplication.h>
#include <qscreen.h>

OptionMenue::OptionMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Option Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();


    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(pApp->getSettings()->getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f,
                             pApp->getSettings()->getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &OptionMenue::sigExitMenue, this, &OptionMenue::exitMenue, Qt::QueuedConnection);



    oxygine::spButton pButtonMods = ObjectManager::createButton(tr("Mods"));
    pButtonMods->attachTo(this);
    pButtonMods->setPosition(pApp->getSettings()->getWidth() - pButtonMods->getWidth() - 10, 10);
    pButtonMods->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowMods();
    });
    connect(this, &OptionMenue::sigShowMods, this, &OptionMenue::showMods, Qt::QueuedConnection);

    oxygine::spButton pButtonSettings = ObjectManager::createButton(tr("Settings"));
    pButtonSettings->attachTo(this);
    pButtonSettings->setPosition(10, 10);
    pButtonSettings->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowSettings();
    });
    connect(this, &OptionMenue::sigShowSettings, this, &OptionMenue::showSettings, Qt::QueuedConnection);
    connect(this, &OptionMenue::sigChangeScreenSize, pApp, &Mainapp::changeScreenSize, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigWindowLayoutChanged, this, &OptionMenue::reloadSettings, Qt::QueuedConnection);

    oxygine::spButton pButtonGameplayAndKeys = ObjectManager::createButton(tr("Gameplay &amp; Keys"));
    pButtonGameplayAndKeys->attachTo(this);
    pButtonGameplayAndKeys->setPosition(pApp->getSettings()->getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f, 10);
    pButtonGameplayAndKeys->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGameplayAndKeys();
    });
    connect(this, &OptionMenue::sigShowGameplayAndKeys, this, &OptionMenue::showGameplayAndKeys, Qt::QueuedConnection);

    QSize size(pApp->getSettings()->getWidth() - 20,
               pApp->getSettings()->getHeight() - static_cast<qint32>(20 + pButtonMods->getHeight()) * 2);
    m_pOptions = new  Panel(true,  size, size);
    m_pOptions->setPosition(10, 20 + pButtonMods->getHeight());
    addChild(m_pOptions);

    size.setWidth(pApp->getSettings()->getWidth() / 2 - 40);
    m_pMods = new  Panel(true,  size, size);
    m_pMods->setPosition(10, 20 + pButtonMods->getHeight());
    addChild(m_pMods);
    m_pModDescription = new  Panel(true,  size, size);
    m_pModDescription->setPosition(pApp->getSettings()->getWidth() / 2 + 10, 20 + pButtonMods->getHeight());
    addChild(m_pModDescription);

    showSettings();
}

void OptionMenue::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // save changed settings :)
    pApp->getSettings()->saveSettings();
    if (restartNeeded)
    {
        restart();
    }
    else
    {
        Console::print("Leaving Option Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Mainwindow());
        oxygine::Actor::detach();
    }
    pApp->continueThread();
}


void OptionMenue::showGameplayAndKeys()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pOptions->clearContent();
    m_pMods->clearContent();
    m_pModDescription->clearContent();
    m_pOptions->setVisible(true);
    m_pMods->setVisible(false);
    m_pModDescription->setVisible(false);
    Settings* pSettings = pApp->getSettings();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    qint32 sliderOffset = 400;

    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Gameplay Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Show Ingame Animations: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    QVector<QString> items = {tr("None"), tr("All"), tr("Own"), tr("Ally"), tr("Enemy"),
                             tr("Only Detailed Battle All"), tr("Only Detailed Battle Own"),
                             tr("Only Detailed Battle Ally"), tr("Only Detailed Battle Enemy")};
    spDropDownmenu pAnimationMode = new DropDownmenu(400, items);
    pAnimationMode->setCurrentItem(static_cast<qint32>(pSettings->getShowAnimations()));
    pAnimationMode->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pAnimationMode);
    connect(pAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        pSettings->setShowAnimations(static_cast<GameEnums::AnimationMode>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    items = {tr("Detailed"), tr("Overworld")};
    spDropDownmenu pBattleAnimationMode = new DropDownmenu(400, items);
    pBattleAnimationMode->setCurrentItem(static_cast<qint32>(pSettings->getBattleAnimations()));
    pBattleAnimationMode->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pBattleAnimationMode);
    connect(pBattleAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        pSettings->setBattleAnimations(static_cast<GameEnums::BattleAnimationMode>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Animation Speed: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pAnimationSpeed = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 1, 100, "");
    pAnimationSpeed->setPosition(sliderOffset - 130, y);
    pAnimationSpeed->setCurrentValue(static_cast<qint32>(pSettings->getAnimationSpeedValue()));
    m_pOptions->addItem(pAnimationSpeed);
    connect(pAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animation Speed: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pBattleAnimationSpeed = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 1, 100, "");
    pBattleAnimationSpeed->setPosition(sliderOffset - 130, y);
    pBattleAnimationSpeed->setCurrentValue(static_cast<qint32>(pSettings->getBattleAnimationSpeedValue()));
    m_pOptions->addItem(pBattleAnimationSpeed);
    connect(pBattleAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setBattleAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Multi Turn Plantime: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pMultiTurnCounter = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 1, 10, "");
    pMultiTurnCounter->setPosition(sliderOffset - 130, y);
    pMultiTurnCounter->setCurrentValue(static_cast<qint32>(pSettings->getMultiTurnCounter()));
    m_pOptions->addItem(pMultiTurnCounter);
    connect(pMultiTurnCounter.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setMultiTurnCounter(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Auto End Turn: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setChecked(Settings::getAutoEndTurn());
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        Settings::setAutoEndTurn(value);
    });
    pCheckbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pCheckbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Menu Heigth: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pMenuItemHeigth = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 10, (Settings::getHeight() - GameMap::Imagesize * 2) / GameMap::Imagesize, "");
    pMenuItemHeigth->setPosition(sliderOffset - 130, y);
    pMenuItemHeigth->setCurrentValue(static_cast<qint32>(pSettings->getMenuItemCount()));
    m_pOptions->addItem(pMenuItemHeigth);
    connect(pMenuItemHeigth.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setMenuItemCount(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Show Windows Cursor: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pCheckbox = new Checkbox();
    pCheckbox->setChecked(Settings::getShowCursor());
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        Settings::setShowCursor(value);
    });
    pCheckbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pCheckbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Ingame Keys"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Up: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSelectKey pSelectKey = new SelectKey(Settings::getKey_up());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_up, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Left: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_left());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_left, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Down: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_down());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_down, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Right: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_right());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_right, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Confirm: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_confirm());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_confirm, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Cancel: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_cancel());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_cancel, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Next: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_next());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_next, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Previous: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_previous());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_previous, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Quicksave 1: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quicksave1());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quicksave1, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Quickload 1: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quickload1());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quickload1, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Quicksave 2: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quicksave2());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quicksave2, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Quickload 2: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quickload2());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quickload2, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Information: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_information());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_information, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Move Map Up: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_moveMapUp());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_moveMapUp, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Move Map Left: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_moveMapLeft());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_moveMapLeft, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Move Map Down: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_moveMapDown());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_moveMapDown, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Key Move Map Right: "));
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_moveMapRight());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_moveMapRight, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    m_pOptions->setContentHeigth(20 + y);
    pApp->continueThread();
}

void OptionMenue::reloadSettings()
{
    Console::print("Leaving Option Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new OptionMenue());
    oxygine::Actor::detach();
}

void OptionMenue::showSettings()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pOptions->clearContent();
    m_pMods->clearContent();
    m_pModDescription->clearContent();
    m_pOptions->setVisible(true);
    m_pMods->setVisible(false);
    m_pModDescription->setVisible(false);

    AudioThread* pAudio = pApp->getAudioThread();
    Settings* pSettings = pApp->getSettings();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;


    // cache all supported display modes
    // we're cheating here little bit since qt offers no way to get the screen resolutions from the hardware driver
    QVector<QSize> supportedSizes = {QSize(15360, 8640),
                                     QSize(8192 , 8192),
                                     QSize(10240, 4320),
                                     QSize(8192 , 4608),
                                     QSize(8192 , 4320),
                                     QSize(7680 , 4800),
                                     QSize(7680 , 4320),
                                     QSize(6400 , 4800),
                                     QSize(6400 , 4096),
                                     QSize(5120 , 4096),
                                     QSize(5120 , 3200),
                                     QSize(5120 , 2880),
                                     QSize(5120 , 2160),
                                     QSize(4500 , 3000),
                                     QSize(4096 , 3072),
                                     QSize(4096 , 2160),
                                     QSize(3840 , 2400),
                                     QSize(3840 , 2160),
                                     QSize(3840 , 1600),
                                     QSize(3440 , 1440),
                                     QSize(3240 , 2160),
                                     QSize(3200 , 2400),
                                     QSize(3200 , 2048),
                                     QSize(3200 , 1800),
                                     QSize(3000 , 2000),
                                     QSize(2960 , 1440),
                                     QSize(2880 , 1800),
                                     QSize(2880 , 1620),
                                     QSize(2880 , 1440),
                                     QSize(2880 , 900 ),
                                     QSize(2800 , 2100),
                                     QSize(2736 , 1824),
                                     QSize(2732 , 2048),
                                     QSize(2560 , 2048),
                                     QSize(2560 , 1920),
                                     QSize(2560 , 1800),
                                     QSize(2560 , 1700),
                                     QSize(2560 , 1600),
                                     QSize(2560 , 1440),
                                     QSize(2560 , 1080),
                                     QSize(2538 , 1080),
                                     QSize(2436 , 1125),
                                     QSize(2304 , 1728),
                                     QSize(2256 , 1504),
                                     QSize(2304 , 1440),
                                     QSize(2280 , 1080),
                                     QSize(2160 , 1440),
                                     QSize(2160 , 1200),
                                     QSize(2160 , 1080),
                                     QSize(2048 , 1536),
                                     QSize(2048 , 1280),
                                     QSize(2048 , 1152),
                                     QSize(2048 , 1080),
                                     QSize(1920 , 1440),
                                     QSize(1920 , 1400),
                                     QSize(1920 , 1280),
                                     QSize(1920 , 1200),
                                     QSize(1920 , 1080),
                                     QSize(1856 , 1392),
                                     QSize(1800 , 1440),
                                     QSize(1792 , 1344),
                                     QSize(1776 , 1000),
                                     QSize(1680 , 1050),
                                     QSize(1600 , 1280),
                                     QSize(1600 , 1200),
                                     QSize(1600 , 1024),
                                     QSize(1600 , 900 ),
                                     QSize(1600 , 768 ),
                                     QSize(1440 , 1440),
                                     QSize(1440 , 1080),
                                     QSize(1440 , 1024),
                                     QSize(1440 , 960 ),
                                     QSize(1440 , 900 ),
                                     QSize(1400 , 1050),
                                     QSize(1366 , 768 ),
                                     QSize(1334 , 750 ),
                                     QSize(1280 , 1024),
                                     QSize(1280 , 960 ),
                                     QSize(1280 , 854 ),
                                     QSize(1280 , 800 ),
                                     QSize(1280 , 768 ),
                                     QSize(1280 , 720 ),
                                     QSize(1152 , 900 ),
                                     QSize(1152 , 864 ),
                                     QSize(1152 , 768 ),
                                     QSize(1152 , 720 ),
                                     QSize(1136 , 640 ),
                                     QSize(1120 , 832 ),
                                     QSize(1080 , 1200),
                                     QSize(1024 , 1024),
                                     QSize(1024 , 800 ),
                                     QSize(1024 , 768 ),
                                     QSize(1024 , 640 ),
                                     QSize(1024 , 600 ),
                                     QSize(960 ,  720 ),
                                     QSize(960 ,  640 ),
                                     QSize(800 ,  600 )};
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize  screenSize = screen->availableSize ();
    if (Settings::getFullscreen())
    {
        supportedSizes.clear();
        supportedSizes.append(screenSize);
    }
    else
    {
        qint32 count = 0;
        while  (count < supportedSizes.size())
        {
            if (supportedSizes[count].width() <= screenSize.width() &&
                supportedSizes[count].height() <= screenSize.height())
            {
                count++;
            }
            else
            {
                supportedSizes.removeAt(count);
            }
        }
    }
    QVector<QString> displaySizes;
    qint32 currentDisplayMode = 0;
    for  (qint32 i = 0; i < supportedSizes.size(); i++)
    {
        if (supportedSizes[i].width() == pSettings->getWidth() &&
            supportedSizes[i].height() == pSettings->getHeight())
        {
            currentDisplayMode = i;
        }
        displaySizes.append(QString::number(supportedSizes[i].width()) + " x " + QString::number(supportedSizes[i].height()));
    }
    qint32 sliderOffset = 400;

    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Screen Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;


    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Screen Resolution: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spDropDownmenu pScreenResolution = new DropDownmenu(400, displaySizes);
    pScreenResolution->setPosition(sliderOffset - 130, y);
    pScreenResolution->setCurrentItem(currentDisplayMode);
    m_pOptions->addItem(pScreenResolution);
    connect(pScreenResolution.get(), &DropDownmenu::sigItemChanged, [=](qint32)
    {
        QStringList itemData = pScreenResolution->getCurrentItemText().split(" x ");
        emit sigChangeScreenSize(itemData[0].toInt(), itemData[1].toInt());
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Screen Mode: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    QVector<QString> items = {tr("Window"), tr("Bordered"), tr("Fullscreen")};
    spDropDownmenu pScreenModes = new DropDownmenu(400, items);
    pScreenModes->setPosition(sliderOffset - 130, y);
    pScreenModes->setCurrentItem(pApp->getScreenMode());
    m_pOptions->addItem(pScreenModes);
    connect(pScreenModes.get(), &DropDownmenu::sigItemChanged, pApp, &Mainapp::changeScreenMode, Qt::QueuedConnection);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Audio Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Global Volume: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pSlider = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(pSettings->getTotalVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setTotalVolume(value);
        pAudio->setVolume(pSettings->getMusicVolume());
    });
    m_pOptions->addItem(pSlider);

    y += 40;
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Music Volume: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSlider = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(pSettings->getMusicVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setMusicVolume(value);
        pAudio->setVolume(value);
    });
    m_pOptions->addItem(pSlider);

    y += 40;
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Sound Volume: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSlider = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(pSettings->getSoundVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setSoundVolume(value);
    });
    m_pOptions->addItem(pSlider);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Network Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Username: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spTextbox pTextbox = new Textbox(pApp->getSettings()->getWidth() - 20 - sliderOffset);
    pTextbox->setCurrentText(Settings::getUsername());
    connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString value)
    {
        if (value.isEmpty())
        {
           pTextbox->setCurrentText(Settings::getUsername());
        }
        else
        {
            if (value != Settings::getUsername())
            {
                restartNeeded = true;
            }
            Settings::setUsername(value);
        }
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Server Adress: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pTextbox = new Textbox(pApp->getSettings()->getWidth() - 20 - sliderOffset);
    pTextbox->setCurrentText(Settings::getServerAdress());
    connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString value)
    {
        Settings::setServerAdress(value);
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Lobby port: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSpinBox portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setCurrentValue(Settings::getServerPort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setServerPort(static_cast<quint16>(value));
    });
    m_pOptions->addItem(portBox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Server: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setChecked(Settings::getServer());
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        Settings::setServer(value);
        restartNeeded = true;
    });
    pCheckbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pCheckbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Game port: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setCurrentValue(Settings::getGamePort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setGamePort(static_cast<quint16>(value));
    });
    m_pOptions->addItem(portBox);
    y += 40;

    m_pOptions->setContentHeigth(20 + y);
    pApp->continueThread();
}

void OptionMenue::showMods()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pOptions->clearContent();
    m_pMods->clearContent();
    m_pModDescription->clearContent();
    m_pOptions->setVisible(false);
    m_pMods->setVisible(true);
    m_pModDescription->setVisible(true);

    QFileInfoList infoList = QDir("mods").entryInfoList(QDir::Dirs);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_ModDescriptionText = new oxygine::TextField();
    m_ModDescriptionText->setStyle(style);
    m_ModDescriptionText->setSize(m_pModDescription->getContentWidth() - 40, 500);
    m_pModDescription->addItem(m_ModDescriptionText);

    qint32 width = 0;
    qint32 mods = 0;
    Settings* pSettings = Mainapp::getInstance()->getSettings();
    QStringList currentMods = pSettings->getMods();
    style.multiline = false;

    for (qint32 i = 0; i < infoList.size(); i++)
    {
        QString folder = infoList[i].filePath();
        if (!folder.endsWith("."))
        {
            QString name = folder;
            QString description;
            QFile file(folder + "/mod.txt");
            if (file.exists())
            {
                file.open(QFile::ReadOnly);
                QTextStream stream(&file);
                while (!stream.atEnd())
                {
                    QString line = stream.readLine();
                    if (line.startsWith("name="))
                    {
                        name = line.split("=")[1];
                    }
                    if (line.startsWith("description="))
                    {
                        description = line.split("=")[1];
                    }
                }
            }
            oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
            oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
            pBox->setResAnim(pAnim);
            pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);

            oxygine::spTextField pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(name);
            pTextfield->setPosition(50, 5);
            pBox->addChild(pTextfield);
            qint32 curWidth = pTextfield->getTextRect().getWidth() + 30;
            spCheckbox modCheck = new Checkbox();
            modCheck->setPosition(10, 5);
            pBox->addChild(modCheck);
            curWidth += modCheck->getWidth() + 10;
            if (currentMods.contains(folder))
            {
                modCheck->setChecked(true);
            }
            connect(modCheck.get(), &Checkbox::checkChanged, [=](bool checked)
            {
                if (checked)
                {
                    pSettings->addMod(folder);
                }
                else
                {
                    pSettings->removeMod(folder);
                }
                restartNeeded = true;
            });
            mods++;
            if (curWidth > width)
            {
                width = curWidth;
            }
            pBox->setPosition(10, 10 + mods * 50);
            pBox->setSize(curWidth + 20, 50);

            pBox->addClickListener([=](oxygine::Event* pEvent)
            {
                pEvent->stopPropagation();
                for (qint32 i2 = 0; i2 < m_ModBoxes.size(); i2++)
                {
                    m_ModBoxes[i2]->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
                }
                pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
                m_ModDescriptionText->setHtmlText(description);
                m_pModDescription->setContentHeigth(m_ModDescriptionText->getTextRect().getHeight() + 40);
            });
            m_ModBoxes.append(pBox);
            m_pMods->addItem(pBox);
        }
    }
    m_pMods->setContentWidth(width);
    m_pMods->setContentHeigth(20 + mods * 50);
    pApp->continueThread();
}

void OptionMenue::restart()
{
    QCoreApplication::exit(1);
}
