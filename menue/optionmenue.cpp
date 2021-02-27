#include <QDir>
#include <QFileInfoList>
#include <QTextStream>
#include <QProcess>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include "menue/optionmenue.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"

#include "game/gamemap.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/slider.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/selectkey.h"
#include "objects/base/spinbox.h"
#include "objects/base/textbox.h"
#include "objects/base/label.h"
#include "objects/base/timespinbox.h"

OptionMenue::OptionMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Option Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("optionmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();


    oxygine::spButton pButtonExit = ObjectManager::createButton(QObject::tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(Settings::getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &OptionMenue::sigExitMenue, this, &OptionMenue::exitMenue, Qt::QueuedConnection);



    oxygine::spButton pButtonMods = ObjectManager::createButton(tr("Mods"));
    pButtonMods->attachTo(this);
    pButtonMods->setPosition(Settings::getWidth() - pButtonMods->getWidth() - 10, 10);
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
    connect(this, &OptionMenue::sigReloadSettings, this, &OptionMenue::reloadSettings, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigWindowLayoutChanged, this, &OptionMenue::reloadSettings, Qt::QueuedConnection);

    oxygine::spButton pButtonGameplayAndKeys = ObjectManager::createButton(tr("Gameplay &amp; Keys"), 220);
    pButtonGameplayAndKeys->attachTo(this);
    pButtonGameplayAndKeys->setPosition(Settings::getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f, 10);
    pButtonGameplayAndKeys->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowGameplayAndKeys();
    });
    connect(this, &OptionMenue::sigShowGameplayAndKeys, this, &OptionMenue::showGameplayAndKeys, Qt::QueuedConnection);
    connect(this, &OptionMenue::sigUpdateModCheckboxes, this, &OptionMenue::updateModCheckboxes, Qt::QueuedConnection);

    QSize size(Settings::getWidth() - 20,
               Settings::getHeight() - static_cast<qint32>(20 + pButtonMods->getHeight()) * 2);
    m_pOptions = new  Panel(true,  size, size);
    m_pOptions->setPosition(10, 20 + pButtonMods->getHeight());
    addChild(m_pOptions);

    m_pGameplayAndKeys = new  GameplayAndKeys(size.height());
    m_pGameplayAndKeys->setPosition(10, 20 + pButtonMods->getHeight());
    addChild(m_pGameplayAndKeys);

    size.setWidth(Settings::getWidth() / 2 - 50);
    m_pMods = new  Panel(true,  size - QSize(0, 50), size);
    m_pMods->setPosition(10, 20 + pButtonMods->getHeight() + 50);
    addChild(m_pMods);
    m_pModDescription = new  Panel(true,  size - QSize(0, 40), size);
    m_pModDescription->setPosition(Settings::getWidth() / 2 + 10, 20 + pButtonMods->getHeight() + 50);
    addChild(m_pModDescription);
    m_ModSelector = new oxygine::Actor();
    m_ModSelector->setPosition(10, 20 + pButtonMods->getHeight());

    addChild(m_ModSelector);
    showSettings();
    pApp->continueRendering();
}

OptionMenue::~OptionMenue()
{
}

void OptionMenue::exitMenue()
{
    
    // save changed settings :)
    Settings::saveSettings();
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
    
}


void OptionMenue::showGameplayAndKeys()
{
    
    m_pOptions->setVisible(false);
    m_pMods->setVisible(false);
    m_pModDescription->setVisible(false);
    m_ModSelector->setVisible(false);
    m_pGameplayAndKeys->setVisible(true);
    
}

void OptionMenue::reloadSettings()
{
    
    Console::print("Leaving Option Menue", Console::eDEBUG);

    OptionMenue* newMenu = new OptionMenue();
    // carry over restart flag
    newMenu->restartNeeded = restartNeeded;
    oxygine::getStage()->addChild(newMenu);
    oxygine::Actor::detach();
    
}

