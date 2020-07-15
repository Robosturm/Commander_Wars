#include "gameanimationdialog.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "menue/gamemenue.h"

#include "game/gameanimationfactory.h"

#include "qfile.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"

GameAnimationDialog::GameAnimationDialog(quint32 frameTime)
    : GameAnimation (frameTime),
      textSpeed(100 / Settings::getAnimationSpeed())
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    connect(this, &GameAnimationDialog::sigStartFinishTimer, this, &GameAnimationDialog::startFinishTimer, Qt::QueuedConnection);
    connect(&finishTimer, &QTimer::timeout, this, &GameAnimationDialog::onFinished, Qt::QueuedConnection);

    m_BackgroundSprite = new oxygine::Sprite();
    addChild(m_BackgroundSprite);

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("dialogfield+mask");
    m_TextMask = new oxygine::Sprite();
    m_TextMask->setScaleX(Settings::getWidth() / pAnim->getWidth());
    m_TextMask->setResAnim(pAnim);
    addChild(m_TextMask);

    pAnim = pGameManager->getResAnim("dialogfield");
    m_TextBackground = new oxygine::Sprite();
    m_TextBackground->setScaleX(Settings::getWidth() / pAnim->getWidth());
    m_TextBackground->setResAnim(pAnim);
    addChild(m_TextBackground);

    oxygine::TextStyle style = FontManager::getMainFont48();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    oxygine::spClipRectActor pRect = new oxygine::ClipRectActor();
    pRect->setPosition(48 * 2 + 5, 6);
    pRect->setSize(Settings::getWidth() - pRect->getX() - 5, 96);

    m_TextField = new oxygine::TextField();
    m_TextField->setPosition(0, -10);
    m_TextField->setSize(pRect->getWidth() - 5, pRect->getHeight());
    m_TextField->setStyle(style);
    pRect->addChild(m_TextField);
    addChild(pRect);

    m_COSprite = new oxygine::Sprite();
    m_COSprite->setScale(2);
    m_COSprite->setY(6);
    addChild(m_COSprite);

    setPositionTop(false);
    textTimer.start();
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
                emit sigLeftClick();
            }
        }
        pTouchEvent->stopPropagation();
    });
    connect(this, &GameAnimationDialog::sigRightClick, this, &GameAnimationDialog::onFinished, Qt::QueuedConnection);
    connect(this, &GameAnimationDialog::sigLeftClick, this, &GameAnimationDialog::nextDialogStep, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, this, &GameAnimationDialog::keyInput, Qt::QueuedConnection);
}

void GameAnimationDialog::keyInput(oxygine::KeyEvent event)
{
    if (!m_stopped && writePosition > 0)
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (cur == Settings::getKey_confirm())
        {
            nextDialogStep();
        }
    }
}

void GameAnimationDialog::nextDialogStep()
{
    if (paused)
    {
        paused = false;
        writePosition += 1;
        m_TextField->setHtmlText(m_Text.mid(0, writePosition));
    }
    else
    {
        if (writePosition >= m_Text.size())
        {
            onFinished();
        }
        else
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            float textHeight = m_TextField->getTextRect().getHeight();
            qint32 nextHeight = (static_cast<qint32>(textHeight) / dialogHeigth + 1) * dialogHeigth;
            // loop till two lines of text will be shown
            while (writePosition < m_Text.size())
            {
                writePosition += 1;
                m_TextField->setHtmlText(m_Text.mid(0, writePosition));
                textHeight = m_TextField->getTextRect().getHeight();
                if (textHeight > nextHeight)
                {
                    writePosition -= 1;
                    break;
                }
            }
            updateShownText();
            if (writePosition < m_Text.size())
            {
                paused = true;
            }
            pApp->continueThread();
        }
    }
}

void GameAnimationDialog::startFinishTimer()
{
    finishTimer.setSingleShot(true);
    finishTimer.start(autoFinishMs);
}

void GameAnimationDialog::update(const oxygine::UpdateState& us)
{
    // update background if wanted
    if (!m_BackgroundFile.isEmpty())
    {
        _loadBackground();
    }
    if (textTimer.elapsed() > textSpeed && !paused)
    {
        writePosition += 1;
        // check for auto pause
        float textHeight = m_TextField->getTextRect().getHeight();
        qint32 nextHeight = (static_cast<qint32>(textHeight) / dialogHeigth) * dialogHeigth;
        if (static_cast<qint32>(textHeight) % dialogHeigth != 0)
        {
            nextHeight += dialogHeigth;
        }
        m_TextField->setHtmlText(m_Text.mid(0, writePosition));
        textHeight = m_TextField->getTextRect().getHeight();
        if (textHeight > nextHeight && autoFinishMs < 0)
        {
            writePosition -= 1;
            updateShownText();
            paused = true;
        }
        else
        {
            if (writePosition > m_Text.size())
            {
                writePosition = m_Text.size();
                if (autoFinishMs >= 0 && !finishTimer.isActive())
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
        textTimer.start();
    }
    GameAnimation::update(us);
}

void GameAnimationDialog::updateShownText()
{
    m_TextField->setHtmlText(m_Text.mid(0, writePosition));
    float textHeight = m_TextField->getTextRect().getHeight();
    m_TextField->setHeight(textHeight);
    if (textHeight > dialogHeigth)
    {
        m_TextField->setY((-textHeight + dialogHeigth) - 6);
    }
}


bool GameAnimationDialog::onFinished()
{
    if (writePosition >= m_Text.size())
    {
        return GameAnimation::onFinished();
    }
    else
    {
        writePosition = m_Text.size();
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
    oxygine::Sprite::TweenColor tweenColor(color);
    oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(1));
    m_TextMask->addTween(tween);
}

void GameAnimationDialog::setDialog(QString text)
{
    m_Text = text;
}

void GameAnimationDialog::setCO(QString coid, GameEnums::COMood mood)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QString resAnim = coid.toLower() + "+face";
    oxygine::ResAnim* pAnim = pCOSpriteManager->getResAnim(resAnim);
    m_COSprite->setResAnim(pAnim, static_cast<qint32>(mood));
}

void GameAnimationDialog::setFinishDelay(qint32 valueMs)
{
    autoFinishMs = valueMs;
}

void GameAnimationDialog::setTextSpeed(qint32 speed)
{
    textSpeed = speed / Settings::getAnimationSpeed();
}

void GameAnimationDialog::restart()
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        m_stopped = false;
        pGameMenue->addChild(this);
    }
}

void GameAnimationDialog::loadBackground(QString file)
{
    m_BackgroundFile = file;
}

void GameAnimationDialog::_loadBackground()
{
    if (QFile::exists(m_BackgroundFile))
    {
        oxygine::SingleResAnim* pAnim = new oxygine::SingleResAnim();
        pAnim->setResPath(m_BackgroundFile);
        pAnim->init(m_BackgroundFile, 1, 1, 1.0f);
        m_BackgroundAnim = pAnim;
        m_BackgroundSprite->setResAnim(m_BackgroundAnim.get());
        m_BackgroundSprite->setScaleX(Settings::getWidth() / pAnim->getWidth());
        m_BackgroundSprite->setScaleY(Settings::getHeight() / pAnim->getHeight());
    }
    m_BackgroundFile = "";
}
