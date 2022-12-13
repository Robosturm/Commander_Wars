#ifdef GRAPHICSUPPORT
#include <QApplication>
#endif

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/base/tooltip.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

oxygine::spActor Tooltip::m_Tooltip{nullptr};

Tooltip::Tooltip()
#ifdef GRAPHICSUPPORT
    : m_TooltipTimer(this),
      m_TooltipPauseTimer(this)
#endif
{
    Interpreter::setCppOwnerShip(this);

#ifdef GRAPHICSUPPORT
    setObjectName("Tooltip");
    m_TooltipTimer.setSingleShot(true);
    m_TooltipPauseTimer.setSingleShot(true);
    addEventListener(oxygine::TouchEvent::MOVE, [this](oxygine::Event* pEvent)
    {
        if (m_mouseHovered)
        {
            emit sigStartTooltip();
        }
        else if (!m_TooltipPauseTimer.isActive())
        {
            emit sigStartHoveredTimer();
        }
    });
    addEventListener(oxygine::TouchEvent::OVER, [this](oxygine::Event* pEvent)
    {
        if (m_mouseHovered)
        {
            emit sigStartTooltip();
        }
        else if (!m_TooltipPauseTimer.isActive())
        {
            emit sigStartHoveredTimer();
        }
    });
    addEventListener(oxygine::TouchEvent::OUTX, [this](oxygine::Event* pEvent)
    {
        m_mouseHovered = false;
        emit sigStartHoveredTimer();
        emit sigHideTooltip();
    });
    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [this](oxygine::Event* pEvent)
    {
        m_mouseHovered = false;
        emit sigStartHoveredTimer();
        emit sigHideTooltip();
    });

    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        emit sigStopTooltip();
    });
    connect(&m_TooltipTimer, &QTimer::timeout, this, &Tooltip::showTooltip, Qt::QueuedConnection);
    connect(&m_TooltipPauseTimer, &QTimer::timeout, this, [this]()
    {
        stopTooltiptimer();
        hideTooltip();
        m_mouseHovered = true;
    }, Qt::QueuedConnection);
#endif
    connect(this, &Tooltip::sigHideTooltip, this, &Tooltip::hideTooltip, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStopTooltip, this, &Tooltip::stopTooltiptimer, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStartTooltip, this, &Tooltip::restartTooltiptimer, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStartHoveredTimer, this, &Tooltip::startHoveredTimer, Qt::QueuedConnection);
}

Tooltip::~Tooltip()
{
#ifdef GRAPHICSUPPORT
    m_TooltipTimer.stop();
#endif
    if (m_Tooltip.get() != nullptr)
    {
        m_Tooltip->detach();
        m_Tooltip = nullptr;
    }
}

void Tooltip::startHoveredTimer()
{
#ifdef GRAPHICSUPPORT
    m_TooltipPauseTimer.start(100);
#endif
}

void Tooltip::restartTooltiptimer()
{
#ifdef GRAPHICSUPPORT
    if (!m_disabled && m_mouseHovered)
    {
        m_TooltipTimer.start(std::chrono::milliseconds(1000));
    }
    else
    {
        m_TooltipTimer.stop();
    }
#endif
    removeTooltip();
}

void Tooltip::stopTooltiptimer()
{
#ifdef GRAPHICSUPPORT
    m_mouseHovered = false;
    m_TooltipTimer.stop();
#endif
}

QString Tooltip::getTooltipText() const
{
#ifdef GRAPHICSUPPORT
    return m_tooltipText;
#else
    return QString();
#endif
}

void Tooltip::setTooltipText(const QString &tooltipText)
{
#ifdef GRAPHICSUPPORT
    m_tooltipText = tooltipText;
#endif
}

void Tooltip::showTooltip()
{
#ifdef GRAPHICSUPPORT
    if (!m_disabled && m_mouseHovered)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->pauseRendering();
        if (oxygine::Stage::getStage()->isDescendant(this) &&
            m_enabled &&
            pApp->hasCursor() &&
            //QGuiApplication::focusWindow() == pApp &&
            !m_tooltipText.isEmpty())
        {
            hideTooltip();
            CONSOLE_PRINT("Showing tooltip", GameConsole::eDEBUG);
            QPoint curPos = pApp->mapPosFromGlobal(pApp->cursor().pos());
            m_Tooltip = oxygine::spActor::create();
            m_Tooltip->setPriority(static_cast<qint32>(Mainapp::ZOrder::Tooltip));

            oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
            style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
            style.multiline = true;

            ObjectManager* pObjectManager = ObjectManager::getInstance();
            oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
            oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
            pSpriteBox->setResAnim(pAnim);
            m_Tooltip->addChild(pSpriteBox);
            pSpriteBox->setPosition(0, 0);
            pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
            oxygine::spTextField pText = oxygine::spTextField::create();
            pText->setHtmlText(m_tooltipText);
            pText->setWidth(Settings::getWidth() / 3);
            pText->setStyle(style);
            pText->setPosition(10, 10);
            pSpriteBox->addChild(pText);
            pSpriteBox->setSize(pText->getTextRect().getSize() + oxygine::Point(30, 30));

            oxygine::Stage::getStage()->addChild(m_Tooltip);
            if (curPos.x() + 10 + pSpriteBox->getScaledWidth() + 5 > Settings::getWidth())
            {
                m_Tooltip->setX(curPos.x() - 10 - pSpriteBox->getScaledWidth());
            }
            else
            {
                m_Tooltip->setX(curPos.x() + 10);
            }
            if (curPos.y() + 10 + pSpriteBox->getScaledHeight() + 5 > Settings::getHeight())
            {
                m_Tooltip->setY(curPos.y() - 10 - pSpriteBox->getScaledHeight());
            }
            else
            {
                m_Tooltip->setY(curPos.y() + 10);
            }
        }
        pApp->continueRendering();
    }
#endif
}

void Tooltip::enableTooltip()
{
#ifdef GRAPHICSUPPORT
    m_mouseHovered = true;
    m_disabled = false;
#endif
}

void Tooltip::disableTooltip()
{
#ifdef GRAPHICSUPPORT
    m_disabled = true;
    stopTooltiptimer();
#endif
    emit sigHideTooltip();
}

void Tooltip::hideTooltip()
{    
#ifdef GRAPHICSUPPORT
    stopTooltiptimer();
    removeTooltip();
#endif
}

void Tooltip::removeTooltip()
{
    if (m_Tooltip.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->pauseRendering();
        m_Tooltip->detach();
        m_Tooltip = nullptr;
        pApp->continueRendering();
    }
}

void Tooltip::looseFocusInternal()
{
    FocusableObject::looseFocusInternal();
    stopTooltiptimer();
    hideTooltip();
}