void OptionMenue::showSettings()
{
    
    m_pOptions->setVisible(true);
    m_pMods->setVisible(false);
    m_pModDescription->setVisible(false);
    m_ModSelector->setVisible(false);
    m_pGameplayAndKeys->setVisible(false);
    m_pOptions->clearContent();
    Mainapp* pApp = Mainapp::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
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
                                     QSize(960 ,  640 )};
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize  screenSize = screen->availableSize ();
    supportedSizes.push_front(screenSize);
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
    QVector<QString> displaySizes;
    qint32 currentDisplayMode = 0;
    for  (qint32 i = 0; i < supportedSizes.size(); i++)
    {
        if (supportedSizes[i].width() == Settings::getWidth() &&
            supportedSizes[i].height() == Settings::getHeight())
        {
            currentDisplayMode = i;
        }
        displaySizes.append(QString::number(supportedSizes[i].width()) + " x " + QString::number(supportedSizes[i].height()));
    }
    qint32 sliderOffset = 400;

    spLabel pTextfield = new Label(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(QObject::tr("Screen Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;


    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(QObject::tr("Screen Resolution: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spDropDownmenu pScreenResolution = new DropDownmenu(400, displaySizes);
    pScreenResolution->setPosition(sliderOffset - 130, y);
    pScreenResolution->setCurrentItem(currentDisplayMode);
    pScreenResolution->setTooltipText(tr("Selects the screen resolution for the game"));
    m_pOptions->addItem(pScreenResolution);
    connect(pScreenResolution.get(), &DropDownmenu::sigItemChanged, [=](qint32)
    {
        QStringList itemData = pScreenResolution->getCurrentItemText().split(" x ");
        emit sigChangeScreenSize(itemData[0].toInt(), itemData[1].toInt());
    });
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Screen Mode: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    QVector<QString> items = {tr("Window"), tr("Bordered"), tr("Fullscreen")};
    spDropDownmenu pScreenModes = new DropDownmenu(400, items);
    pScreenModes->setTooltipText(tr("Selects the screen mode for the game"));
    pScreenModes->setPosition(sliderOffset - 130, y);
    pScreenModes->setCurrentItem(pApp->getScreenMode());
    m_pOptions->addItem(pScreenModes);
    connect(pScreenModes.get(), &DropDownmenu::sigItemChanged, pApp, &Mainapp::changeScreenMode, Qt::QueuedConnection);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Brightness: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pSlider = new Slider(Settings::getWidth() - 20 - sliderOffset, -50, 50);
    pSlider->setTooltipText(tr("Selects the brightness for the game"));
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(Settings::getBrightness());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setBrightness(-value);
        pApp->setBrightness(-value);
    });
    m_pOptions->addItem(pSlider);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Gamma: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSlider = new Slider(Settings::getWidth() - 20 - sliderOffset, 1, 160, "");
    pSlider->setTooltipText(tr("Selects the gamma factor for the game"));
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(Settings::getGamma() * 30.0f);
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setGamma(value / 30.0f);
        pApp->setGamma(value / 30.0f);
    });
    m_pOptions->addItem(pSlider);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Sprite Aliasing: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spCheckbox pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked ingame sprites will be aliased smoother."));
    pCheckbox->setChecked(Settings::getSpriteFilter() != GL_NEAREST);
    pCheckbox->setPosition(sliderOffset - 130, y);
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        if (value)
        {
            Settings::setSpriteFilter(GL_LINEAR_MIPMAP_LINEAR);
            pApp->applyFilter(GL_LINEAR_MIPMAP_LINEAR );
        }
        else
        {
            Settings::setSpriteFilter(GL_NEAREST);
            pApp->applyFilter(GL_NEAREST);
        }
    });
    m_pOptions->addItem(pCheckbox);
    y += 40;

    showSoundOptions(m_pOptions, sliderOffset, y);

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Language: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    items.clear();
    QLocale english("en");
    items.append(english.nativeLanguageName());
    languages.append("en");
    qint32 current = 0;
    QString path =  QCoreApplication::applicationDirPath() + "/resources/translation/";
    QStringList filter;
    filter << "*.qm";
    QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString lang = dirIter.fileName().replace(".qm", "").replace("lang_", "");
        if (lang != "en")
        {
            languages.append(lang);
            QLocale langLoc(lang);
            items.append(langLoc.nativeLanguageName());
            if (lang == Settings::getLanguage())
            {
                current = items.size() - 1;
            }
        }
    }
    spDropDownmenu pLanguageMenu = new DropDownmenu(400, items);
    pLanguageMenu->setTooltipText(tr("Selects the language for the game. Note: Not everything may be translated."));
    pLanguageMenu->setPosition(sliderOffset - 130, y);
    pLanguageMenu->setCurrentItem(current);
    m_pOptions->addItem(pLanguageMenu);
    connect(pLanguageMenu.get(), &DropDownmenu::sigItemChanged, pApp,
            [=](qint32 item)
    {
        Console::print("Marking restart cause language changed.", Console::eDEBUG);
        Settings::setLanguage(languages[item]);
        restartNeeded = true;
        emit sigReloadSettings();
    });
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Auto Saving Time: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spTimeSpinBox autoSavingCycleTime = new TimeSpinBox(200);
    autoSavingCycleTime->setTooltipText(tr("Selects the auto saving cycle in hours:minutes:seconds"));
    autoSavingCycleTime->setCurrentValue(std::chrono::duration_cast<std::chrono::milliseconds>(Settings::getAutoSavingCylceTime()).count());
    autoSavingCycleTime->setPosition(sliderOffset - 130, y);
    connect(autoSavingCycleTime.get(), &TimeSpinBox::sigValueChanged, [=](qint32 value)
    {
        Settings::setAutoSavingCylceTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(value)));
    });
    m_pOptions->addItem(autoSavingCycleTime);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Auto Saving Cycle: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSpinBox autoSavingCycle = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    autoSavingCycle->setTooltipText(tr("Selects the amount of auto save games that get cycled through while auto saving. A value 0 disables this feature."));
    autoSavingCycle->setCurrentValue(Settings::getAutoSavingCycle());
    autoSavingCycle->setPosition(sliderOffset - 130, y);
    connect(autoSavingCycle.get(), &SpinBox::sigValueChanged, Settings::getInstance(), &Settings::setAutoSavingCycle);
    m_pOptions->addItem(autoSavingCycle);
    y += 40;


    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Record Games: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("If checked games will be recorded and you can rewatch them in the replay section."));
    pCheckbox->setChecked(Settings::getRecord());
    pCheckbox->setPosition(sliderOffset - 130, y);
    connect(pCheckbox.get(), &Checkbox::checkChanged, Settings::getInstance(), &Settings::setRecord, Qt::QueuedConnection);
    m_pOptions->addItem(pCheckbox);
    y += 40;

    pTextfield = new Label(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Network Settings"));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Username: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spTextbox pTextbox = new Textbox(Settings::getWidth() - 20 - sliderOffset);
    pTextbox->setTooltipText(tr("Selects your username shown at various places of the game"));
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
                Console::print("Marking restart cause user changed.", Console::eDEBUG);
                restartNeeded = true;
            }
            Settings::setUsername(value);
        }
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Server Adress: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pTextbox = new Textbox(Settings::getWidth() - 20 - sliderOffset);
    pTextbox->setTooltipText(tr("Selects the game server you wan't to connect to when playing a multiplayer game."));
    pTextbox->setCurrentText(Settings::getServerAdress());
    connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString value)
    {
        Settings::setServerAdress(value);
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Lobby port: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSpinBox portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setTooltipText(tr("Selects the chat port for used to chat with the server"));
    portBox->setCurrentValue(Settings::getServerPort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setServerPort(static_cast<quint16>(value));
    });
    m_pOptions->addItem(portBox);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Server: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pCheckbox = new Checkbox();
    pCheckbox->setTooltipText(tr("Enables this game as global server."));
    pCheckbox->setChecked(Settings::getServer());
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        Console::print("Marking restart cause server settings changed.", Console::eDEBUG);
        Settings::setServer(value);
        restartNeeded = true;
    });
    pCheckbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pCheckbox);
    y += 40;

    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Game port: "));
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setTooltipText(tr("Selects the game port for used to play the game with the server"));
    portBox->setCurrentValue(Settings::getGamePort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setGamePort(static_cast<quint16>(value));
    });
    m_pOptions->addItem(portBox);
    y += 40;

    m_pOptions->setContentHeigth(20 + y);
}

