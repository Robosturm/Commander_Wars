#include "objects/base/tooltip.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include <qguiapplication.h>

Tooltip::Tooltip()
    : m_TooltipTimer(this),
      m_TooltipPauseTimer(this)
{
    setObjectName("Tooltip");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    m_TooltipTimer.setSingleShot(true);
    m_TooltipPauseTimer.setSingleShot(true);
    addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event*)
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
    addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
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
    addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_mouseHovered = false;
        emit sigStartHoveredTimer();
        emit sigHideTooltip();
    });
    addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigStopTooltip();
    });

    connect(this, &Tooltip::sigHideTooltip, this, &Tooltip::hideTooltip, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStopTooltip, this, &Tooltip::stopTooltiptimer, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStartTooltip, this, &Tooltip::restartTooltiptimer, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStartHoveredTimer, this, &Tooltip::startHoveredTimer, Qt::QueuedConnection);
    connect(&m_TooltipTimer, &QTimer::timeout, this, &Tooltip::showTooltip, Qt::QueuedConnection);
    connect(&m_TooltipPauseTimer, &QTimer::timeout, this, [=]()
    {
        stopTooltiptimer();
        hideTooltip();
        m_mouseHovered = true;
    }, Qt::QueuedConnection);
}

Tooltip::~Tooltip()
{
    m_TooltipTimer.stop();
    if (m_Tooltip.get() != nullptr)
    {
        m_Tooltip->detach();
        m_Tooltip = nullptr;
    }
}

void Tooltip::startHoveredTimer()
{
    m_TooltipPauseTimer.start(100);
}

void Tooltip::restartTooltiptimer()
{
    if (!m_disabled && m_mouseHovered)
    {
        m_TooltipTimer.start(std::chrono::milliseconds(1000));
    }
    else
    {
        m_TooltipTimer.stop();
    }
    removeTooltip();
}

void Tooltip::stopTooltiptimer()
{
    m_mouseHovered = false;
    m_TooltipTimer.stop();
}

QString Tooltip::getTooltipText() const
{
    return m_tooltipText;
}

void Tooltip::setTooltipText(const QString &tooltipText)
{
    m_tooltipText = tooltipText;
}

void Tooltip::showTooltip()
{
    if (!m_disabled && m_mouseHovered)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->pauseRendering();
        hideTooltip();
        if (oxygine::getStage()->isDescendant(this) && m_enabled)
        {
            if (QGuiApplication::focusWindow() == pApp && !m_tooltipText.isEmpty())
            {
                Console::print("Showing tooltip", Console::eDEBUG);
                QPoint curPos = pApp->mapFromGlobal(pApp->cursor().pos());
                if (m_Tooltip.get() != nullptr)
                {
                    m_Tooltip->detach();
                    m_Tooltip = nullptr;
                }
                m_Tooltip = oxygine::spActor::create();
                m_Tooltip->setPriority(static_cast<qint32>(Mainapp::ZOrder::Tooltip));

                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
                style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
                style.multiline = true;

                ObjectManager* pObjectManager = ObjectManager::getInstance();
                oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
                pSpriteBox->setResAnim(pAnim);

                pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
                pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
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

                oxygine::getStage()->addChild(m_Tooltip);
                if (curPos.x() + 10 + pSpriteBox->getWidth() + 5 > Settings::getWidth())
                {
                    m_Tooltip->setX(curPos.x() - 10 - pSpriteBox->getWidth());
                }
                else
                {
                    m_Tooltip->setX(curPos.x() + 10);
                }
                if (curPos.y() + 10 + pSpriteBox->getHeight() + 5 > Settings::getHeight())
                {
                    m_Tooltip->setY(curPos.y() - 10 - pSpriteBox->getHeight());
                }
                else
                {
                    m_Tooltip->setY(curPos.y() + 10);
                }
            }
        }
        pApp->continueRendering();
    }
}

void Tooltip::enableTooltip()
{
    m_mouseHovered = true;
    m_disabled = false;
}

void Tooltip::disableTooltip()
{
    m_disabled = true;
    stopTooltiptimer();
    emit sigHideTooltip();
}

void Tooltip::hideTooltip()
{    
    stopTooltiptimer();
    removeTooltip();
}

void Tooltip::removeTooltip()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    if (m_Tooltip.get() != nullptr)
    {
        m_Tooltip->detach();
        m_Tooltip = nullptr;
    }
    pApp->continueRendering();
}

void Tooltip::looseFocusInternal()
{
    FocusableObject::looseFocusInternal();
    stopTooltiptimer();
    hideTooltip();
}
