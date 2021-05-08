#include <qfile.h>
#include <qtextstream.h>

#include "menue/creditsmenue.h"
#include "menue/mainwindow.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "ui_reader/uifactory.h"

CreditsMenue::CreditsMenue()
    : Basemenu()
{
    setObjectName("CreditsMenue");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Credits Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("creditsmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(10,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &CreditsMenue::sigExitMenue, this, &CreditsMenue::exitMenue, Qt::QueuedConnection);

    QFile file("resources/credits/credits.cred");
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString line = stream.readLine().trimmed();
        QString lowerLine = line.toLower();
        if (line.startsWith("//"))
        {
            continue;
        }
        if (lowerLine.startsWith("headline:"))
        {
            QString headline = line.remove(0, line.indexOf(":") + 1);
            m_Headlines.append(headline);
            m_Authors.append(QVector<QString>());
            while (!stream.atEnd())
            {
                line = stream.readLine();
                line = line.trimmed();
                lowerLine = line.toLower();
                if (lowerLine.startsWith("end"))
                {
                    break;
                }
                else if (line.startsWith("//"))
                {
                    continue;
                }
                else
                {
                    m_Authors[m_Headlines.size() - 1].append(line);
                }
            }
        }
    }
    m_creditsActor = oxygine::spActor::create();
    addChild(m_creditsActor);
    m_creditsActor->setY(Settings::getHeight());
    qint32 y = 0;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headstyle = FontManager::getMainFont48();
    headstyle.color = FontManager::getFontColor();
    headstyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headstyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headstyle.multiline = false;

    qint32 x = Settings::getWidth() / 2;
    oxygine::spTextField pTextfield;
    for (qint32 i = 0; i < m_Headlines.size(); i++)
    {
        pTextfield = oxygine::spTextField::create();
        pTextfield->setStyle(headstyle);
        pTextfield->setHtmlText(m_Headlines[i]);
        pTextfield->setPosition(x - pTextfield->getTextRect().getWidth() / 2, y);
        m_creditsActor->addChild(pTextfield);
        y += pTextfield->getTextRect().getHeight() + 10;
        for (qint32 i2 = 0; i2 < m_Authors[i].size(); i2++)
        {
            pTextfield = oxygine::spTextField::create();
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(m_Authors[i][i2]);
            pTextfield->setPosition(x - pTextfield->getTextRect().getWidth() / 2.0f, y);
            m_creditsActor->addChild(pTextfield);
            y += pTextfield->getTextRect().getHeight() + 5;
        }
    }
    m_creditsHeigth = y;
    m_speedTimer.start();

    UiFactory::getInstance().createUi("ui/creditsmenu.xml", this);

    pApp->continueRendering();
}

void CreditsMenue::doUpdate(const oxygine::UpdateState&)
{
    if (m_speedTimer.elapsed() > 40)
    {
        m_creditsActor->setY(m_creditsActor->getY() - 2);
        m_speedTimer.start();
    }
    if (m_creditsActor->getY() - Settings::getHeight() / 2.0f + m_creditsHeigth < 0)
    {
        emit sigExitMenue();
    }
}

void CreditsMenue::exitMenue()
{    
    Console::print("Leaving Credits Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(spMainwindow::create());
    oxygine::Actor::detach();    
}