void OptionMenue::showSoundOptions(spPanel pOwner, qint32 sliderOffset, qint32 & y)
{
    AudioThread* pAudio = Mainapp::getInstance()->getAudioThread();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spLabel pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Audio Settings"));
    pTextfield->setPosition(10, y);
    pOwner->addItem(pTextfield);
    y += 40;
    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Global Volume: "));
    pTextfield->setPosition(10, y);
    pOwner->addItem(pTextfield);
    spSlider pSlider = new Slider(Settings::getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setTooltipText(tr("Selects the global volume for the game"));
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(Settings::getTotalVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setTotalVolume(value);
        pAudio->setVolume(Settings::getMusicVolume());
    });
    pOwner->addItem(pSlider);

    y += 40;
    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Music Volume: "));
    pTextfield->setPosition(10, y);
    pOwner->addItem(pTextfield);
    pSlider = new Slider(Settings::getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setTooltipText(tr("Selects the music volume for the game"));
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(Settings::getMusicVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setMusicVolume(value);
        pAudio->setVolume(value);
    });
    pOwner->addItem(pSlider);

    y += 40;
    pTextfield = new Label(sliderOffset - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Sound Volume: "));
    pTextfield->setPosition(10, y);
    pOwner->addItem(pTextfield);
    pSlider = new Slider(Settings::getWidth() - 20 - sliderOffset, 0, 100);
    pSlider->setTooltipText(tr("Selects the sound volume for the game"));
    pSlider->setPosition(sliderOffset - 130, y);
    pSlider->setCurrentValue(Settings::getSoundVolume());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setSoundVolume(value);
    });
    pOwner->addItem(pSlider);
    y += 40;
}

