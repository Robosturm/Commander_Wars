#include <QFile>

#include "game/gameanimation/gameanimationdialog.h"
#include "game/gameanimation/gameanimationfactory.h"
#include "game/gamemap.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "menue/gamemenue.h"
#include "menue/movementplanner.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

GameAnimationDialog::GameAnimationDialog(quint32 frameTime, GameMap* pMap)
    : GameAnimation (frameTime, pMap),
      m_finishTimer(this),
      m_textSpeed(100 / Settings::getDialogAnimationSpeed())
{
    setObjectName("GameAnimationDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    connect(this, &GameAnimationDialog::sigStartFinishTimer, this, &GameAnimationDialog::startFinishTimer, Qt::QueuedConnection);
    connect(&m_finishTimer, &QTimer::timeout, this, [this]()
    {
        emitFinished();
    });
    m_BackgroundSprite = oxygine::spSprite::create();
    m_BackgroundSprite->setPriority(-1);
    addChild(m_BackgroundSprite);

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("dialogfield+mask");
    m_TextMask = oxygine::spSprite::create();
    if (pAnim != nullptr)
    {
        m_TextMask->setScaleX(Settings::getWidth() / pAnim->getWidth());
    }
    m_TextMask->setResAnim(pAnim);
    addChild(m_TextMask);

    pAnim = pGameManager->getResAnim("dialogfield");
    m_TextBackground = oxygine::spSprite::create();
    if (pAnim != nullptr)
    {
        m_TextBackground->setScaleX(Settings::getWidth() / pAnim->getWidth());
    }
    m_TextBackground->setResAnim(pAnim);
    m_TextBackground->setPriority(1);
    addChild(m_TextBackground);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont48());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    oxygine::spClipRectActor pRect = oxygine::spClipRectActor::create();
    pRect->setPosition(48 * 2 + 5, 6);
    pRect->setSize(Settings::getWidth() - pRect->getX() - 5, 96);

    m_TextField = oxygine::spTextField::create();
    m_TextField->setPosition(0, -10);
    m_TextField->setSize(pRect->getWidth() - 5, pRect->getHeight());
    m_TextField->setStyle(style);
    pRect->addChild(m_TextField);
    pRect->setPriority(1);
    addChild(pRect);

    m_COSprite = oxygine::spSprite::create();
    m_COSprite->setScale(2);
    m_COSprite->setY(6);
    m_COSprite->setPriority(1);
    addChild(m_COSprite);

    setPositionTop(false);
    m_textTimer.start();
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
                emit sigLeftClick();
            }
            pTouchEvent->stopPropagation();
        }
    });
    connect(this, &GameAnimationDialog::sigRightClick, GameAnimationFactory::getInstance(), &GameAnimationFactory::finishAllAnimationsWithEmitFinished, Qt::QueuedConnection);
    connect(this, &GameAnimationDialog::sigLeftClick, this, &GameAnimationDialog::nextDialogStep, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, this, &GameAnimationDialog::keyInput, Qt::QueuedConnection);
}

void GameAnimationDialog::keyInput(oxygine::KeyEvent event)
{
    if (!event.getContinousPress())
    {
        if (!m_stopped && m_writePosition > 0)
        {
            // for debugging
            Qt::Key cur = event.getKey();
            if (cur == Settings::getKey_confirm() ||
                cur == Settings::getKey_confirm2())
            {
                nextDialogStep();
            }
        }
    }
}

void GameAnimationDialog::nextDialogStep()
{
    if (m_paused)
    {
        m_paused = false;
        m_writePosition += 1;
        m_TextField->setHtmlText(m_Text.mid(0, m_writePosition));
    }
    else
    {
        if (m_writePosition >= m_Text.size())
        {
            onFinished(false);
        }
        else
        {
            float textHeight = m_TextField->getTextRect().getHeight();
            qint32 nextHeight = (static_cast<qint32>(textHeight) / dialogHeigth + 1) * dialogHeigth;
            // loop till two lines of text will be shown
            while (m_writePosition < m_Text.size())
            {
                m_writePosition += 1;
                m_TextField->setHtmlText(m_Text.mid(0, m_writePosition));
                textHeight = m_TextField->getTextRect().getHeight();
                if (textHeight > nextHeight)
                {
                    m_writePosition -= 1;
                    break;
                }
            }
            updateShownText();
            if (m_writePosition < m_Text.size())
            {
                m_paused = true;
            }
            
        }
    }
}

void GameAnimationDialog::startFinishTimer()
{
    m_finishTimer.setSingleShot(true);
    m_finishTimer.start(m_autoFinishMs);
}

void GameAnimationDialog::update(const oxygine::UpdateState& us)
{
    if (m_textTimer.elapsed() > m_textSpeed && !m_paused)
    {
        m_writePosition += 1;
        // check for auto pause
        float textHeight = m_TextField->getTextRect().getHeight();
        qint32 nextHeight = (static_cast<qint32>(textHeight) / dialogHeigth) * dialogHeigth;
        if (static_cast<qint32>(textHeight) % dialogHeigth != 0)
        {
            nextHeight += dialogHeigth;
        }
        m_TextField->setHtmlText(m_Text.mid(0, m_writePosition));
        textHeight = m_TextField->getTextRect().getHeight();
        if (textHeight > nextHeight && m_autoFinishMs < 0)
        {
            m_writePosition -= 1;
            updateShownText();
            m_paused = true;
        }
        else
        {
            if (m_writePosition > m_Text.size())
            {
                m_writePosition = m_Text.size();
                if (m_autoFinishMs >= 0 && !m_finishTimer.isActive())
                {
                    emit sigStartFinishTimer();
                }
            }
            else
            {
                Mainapp* pApp = Mainapp::getInstance();
                pApp->getAudioThread()->playSound("speaking.wav");
            }
            updateShownText();
        }
        m_textTimer.start();
    }
    GameAnimation::update(us);
}

