#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationnextday.h"
#include "game/co.h"
#include "game/gamemap.h"
#include "game/gameanimationfactory.h"

#include "menue/gamemenue.h"



GameAnimationNextDay::GameAnimationNextDay(Player* pPlayer, quint32 frameTime, bool permanent)
    : GameAnimation(frameTime),
      m_permanent(permanent)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());    
    Interpreter::setCppOwnerShip(this);
    setSize(Settings::getWidth(), Settings::getHeight());
    oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("nextday");
    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setScaleX((Settings::getWidth() + 8.0f) / pAnim->getWidth());
    pSprite->setScaleY((Settings::getHeight() + 8.0f) / pAnim->getHeight());
    pSprite->setPosition(-4.0f, -4.0f);
    QColor color = pPlayer->getColor();
    if (permanent)
    {
        pSprite->setColor(color);
        this->setPriority(static_cast<qint32>(Mainapp::ZOrder::AnimationFullScreen));
    }
    else
    {
        color.setAlpha(150);
        pSprite->setColor(color);
    }
    addChild(pSprite);

    CO* pCO = pPlayer->getCO(0);
    oxygine::spSprite m_CO = new oxygine::Sprite();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+nrm";
        oxygine::ResAnim* pAnim = pCO->getResAnim(resAnim);
        if (pAnim != nullptr)
        {
            m_CO->setResAnim(pAnim);
            m_CO->setScale((Settings::getHeight() - 300) / pAnim->getHeight());
            m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
            m_CO->setPosition(Settings::getWidth() - m_CO->getScaledWidth() - 20, Settings::getHeight() / 2 -  m_CO->getScaledHeight() / 2);
            addChild(m_CO);
        }
    }
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+nrm";
        oxygine::ResAnim* pAnim = pCO->getResAnim(resAnim);
        if (pAnim != nullptr)
        {
            m_CO = new oxygine::Sprite();
            m_CO->setResAnim(pAnim);
            m_CO->setScale((Settings::getHeight() - 300) / pAnim->getHeight());
            m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
            m_CO->setPosition(20, Settings::getHeight() / 2 -  m_CO->getScaledHeight() / 2);
            addChild(m_CO);
        }
    }

    oxygine::TextStyle headline = FontManager::getMainFont72();
    headline.color = FontManager::getFontColor();
    headline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headline.multiline = false;
    oxygine::spTextField textField = new oxygine::TextField();
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        textField->setHtmlText((QString("Day ") + QString::number(pMap->getCurrentDay())));
    }
    textField->setPosition(10, Settings::getHeight() / 2 - textField->getTextRect().getHeight() / 2);
    if (pCO != nullptr)
    {
        textField->setX(40 + m_CO->getScaledWidth());
    }
    textField->setStyle(headline);
    addChild(textField);

    if (!m_permanent)
    {
        endTimer.setSingleShot(true);
        endTimer.setInterval(1000 / Settings::getAnimationSpeed());
        connect(&endTimer, &QTimer::timeout, [=]()
        {
            emitFinished();
        });
        endTimer.start();
        addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                {
                    emit sigRightClick();
                }
                else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                {
                    if (m_permanent)
                    {
                        emit sigRightClick();
                    }
                }
            }
        });
    }
    else
    {
        spGameMenue pMenu = GameMenue::getInstance();
        if (pMenu.get() != nullptr)
        {
            oxygine::spButton pButtonSaveAndExit = ObjectManager::createButton(tr("Save and Exit"), 220);
            pButtonSaveAndExit->attachTo(this);
            pButtonSaveAndExit->setPosition(Settings::getWidth() / 2 - pButtonSaveAndExit->getWidth() - 10, Settings::getHeight() - 50);
            pButtonSaveAndExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
            {
                emit sigShowSaveAndExit();
            });
            connect(this, &GameAnimationNextDay::sigShowSaveAndExit, pMenu.get(), &GameMenue::showSaveAndExitGame, Qt::QueuedConnection);

            oxygine::spButton pButtonContinue = ObjectManager::createButton(tr("Continue"), 220);
            pButtonContinue->attachTo(this);
            pButtonContinue->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 50);
            pButtonContinue->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
            {
                emit sigRightClick();
            });
        }
    }
    connect(this, &GameAnimationNextDay::sigRightClick, this, &GameAnimationNextDay::rightClick, Qt::QueuedConnection);
}

void GameAnimationNextDay::rightClick()
{
    if (!m_permanent)
    {
        stop();
        onFinished(false);
    }
    else
    {
        detach();
    }
}

void GameAnimationNextDay::stop()
{
    endTimer.stop();
}

void GameAnimationNextDay::restart()
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        if (!m_permanent)
        {
            pMenu->addChild(this);
            endTimer.start();
        }
    }
}

bool GameAnimationNextDay::onFinished(bool skipping)
{
    bool ret = true;
    if (!m_permanent)
    {
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr)
        {
            pMap->getGameScript()->turnStart(pMap->getCurrentDay(), pMap->getCurrentPlayer()->getPlayerID());
        }
        ret = GameAnimation::onFinished(skipping);
        
    }
    return ret;
}