void OptionMenue::showMods()
{
    
    m_pMods->clearContent();
    m_pModDescription->clearContent();
    m_ModBoxes.clear();
    m_ModCheckboxes.clear();

    m_pOptions->setVisible(false);
    m_pMods->setVisible(true);
    m_pModDescription->setVisible(true);
    m_ModSelector->setVisible(true);
    m_pGameplayAndKeys->setVisible(false);
    m_ModSelector->removeChildren();


    QFileInfoList infoList = QDir("mods").entryInfoList(QDir::Dirs);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_ModDescriptionText = new oxygine::TextField();
    m_ModDescriptionText->setStyle(style);
    m_ModDescriptionText->setSize(m_pModDescription->getContentWidth() - 60, 500);
    m_pModDescription->addItem(m_ModDescriptionText);

    spLabel pLabel = new Label(250);
    style.multiline = false;
    pLabel->setStyle(style);
    pLabel->setText(QObject::tr("Advance Wars Game:"));
    m_ModSelector->addChild(pLabel);
    QVector<QString> versions = {QObject::tr("Unkown"),
                                 QObject::tr("Commander Wars"),
                                 QObject::tr("Advance Wars DS"),
                                 QObject::tr("Advance Wars DC")};
    spDropDownmenu pModSelection = new DropDownmenu(300, versions);
    pModSelection->setTooltipText(tr("Select an Advance Wars Game to preselect all mods which are required to play like this Advance Wars Game"));
    pModSelection->setX(260);
    connect(pModSelection.get(), &DropDownmenu::sigItemChanged, this, &OptionMenue::selectMods, Qt::QueuedConnection);
    m_ModSelector->addChild(pModSelection);

    qint32 width = 0;
    qint32 mods = 0;
    QStringList currentMods = Settings::getMods();
    style.multiline = false;
    for (const auto & info : infoList)
    {
        QString folder = info.filePath();
        if (!folder.endsWith("."))
        {
            QString name;
            QString description;
            QString version;
            QStringList compatibleMods;
            QStringList incompatibleMods;
            QStringList requiredMods;
            bool isComsetic = false;
            Settings::getModInfos(folder, name, description, version,
                                  compatibleMods, incompatibleMods, requiredMods, isComsetic);
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
            m_ModCheckboxes.append(modCheck);
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
                    Settings::addMod(folder);
                }
                else
                {
                    Settings::removeMod(folder);
                }
                restartNeeded = true;
                emit sigUpdateModCheckboxes();
            });
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
                QString cosmeticInfo;
                if (isComsetic)
                {
                    cosmeticInfo = QString("\n\n") + tr("The mod is claimed to be pure cosmetic by the creator and may be used during multiplayer games based on the game rules.");
                }
                QString modInfo = "\n\n" + tr("Compatible Mods:\n");
                for (const auto & mod : compatibleMods)
                {
                    modInfo += Settings::getModName(mod) + "\n";
                }
                modInfo += "\n" + tr("Incompatible Mods:\n");
                for (const auto & mod : incompatibleMods)
                {
                    modInfo += Settings::getModName(mod) + "\n";
                }
                modInfo += "\n" + tr("Required Mods:\n");
                for (const auto & mod : requiredMods)
                {
                    modInfo += Settings::getModName(mod) + "\n";
                }
                m_ModDescriptionText->setHtmlText(description + cosmeticInfo + modInfo + "\n\n" + tr("Version: ") + version);
                m_pModDescription->setContentHeigth(m_ModDescriptionText->getTextRect().getHeight() + 40);
            });
            m_ModBoxes.append(pBox);
            m_pMods->addItem(pBox);
            mods++;
        }
    }
    m_pMods->setContentWidth(width);
    m_pMods->setContentHeigth(50 + mods * 50);
    
}