void GameAnimationDialog::updateShownText()
{
    m_TextField->setHtmlText(m_Text.mid(0, m_writePosition));
    float textHeight = m_TextField->getTextRect().getHeight();
    m_TextField->setHeight(textHeight);
    if (textHeight > dialogHeigth)
    {
        m_TextField->setY((-textHeight + dialogHeigth) - 6);
    }
}

bool GameAnimationDialog::onFinished(bool skipping)
{
    if (m_writePosition >= m_Text.size())
    {
        return GameAnimation::onFinished(skipping);
    }
    else
    {
        m_writePosition = m_Text.size();
    }
    return false;
}

void GameAnimationDialog::setPositionTop(bool value)
{
    if (value)
    {
        setY(0);
        m_BackgroundSprite->setY(0);
    }
    else
    {
        setY(Settings::getHeight() - m_TextBackground->getScaledHeight());
        m_BackgroundSprite->setY(-getY());
    }
}

void GameAnimationDialog::setColor(QColor color)
{
    m_TextMask->setColor(color);
}

void GameAnimationDialog::setDialog(const QString & text)
{
    m_Text = text;
}

void GameAnimationDialog::setCO(const QString & coid, GameEnums::COMood mood)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QString resAnim = coid.toLower() + "+face";
    oxygine::ResAnim* pAnim = pCOSpriteManager->getResAnim(resAnim);
    if (pAnim != nullptr)
    {
        if (pAnim->getColumns() > 0)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnimColumn(pAnim, static_cast<qint32>(mood)), oxygine::timeMS(static_cast<qint64>(pAnim->getColumns() * GameMap::frameTime)), -1);
            m_COSprite->addTween(tween);
        }
        else
        {
            m_COSprite->setResAnim(pAnim, static_cast<qint32>(mood));
        }
    }
}

void GameAnimationDialog::setPlayerCO(qint32 player, quint8 co, GameEnums::COMood mood)
{
    
    if (m_pMap)
    {
        if (player >= 0 && player < m_pMap->getPlayerCount())
        {
            CO* pCo = m_pMap->getPlayer(player)->getCO(co);
            if (pCo != nullptr)
            {
                QString resAnim = pCo->getCoID().toLower() + "+face";
                oxygine::ResAnim* pAnim = pCo->getResAnim(resAnim);
                m_COSprite->setResAnim(pAnim, static_cast<qint32>(mood));
            }
            else
            {
                m_COSprite->setResAnim(nullptr);
            }
        }
    }
}

void GameAnimationDialog::setFinishDelay(qint32 valueMs)
{
    m_autoFinishMs = valueMs;
}

void GameAnimationDialog::setTextSpeed(qint32 speed)
{
    m_textSpeed = speed / Settings::getAnimationSpeed();
}

void GameAnimationDialog::restart()
{
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    if (pMenu != nullptr)
    {
        m_stopped = false;
        pMenu->addChild(spGameAnimationDialog(this));
    }
}

void GameAnimationDialog::loadBackground(const QString & file)
{
    if (!file.isEmpty())
    {
        QImage img;
        if (QFile::exists(Settings::getUserPath() + file))
        {
            img = QImage(Settings::getUserPath() + file);
        }
        else if (QFile::exists(oxygine::Resource::RCC_PREFIX_PATH + file))
        {
            img = QImage(oxygine::Resource::RCC_PREFIX_PATH + file);
        }
        oxygine::spSingleResAnim pAnim = oxygine::spSingleResAnim::create();
        Mainapp::getInstance()->loadResAnim(pAnim, img, 1, 1, 1, false);
        m_BackgroundAnim = pAnim;
        m_BackgroundSprite->setResAnim(m_BackgroundAnim.get());
        if (pAnim.get() != nullptr)
        {
            m_BackgroundSprite->setScaleX(Settings::getWidth() / pAnim->getWidth());
            m_BackgroundSprite->setScaleY(Settings::getHeight() / pAnim->getHeight());
        }
    }
    else
    {
        CONSOLE_PRINT("Ignoring loading of empty image. GameAnimationDialog::loadBackground", Console::eDEBUG);
    }
}

void GameAnimationDialog::loadCoSprite(const QString & coid, float offsetX, float offsetY, bool flippedX, float scale)
{
    if (!coid.isEmpty())
    {
        oxygine::ResAnim* pAnim = COSpriteManager::getInstance()->getResAnim(coid + "+nrm", oxygine::error_policy::ep_ignore_error);
        if (pAnim != nullptr)
        {
            oxygine::spSprite pSprite = oxygine::spSprite::create();
            pSprite->setSize(pAnim->getSize());
            pSprite->setFlippedX(flippedX);
            pSprite->setScale(scale);
            pSprite->setResAnim(pAnim);
            pSprite->setPosition(offsetX, offsetY);
            addChild(pSprite);
        }
    }
}
