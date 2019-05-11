#include "menue/optionmenue.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

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
    connect(this, &OptionMenue::sigChangeScreenSize, this, &OptionMenue::changeScreenSize, Qt::QueuedConnection);

    oxygine::spButton pButtonGameplayAndKeys = ObjectManager::createButton(tr("Gameplay & Keys"));
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

void OptionMenue::changeScreenMode(qint32 mode)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (mode)
    {
        case 1:
        {
            SDL_SetWindowBordered(oxygine::core::getWindow(), SDL_bool::SDL_FALSE);
            SDL_SetWindowFullscreen(oxygine::core::getWindow(), 0);
            pApp->getSettings()->setFullscreen(false);
            pApp->getSettings()->setBorderless(true);
            break;
        }
        case 2:
        {
            SDL_SetWindowFullscreen(oxygine::core::getWindow(), SDL_WINDOW_FULLSCREEN);
            pApp->getSettings()->setFullscreen(true);
            pApp->getSettings()->setBorderless(true);
            break;
        }
        default:
        {
            SDL_SetWindowBordered(oxygine::core::getWindow(), SDL_bool::SDL_TRUE);
            SDL_SetWindowFullscreen(oxygine::core::getWindow(), 0);
            pApp->getSettings()->setFullscreen(false);
            pApp->getSettings()->setBorderless(false);
        }
    }
    pApp->continueThread();
}

void OptionMenue::changeScreenSize(qint32 width, qint32 heigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    SDL_SetWindowSize(oxygine::core::getWindow(), width, heigth);
    pApp->getSettings()->setWidth(width);
    pApp->getSettings()->setHeight(heigth);
    pApp->getSettings()->saveSettings();
    Console::print("Leaving Option Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new OptionMenue());
    oxygine::Actor::detach();
    pApp->continueThread();
}

void OptionMenue::showGameplayAndKeys()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pOptions->clearContent();
    Settings* pSettings = pApp->getSettings();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    qint32 sliderOffset = 400;

    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Gameplay Settings").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Show Ingame Animations: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    QVector<QString> items = {tr("None"), tr("All"), tr("Own"), tr("Ally"), tr("Enemy")};
    spDropDownmenu pAnimationMode = new DropDownmenu(300, items);
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
    pTextfield->setText(tr("Animation Speed: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSlider pAnimationSpeed = new Slider(pApp->getSettings()->getWidth() - 20 - sliderOffset, 1, 100, "");
    pAnimationSpeed->setPosition(sliderOffset - 130, y);
    pAnimationSpeed->setCurrentValue(static_cast<qint32>(pSettings->getAnimationSpeed()));
    m_pOptions->addItem(pAnimationSpeed);
    connect(pAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        pSettings->setAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Show Windows Cursor: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spCheckbox pCheckbox = new Checkbox();
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
    pTextfield->setText(tr("Ingame Keys").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Up: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSelectKey pSelectKey = new SelectKey(Settings::getKey_up());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_up, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Left: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_left());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_left, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Down: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_down());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_down, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Right: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_right());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_right, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Confirm: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_confirm());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_confirm, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Cancel: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_cancel());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_cancel, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Next: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_next());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_next, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Previous: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pSelectKey = new SelectKey(Settings::getKey_previous());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_previous, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Quicksave 1: ").toStdString().c_str());
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quicksave1());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quicksave1, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Quickload 1: ").toStdString().c_str());
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quickload1());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quickload1, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Quicksave 2: ").toStdString().c_str());
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quicksave2());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quicksave2, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Key Quickload 2: ").toStdString().c_str());
    m_pOptions->addItem(pTextfield);
    pTextfield->setPosition(10, y);
    pSelectKey = new SelectKey(Settings::getKey_quickload2());
    pSelectKey->setPosition(sliderOffset - 130, y);
    connect(pSelectKey.get(), &SelectKey::sigKeyChanged, pSettings, &Settings::setKey_quickload2, Qt::QueuedConnection);
    m_pOptions->addItem(pSelectKey);
    y += 40;


    m_pOptions->setContentHeigth(20 + y);
    pApp->continueThread();
}