void OptionMenue::selectMods(qint32 item)
{
    
    QStringList removeList;
    QStringList addList;
    switch (item)
    {
        case 1:
        {
            removeList.append("mods/aw_unloading");
            removeList.append("mods/aw2_damage_formula");
            removeList.append("mods/awds_unit");
            removeList.append("mods/awds_co");
            removeList.append("mods/awds_weather");
            removeList.append("mods/awdc_co");
            removeList.append("mods/awdc_unit");
            removeList.append("mods/awdc_powergain");
            removeList.append("mods/awdc_weather");
            removeList.append("mods/awdc_terrain");
            removeList.append("mods/awdc_flare");
            removeList.append("map_creator");
            removeList.append("coop_mod");
            break;
        }
        case 2:
        {
            addList.append("mods/aw_unloading");
            addList.append("mods/aw2_damage_formula");
            addList.append("mods/awds_unit");
            addList.append("mods/awds_co");
            addList.append("mods/awds_weather");
            removeList.append("mods/awdc_co");
            removeList.append("mods/awdc_unit");
            removeList.append("mods/awdc_powergain");
            removeList.append("mods/awdc_weather");
            removeList.append("mods/awdc_terrain");
            removeList.append("mods/awdc_flare");
            removeList.append("map_creator");
            removeList.append("coop_mod");
            break;
        }
        case 3:
        {
            addList.append("mods/aw_unloading");
            removeList.append("mods/aw2_damage_formula");
            removeList.append("mods/awds_unit");
            removeList.append("mods/awds_co");
            removeList.append("mods/awds_weather");
            removeList.append("map_creator");
            removeList.append("coop_mod");
            addList.append("mods/awdc_co");
            addList.append("mods/awdc_unit");
            addList.append("mods/awdc_powergain");
            addList.append("mods/awdc_weather");
            addList.append("mods/awdc_terrain");
            addList.append("mods/awdc_flare");
            break;
        }
    }
    for (auto & removeMod : removeList)
    {
        Settings::removeMod(removeMod);
    }
    for (auto & addMod : addList)
    {
        Settings::addMod(addMod);
    }
    Console::print("Marking restart cause mods changed.", Console::eDEBUG);
    restartNeeded = true;
    showMods();
    
}

void OptionMenue::restart()
{
    Console::print("Forcing restart to reload required data changed in the options.", Console::eDEBUG);
    QCoreApplication::exit(1);
}

void OptionMenue::updateModCheckboxes()
{
    const auto mods = Settings::getActiveMods();
    QFileInfoList infoList = QDir("mods").entryInfoList(QDir::Dirs);

    qint32 i = 0;
    for (const auto & info : infoList)
    {
        QString mod = info.filePath();
        if (!mod.endsWith("."))
        {
            QString name;
            QString description;
            QString version;
            QStringList compatibleMods;
            QStringList incompatibleMods;
            QStringList requiredMods;
            bool isComsetic = false;
            Settings::getModInfos(mod, name, description, version,
                                  compatibleMods, incompatibleMods, requiredMods, isComsetic);
            bool enabled = true;
            for (const auto & incompatibleMod : incompatibleMods)
            {
                if (mods.contains(incompatibleMod))
                {
                    enabled = false;
                    break;
                }
            }
            if (enabled)
            {
                for (const auto & requiredMod : requiredMods)
                {
                    if (!mods.contains(requiredMod))
                    {
                        enabled = false;
                        break;
                    }
                }
            }
            m_ModCheckboxes[i]->setEnabled(enabled);
            ++i;
        }
    }
}
