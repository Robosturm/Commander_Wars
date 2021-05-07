#include "colorselector.h"

#include "resource_management/fontmanager.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/label.h"
#include "objects/base/focusableobject.h"

ColorSelector::ColorSelector(QColor color, qint32 pixelSize)
    : m_CurrentColor(color),
      m_pixelSize(pixelSize)
{
    setObjectName("ColorSelector");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    // font style
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    qint32 space = 170;
    qint32 y = pixelSize * 256 + 20;
    qint32 barWidth = 30;
    if (pixelSize > 30)
    {
        barWidth = pixelSize;
    }
    setHeight(pixelSize * 256 + 20 + 40);
    if (pixelSize * 256 + barWidth + 30 > space * 3 + 30)
    {
        setWidth(pixelSize * 256 + barWidth + 30);
    }
    else
    {
        setWidth(space * 3 + 30);
    }
    spLabel pLabel = spLabel::create(space);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Red: "));
    pLabel->setPosition(space * 0, y);
    addChild(pLabel);

    m_SpinBoxRed = spSpinBox::create(space - 20, 0, 255);
    m_SpinBoxRed->setPosition(space * 0, y + 40);
    addChild(m_SpinBoxRed);
    connect(m_SpinBoxRed.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(value, m_CurrentColor.green(), m_CurrentColor.blue()));
    });

    pLabel = spLabel::create(space);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Green: "));
    pLabel->setPosition(space * 1, y);
    addChild(pLabel);
    m_SpinBoxGreen = spSpinBox::create(space - 20, 0, 255);
    m_SpinBoxGreen->setPosition(space * 1, y + 40);
    addChild(m_SpinBoxGreen);
    connect(m_SpinBoxGreen.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), value, m_CurrentColor.blue()));
    });

    pLabel = spLabel::create(space);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Blue: "));
    pLabel->setPosition(space * 2, y);
    addChild(pLabel);
    m_SpinBoxBlue= spSpinBox::create(space - 20, 0, 255);
    m_SpinBoxBlue->setPosition(space * 2, y + 40);
    addChild(m_SpinBoxBlue);
    connect(m_SpinBoxBlue.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), m_CurrentColor.green(), value));
    });

    connect(this, &ColorSelector::sigSelecetedColorChanged, this, &ColorSelector::selecetedColorChanged);

    m_ColorDialog = oxygine::spActor::create();
    m_ColorDialog->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event* pEvent)
    {
        pEvent->stopPropagation();
        m_boxUpdating = true;
        FocusableObject::looseFocus();
    });
    m_ColorDialog->addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event* pEvent)
    {
        if (m_boxUpdating)
        {
            pEvent->stopPropagation();
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                qint32 red = pTouchEvent->localPosition.x / (pixelSize);
                qint32 green = pTouchEvent->localPosition.y / (pixelSize);
                emit sigSelecetedColorChanged(QColor(red, green, m_CurrentColor.blue()));
            }
            FocusableObject::looseFocus();
        }
    });
    m_ColorDialog->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event* pEvent)
    {
        if (m_boxUpdating)
        {
            m_boxUpdating = false;
            pEvent->stopPropagation();
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                qint32 red = pTouchEvent->localPosition.x / (pixelSize);
                qint32 green = pTouchEvent->localPosition.y / (pixelSize);
                emit sigSelecetedColorChanged(QColor(red, green, m_CurrentColor.blue()));
            }
            FocusableObject::looseFocus();
        }
    });
    m_ColorDialog->setPosition(0, 0);
    addChild(m_ColorDialog);
    // create dialog fields
    qint32 x = pixelSize * 256 + 30;
    if (x + barWidth + 30 > Settings::getWidth() - 60)
    {
        pixelSize--;
        x = pixelSize * 256 + 30;
    }

    for (qint32 red = 0; red <= 255; red += pixelCount)
    {
        m_RedGreenField.append(QVector<oxygine::spColorRectSprite>());
        for (qint32 green = 0; green <= 255; green += pixelCount)
        {
            oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
            m_RedGreenField[m_RedGreenField.size() - 1].append(pSprite);
            pSprite->setPosition(red * pixelSize, green * pixelSize);
            pSprite->setSize(pixelSize * pixelCount + 1, pixelSize * pixelCount + 1);
            pSprite->setColor(red, green, m_CurrentColor.blue(), 255);
            m_ColorDialog->addChild(pSprite);
        }
    }
    m_Cursor1 = oxygine::spSprite::create();
    oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("colordialogcursor_1");
    m_Cursor1->setResAnim(pAnim);
    m_Cursor1->setPriority(5);
    m_Cursor1->setScale(2);
    m_Cursor1->setPosition(m_CurrentColor.red() * pixelSize - pAnim->getWidth() * m_Cursor1->getScaleX() / 2, m_CurrentColor.green() * pixelSize - pAnim->getHeight()  * m_Cursor1->getScaleY() / 2);
    m_ColorDialog->addChild(m_Cursor1);

    oxygine::spActor bar = oxygine::spActor::create();
    m_ColorDialog->addChild(bar);
    for (qint32 blue = 0; blue <= 255; blue += pixelCount)
    {
        oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
        m_BlueField.append(pSprite);
        pSprite->setPosition(x, blue * pixelSize);
        pSprite->setSize(barWidth, pixelSize * pixelCount + 1);
        pSprite->setColor(m_CurrentColor.red(), m_CurrentColor.green(), blue, 255);
        bar->addChild(pSprite);
    }
    bar->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event* pEvent)
    {
        pEvent->stopPropagation();
        m_barUpdating = true;
    });
    bar->addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event* pEvent)
    {
        if (m_barUpdating)
        {
            pEvent->stopPropagation();
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                qint32 blue = pTouchEvent->localPosition.y / (pixelSize);
                emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), m_CurrentColor.green(), blue));
            }
            FocusableObject::looseFocus();
        }
    });
    bar->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event* pEvent)
    {
        if (m_barUpdating)
        {
            m_barUpdating = false;
            pEvent->stopPropagation();
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                qint32 blue = pTouchEvent->localPosition.y / (pixelSize);
                emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), m_CurrentColor.green(), blue));
            }
            FocusableObject::looseFocus();
        }
    });

    m_Cursor2 = oxygine::spSprite::create();
    pAnim = ObjectManager::getInstance()->getResAnim("colordialogcursor_2");
    m_Cursor2->setResAnim(pAnim);
    m_Cursor2->setScale(3);
    m_Cursor2->setPriority(5);
    m_Cursor2->setPosition(x - pAnim->getWidth() * m_Cursor2->getScaleX() + 5, m_CurrentColor.blue() * pixelSize - pAnim->getHeight() / 2);
    m_ColorDialog->addChild(m_Cursor2);

    selecetedColorChanged(m_CurrentColor);
}