void OptionMenue::showSettings()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pOptions->clearContent();
    AudioThread* pAudio = pApp->getAudioThread();
    Settings* pSettings = pApp->getSettings();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    // cache all supported display modes
    SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, nullptr};
    qint32 modes = SDL_GetNumDisplayModes(0);
    QVector<QSize> supportedSizes;
    for  (qint32 i = 0; i < modes; i++)
    {
        if (SDL_GetDisplayMode(0, i, &mode) == 0)
        {
            QSize newSize(mode.w, mode.h);
            if (!supportedSizes.contains(newSize) &&
                newSize.width() >= 1152 &&
                newSize.height() >= 864)
            {

                supportedSizes.append(newSize);
            }
        }
        else
        {
            Console::print(QString("SDL_GetDisplayMode failed: ") + QString(SDL_GetError()), Console::eLogLevels::eERROR);
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
    pTextfield->setText(tr("Screen Settings").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;


    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Screen Resolution: ").toStdString().c_str());
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
    pTextfield->setText(tr("Screen Mode: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    QVector<QString> items = {tr("Window"), tr("Bordered"), tr("Fullscreen")};
    spDropDownmenu pScreenModes = new DropDownmenu(400, items);
    pScreenModes->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pScreenModes);
    connect(pScreenModes.get(), &DropDownmenu::sigItemChanged, this, &OptionMenue::changeScreenMode, Qt::QueuedConnection);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Audio Settings").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Global Volume: ").toStdString().c_str());
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
    pTextfield->setText(tr("Music Volume: ").toStdString().c_str());
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
    pTextfield->setText(tr("Sound Volume: ").toStdString().c_str());
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
    pTextfield->setText(tr("Network Settings").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Username: ").toStdString().c_str());
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
            Settings::setUsername(value);
        }
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Server Adress: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    pTextbox = new Textbox(pApp->getSettings()->getWidth() - 20 - sliderOffset);
    pTextbox->setCurrentText(Settings::getServerAdress());
    connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString value)
    {
        Settings::setServerAdress(value);
        restartNeeded = true;
    });
    pTextbox->setPosition(sliderOffset - 130, y);
    m_pOptions->addItem(pTextbox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Lobby port: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    spSpinBox portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setCurrentValue(Settings::getServerPort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setServerPort(static_cast<quint16>(value));
        restartNeeded = true;
    });
    m_pOptions->addItem(portBox);
    y += 40;

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Server: ").toStdString().c_str());
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
    pTextfield->setText(tr("Game port: ").toStdString().c_str());
    pTextfield->setPosition(10, y);
    m_pOptions->addItem(pTextfield);
    portBox = new SpinBox(200, 0, std::numeric_limits<quint16>::max());
    portBox->setCurrentValue(Settings::getGamePort());
    portBox->setPosition(sliderOffset - 130, y);
    connect(portBox.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        Settings::setGamePort(static_cast<quint16>(value));
        restartNeeded = true;
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
    QFileInfoList infoList = QDir("mods").entryInfoList(QDir::Dirs);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 0;
    qint32 mods = 0;
    Settings* pSettings = Mainapp::getInstance()->getSettings();
    QStringList currentMods = pSettings->getMods();

    for (qint32 i = 0; i < infoList.size(); i++)
    {
        QString folder = infoList[i].filePath();
        if (!folder.endsWith("."))
        {
            QString name = folder;
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
                }
            }
            oxygine::spTextField pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            pTextfield->setText(name.toStdString().c_str());
            pTextfield->setPosition(10, 10 + mods * 40);
            m_pOptions->addItem(pTextfield);
            qint32 curWidth = pTextfield->getTextRect().getWidth() + 30;
            spCheckbox modCheck = new Checkbox();
            modCheck->setPosition(curWidth, pTextfield->getY());
            m_pOptions->addItem(modCheck);
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
        }
    }
    m_pOptions->setContentWidth(width);
    m_pOptions->setContentHeigth(20 + mods * 40);
    pApp->continueThread();
}

void OptionMenue::restart()
{
    QCoreApplication::exit(1);
}
