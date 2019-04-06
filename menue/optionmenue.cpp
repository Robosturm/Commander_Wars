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

#include <QDir>
#include <QFileInfoList>
#include <QTextStream>
#include <QProcess>

OptionMenue::OptionMenue()
{
    Console::print("Entering Main Menue", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
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
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
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

    QSize size(pApp->getSettings()->getWidth() - 20,
               pApp->getSettings()->getHeight() - (20 + pButtonMods->getHeight()) * 2);
    m_pOptions = new  Panel(true,  size, size);
    m_pOptions->setPosition(10, 20 + pButtonMods->getHeight());
    addChild(m_pOptions);

    showSettings();
}

void OptionMenue::exitMenue()
{
    if (restartNeeded)
    {
        restart();
    }
    else
    {
        Console::print("Leaving Editor Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Mainwindow());
        oxygine::Actor::detach();
    }
}

void OptionMenue::showSettings()
{
    m_pOptions->clearContent();
    Mainapp* pApp = Mainapp::getInstance();
    AudioThread* pAudio = pApp->getAudioThread();
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
    // qApp->screens()[0]->size();

}

void OptionMenue::showMods()
{
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
}

void OptionMenue::restart()
{
    QCoreApplication::exit(1);
}
