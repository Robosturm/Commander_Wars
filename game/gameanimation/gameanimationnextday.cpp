#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "game/gameanimation/gameanimationnextday.h"
#include "game/co.h"
#include "game/gamemap.h"
#include "game/gameanimation/gameanimationfactory.h"

#include "menue/basegamemenu.h"
#include "menue/movementplanner.h"

GameAnimationNextDay::GameAnimationNextDay(GameMap* pMap, Player* pPlayer, quint32 frameTime, bool permanent, quint32 uptimeMs)
    : GameAnimation(frameTime, pMap),
      m_permanent(permanent),
      m_endTimer(this)
{    
#ifdef GRAPHICSUPPORT
    setObjectName("GameAnimationNextDay");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    if (pApp->getSlave())
    {
        m_permanent = false;
    }
    moveToThread(pApp->getWorkerthread());    
    Interpreter::setCppOwnerShip(this);
    setSize(Settings::getWidth(), Settings::getHeight());
    oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("nextday");
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    if (pAnim != nullptr)
    {
        pSprite->setScaleX((Settings::getWidth() + 8.0f) / pAnim->getWidth());
        pSprite->setScaleY((Settings::getHeight() + 8.0f) / pAnim->getHeight());
    }
    pSprite->setPosition(-4.0f, -4.0f);
    QColor color = pPlayer->getColor();
    if (permanent)
    {
        pSprite->setColor(color);
        setPriority(static_cast<qint32>(Mainapp::ZOrder::AnimationFullScreen));
    }
    else
    {
        color.setAlpha(150);
        pSprite->setColor(color);
    }
    addChild(pSprite);

    CO* pCO = pPlayer->getCO(0);
    oxygine::spSprite m_CO = oxygine::spSprite::create();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+nrm";
        oxygine::ResAnim* pAnim = pCO->getResAnim(resAnim);
        if (pAnim != nullptr)
        {
            m_CO->setResAnim(pAnim);
            if (Settings::getSmallScreenDevice())
            {
                m_CO->setScale((Settings::getHeight() - 60) / pAnim->getHeight());
            }
            else
            {
                m_CO->setScale((Settings::getHeight() - 300) / pAnim->getHeight());
            }
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
            m_CO = oxygine::spSprite::create();
            m_CO->setResAnim(pAnim);
            if (Settings::getSmallScreenDevice())
            {
                m_CO->setScale((Settings::getHeight() - 60) / pAnim->getHeight());
            }
            else
            {
                m_CO->setScale((Settings::getHeight() - 300) / pAnim->getHeight());
            }
            m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
            m_CO->setPosition(20, Settings::getHeight() / 2 -  m_CO->getScaledHeight() / 2);
            addChild(m_CO);
        }
    }

    oxygine::TextStyle headline = oxygine::TextStyle(FontManager::getMainFont72());
    headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headline.multiline = false;
    oxygine::spTextField textField = oxygine::spTextField::create();
    
    if (m_pMap != nullptr)
    {
        textField->setHtmlText((QString("Day ") + QString::number(m_pMap->getCurrentDay())));
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
        m_endTimer.setSingleShot(true);
        m_endTimer.setInterval(static_cast<qint32>(static_cast<float>(uptimeMs) / Settings::getAnimationSpeed()));
        connect(&m_endTimer, &QTimer::timeout, this, [this]()
        {
            emitFinished();
        });
        m_endTimer.start();
        addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *pEvent )->void
        {
            oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
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
        if (m_pMap != nullptr)
        {
            GameMenue* pMenu = dynamic_cast<GameMenue*>(m_pMap->getMenu());
            if (pMenu != nullptr)
            {
                oxygine::spButton pButtonSaveAndExit = ObjectManager::createButton(tr("Save and Exit"), 220);
                addChild(pButtonSaveAndExit);
                pButtonSaveAndExit->setPosition(Settings::getWidth() / 2 - pButtonSaveAndExit->getScaledWidth() - 10, Settings::getHeight() - 50);
                pButtonSaveAndExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
                {
                    emit sigShowSaveAndExit();
                });
                connect(this, &GameAnimationNextDay::sigShowSaveAndExit, pMenu, &GameMenue::showSaveAndExitGame, Qt::QueuedConnection);

                oxygine::spButton pButtonContinue = ObjectManager::createButton(tr("Continue"), 220);
                addChild(pButtonContinue);
                pButtonContinue->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 50);
                pButtonContinue->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
                {
                    emit sigRightClick();
                });
            }
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
        onFinished(false);
        detach();
    }
}

void GameAnimationNextDay::stop()
{
    m_endTimer.stop();
}

void GameAnimationNextDay::restart()
{
    if (m_pMap != nullptr)
    {
        auto* pMenu = m_pMap->getMenu();
        if (pMenu != nullptr)
        {
            if (!m_permanent)
            {
                pMenu->addChild(spGameAnimationNextDay(this));
                m_endTimer.start();
            }
        }
    }
}

bool GameAnimationNextDay::onFinished(bool skipping)
{
    bool ret = true;
    
    if (m_pMap != nullptr)
    {
        m_pMap->getGameScript()->turnStart(m_pMap->getCurrentDay(), m_pMap->getCurrentPlayer()->getPlayerID());
        if (!m_permanent)
        {
            ret = GameAnimation::onFinished(skipping);
        }
    }
    return ret;
}
