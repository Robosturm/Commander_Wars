#include "objects/base/multilinetextbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/interpreter.h"

MultilineTextbox::MultilineTextbox(qint32 width, qint32 heigth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MultilineTextbox");
#endif
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setSize(width, heigth);

    m_multiline = true;
    m_lineEdit->setSingleLine(false);

    constexpr qint32 widthReduction = 60;
    QSize size(width, heigth);
    m_pPanel = spPanel::create(true, size, size - QSize(widthReduction, 0));
    addChild(m_pPanel);

    m_textfield = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_textfield->setStyle(style);
    m_textfield->setHtmlText("");
    m_textfield->setSize(width - widthReduction, heigth);
    m_pPanel->addItem(m_textfield);

#ifdef GRAPHICSUPPORT
    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            QMutexLocker locker(m_textfield->getLocked());
            oxygine::text::Node* root = m_textfield->getRootNode();
            m_focusPosition = 0;
            calculateNewFocusPosition(root, pTouchEvent->localPosition.x(), pTouchEvent->localPosition.y());
        }
        pEvent->stopPropagation();
        if (FocusableObject::getFocusedObject() == this)
        {
            setCursorPosition(m_focusPosition);
        }
        else
        {
            emit sigFocused();
        }
    });
#endif
}

bool MultilineTextbox::calculateNewFocusPosition(oxygine::text::Node* pNode, qint32 x, qint32 y)
{
    bool exit = false;
    auto* child = pNode->getFirstChild();
    while (child && !exit)
    {
        exit = getNewFocusPosition(child, x, y);
        child = child->getNextSibling();
    }
    return exit;
}

bool MultilineTextbox::getNewFocusPosition(oxygine::text::Node* pNode, qint32 x, qint32 y)
{
    bool end = false;
#ifdef GRAPHICSUPPORT
    oxygine::text::TextNode* pTextNode = dynamic_cast<oxygine::text::TextNode*>(pNode);
    if (pTextNode != nullptr)
    {
        auto & lines = pTextNode->getLines();
        auto & positions = pTextNode->getPositions();
        qint32 lineCount = lines.size();
        const QFont & font = m_textfield->getStyle().font.font;
        qint32 contentY = m_pPanel->getContentY();
        QFontMetrics metrics(font);
        qint32 descent = metrics.descent();
        for (qint32 i = 0; i < lineCount && !end; ++i)
        {
            if (y - contentY < positions[i].y() + descent)
            {
                m_focusPosition += getClickedLinePosition(x, lines[i], font);
                end = true;
                break;
            }
            m_focusPosition += lines[i].size();
        }
    }
    if (!end)
    {
        end = calculateNewFocusPosition(pNode, x, y);
    }
#endif
    return end;
}


bool MultilineTextbox::onEditFinished()
{
    emit sigTextChanged(getCurrentText());
    emit sigEnterPressed(getCurrentText());
    return true;
}

void MultilineTextbox::focusedLost()
{
    emit sigTextChanged(getCurrentText());
    TextInput::focusedLost();
}

void MultilineTextbox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    QString drawText = getDrawText(getCurrentText());
    if (m_lastDrawText != drawText)
    {
        m_textfield->setHtmlText(drawText);
        m_lastDrawText = drawText;
        if (drawText.size() > 0)
        {
            // calc text field position based on curmsgpos
            m_pPanel->setContentHeigth(m_textfield->getTextRect().height() + 40);
        }
    }
    oxygine::Actor::update(us);
}

void MultilineTextbox::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_pPanel->setEnabled(value);
    m_textfield->setEnabled(value);
}