void ColorSelector::selecetedColorChanged(QColor color)
{
    
    m_CurrentColor = color;
    m_SpinBoxRed->setCurrentValue(m_CurrentColor.red());
    m_SpinBoxGreen->setCurrentValue(m_CurrentColor.green());
    m_SpinBoxBlue->setCurrentValue(m_CurrentColor.blue());

    for (qint32 red = 0; red <= 255; red += pixelCount)
    {
        for (qint32 green = 0; green <= 255; green += pixelCount)
        {
            m_RedGreenField[red / pixelCount][green / pixelCount]->setColor(red, green, m_CurrentColor.blue(), 255);
        }
    }
    m_Cursor1->setPosition(color.red() * m_pixelSize - m_Cursor1->getResAnim()->getWidth() * m_Cursor1->getScaleX() / 2, color.green() * m_pixelSize - m_Cursor1->getResAnim()->getHeight()  * m_Cursor1->getScaleY() / 2);

    for (qint32 blue = 0; blue <= 255; blue += pixelCount)
    {
        m_BlueField[blue / pixelCount]->setColor(m_CurrentColor.red(), m_CurrentColor.green(), blue, 255);
    }
    m_Cursor2->setY(color.blue() * m_pixelSize - m_Cursor2->getResAnim()->getHeight() / 2);

    
}

QColor ColorSelector::getCurrentColor() const
{
    return m_CurrentColor;
}
