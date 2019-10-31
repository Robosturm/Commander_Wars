#include "gameanimationnextday.h"

#include "resource_management/gamemanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

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
    oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("nextday");

    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setScaleX(pApp->getSettings()->getWidth() / pAnim->getWidth());
    pSprite->setScaleY(pApp->getSettings()->getHeight() / pAnim->getHeight());
    QColor color = pPlayer->getColor();
    if (permanent)
    {
        pSprite->setColor(color);
        this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));
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
        oxygine::ResAnim* pAnim = COSpriteManager::getInstance()->getResAnim(resAnim.toStdString().c_str());
        m_CO->setResAnim(pAnim);
        m_CO->setScale((pApp->getSettings()->getHeight() - 300) / pAnim->getHeight());
        m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
        m_CO->setPosition(pApp->getSettings()->getWidth() - m_CO->getScaledWidth() - 20, pApp->getSettings()->getHeight() / 2 -  m_CO->getScaledHeight() / 2);
        addChild(m_CO);
    }
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+nrm";
        oxygine::ResAnim* pAnim = COSpriteManager::getInstance()->getResAnim(resAnim.toStdString().c_str());
        m_CO = new oxygine::Sprite();
        m_CO->setResAnim(pAnim);
        m_CO->setScale((pApp->getSettings()->getHeight() - 300) / pAnim->getHeight());
        m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
        m_CO->setPosition(20, pApp->getSettings()->getHeight() / 2 -  m_CO->getScaledHeight() / 2);
        addChild(m_CO);
    }

    oxygine::ResFont* font = FontManager::getMainFont();
    oxygine::TextStyle style = font;
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    float scale = 3;
    oxygine::spTextField textField = new oxygine::TextField();
    textField->setHtmlText((QString("Day ") + QString::number(GameMap::getInstance()->getCurrentDay())));
    textField->setPosition(10, pApp->getSettings()->getHeight() / 2 - textField->getTextRect(scale).getHeight() / 2);
    if (pCO != nullptr)
    {
        textField->setX(40 + m_CO->getScaledWidth());
    }
    textField->setScale(scale);
    textField->setStyle(style);
    addChild(textField);

    if (!m_permanent)
    {
        endTimer.setSingleShot(true);
        endTimer.setInterval(1000 / Settings::getAnimationSpeed());
        connect(&endTimer, &QTimer::timeout, this, &GameAnimationNextDay::onFinished, Qt::QueuedConnection);
        endTimer.start();
    }
    else
    {
        oxygine::spButton pButtonContinue = ObjectManager::createButton(tr("Continue"), 150);
        pButtonContinue->attachTo(this);
        pButtonContinue->setPosition(Settings::getWidth() / 2 - pButtonContinue->getWidth() / 2, Settings::getHeight() - 50);
        pButtonContinue->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigRightClick();
        });
    }
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

    connect(this, &GameAnimationNextDay::sigRightClick, this, &GameAnimationNextDay::rightClick, Qt::QueuedConnection);
}

void GameAnimationNextDay::rightClick()
{
    if (!m_permanent)
    {
        GameAnimationFactory::finishAllAnimations();
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
    if (!m_permanent)
    {
        GameMenue::getInstance()->addChild(this);
        endTimer.start();
    }
}

bool GameAnimationNextDay::onFinished()
{
    bool ret = true;
    if (!m_permanent)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        GameMap* pMap = GameMap::getInstance();
        pMap->getGameScript()->turnStart(pMap->getCurrentDay(), pMap->getCurrentPlayer()->getPlayerID());
        ret = GameAnimation::onFinished();
        pApp->continueThread();
    }
    return ret;
}
