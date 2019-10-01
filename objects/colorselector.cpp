#include "colorselector.h"

#include "resource_management/fontmanager.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

ColorSelector::ColorSelector(QColor color, qint32 pixelSize)
    : m_CurrentColor(color),
      pixelSize(pixelSize)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    qint32 space = 220;
    qint32 y = pixelSize * 256 + 20;
    qint32 barWidth = 30;
    if (pixelSize > 30)
    {
        barWidth = pixelSize;
    }
    setHeight(pixelSize * 256 + 20 + 40);
    if (pixelSize * 256 + barWidth + 30 > space * 3)
    {
        setWidth(pixelSize * 256 + barWidth + 30);
    }
    else
    {
        setWidth(space * 3);
    }
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Red: ").toStdString().c_str());
    pLabel->setPosition(space * 0, y);
    addChild(pLabel);
    m_SpinBoxRed = new SpinBox(space - pLabel->getTextRect().getWidth() - 20, 0, 255);
    m_SpinBoxRed->setPosition(pLabel->getX() + 10 + pLabel->getTextRect().getWidth(), y);
    addChild(m_SpinBoxRed);
    connect(m_SpinBoxRed.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(value, m_CurrentColor.green(), m_CurrentColor.blue()));
    });

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Green: ").toStdString().c_str());
    pLabel->setPosition(space * 1, y);
    addChild(pLabel);
    m_SpinBoxGreen = new SpinBox(space - pLabel->getTextRect().getWidth() - 20, 0, 255);
    m_SpinBoxGreen->setPosition(pLabel->getX() + 10 + pLabel->getTextRect().getWidth(), y);
    addChild(m_SpinBoxGreen);
    connect(m_SpinBoxGreen.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), value, m_CurrentColor.blue()));
    });

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Blue: ").toStdString().c_str());
    pLabel->setPosition(space * 2, y);
    addChild(pLabel);
    m_SpinBoxBlue= new SpinBox(space - pLabel->getTextRect().getWidth() - 20, 0, 255);
    m_SpinBoxBlue->setPosition(pLabel->getX() + 10 + pLabel->getTextRect().getWidth(), y);
    addChild(m_SpinBoxBlue);
    connect(m_SpinBoxBlue.get(), &SpinBox::sigValueChanged, [=](float value)
    {
        emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), m_CurrentColor.green(), value));
    });

    connect(this, &ColorSelector::sigSelecetedColorChanged, this, &ColorSelector::selecetedColorChanged);

    m_ColorDialog = new oxygine::Actor();
    m_ColorDialog->setPosition(0, 0);
    addChild(m_ColorDialog);
    // create dialog fields
    qint32 x = pixelSize * 256 + 30;
    if (x + barWidth + 30 > pApp->getSettings()->getWidth() - 60)
    {
        pixelSize--;
        x = pixelSize * 256 + 30;
    }
    for (qint32 red = 0; red <= 255; red++)
    {
        m_RedGreenField.append(QVector<oxygine::spColorRectSprite>());
        for (qint32 green = 0; green <= 255; green++)
        {
            oxygine::spColorRectSprite pSprite = new oxygine::ColorRectSprite();
            m_RedGreenField[red].append(pSprite);
            pSprite->setPosition(red * pixelSize, green * pixelSize);
            pSprite->setSize(pixelSize + 1, pixelSize + 1);
            pSprite->setColor(red, green, m_CurrentColor.blue(), 255);
            pSprite->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event* pEvent)
            {
                pEvent->stopPropagation();
                emit sigSelecetedColorChanged(QColor(red, green, m_CurrentColor.blue()));
            });
            m_ColorDialog->addChild(pSprite);
            if (red == m_CurrentColor.red() && green == m_CurrentColor.green())
            {
                m_Cursor1 = new oxygine::Sprite();
                oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("colordialogcursor_1");
                m_Cursor1->setResAnim(pAnim);
                m_Cursor1->setPriority(5);
                m_Cursor1->setScale(2);
                m_Cursor1->setPosition(red * pixelSize - pAnim->getWidth() * m_Cursor1->getScaleX() / 2, green * pixelSize - pAnim->getHeight()  * m_Cursor1->getScaleY() / 2);
                m_ColorDialog->addChild(m_Cursor1);
            }
        }
    }

    for (qint32 blue = 0; blue <= 255; blue++)
    {
        oxygine::spColorRectSprite pSprite = new oxygine::ColorRectSprite();
        m_BlueField.append(pSprite);
        pSprite->setPosition(x, blue * pixelSize);
        pSprite->setSize(barWidth, pixelSize + 1);
        pSprite->setColor(m_CurrentColor.red(), m_CurrentColor.green(), blue, 255);
        pSprite->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event* pEvent)
        {
            pEvent->stopPropagation();
            emit sigSelecetedColorChanged(QColor(m_CurrentColor.red(), m_CurrentColor.green(), blue));
        });
        m_ColorDialog->addChild(pSprite);
        if (blue == m_CurrentColor.blue())
        {
            m_Cursor2 = new oxygine::Sprite();
            oxygine::ResAnim* pAnim = ObjectManager::getInstance()->getResAnim("colordialogcursor_2");
            m_Cursor2->setResAnim(pAnim);
            m_Cursor2->setScale(3);
            m_Cursor2->setPriority(5);
            m_Cursor2->setPosition(x - pAnim->getWidth() * m_Cursor2->getScaleX() + 5, blue * pixelSize - pAnim->getHeight() / 2);
            m_ColorDialog->addChild(m_Cursor2);
        }
    }

    selecetedColorChanged(m_CurrentColor);
}

void ColorSelector::selecetedColorChanged(QColor color)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_CurrentColor = color;
    m_SpinBoxRed->setCurrentValue(m_CurrentColor.red());
    m_SpinBoxGreen->setCurrentValue(m_CurrentColor.green());
    m_SpinBoxBlue->setCurrentValue(m_CurrentColor.blue());

    for (qint32 red = 0; red <= 255; red++)
    {
        for (qint32 green = 0; green <= 255; green++)
        {
            m_RedGreenField[red][green]->setColor(red, green, m_CurrentColor.blue(), 255);

            if (red == m_CurrentColor.red() && green == m_CurrentColor.green())
            {
                m_Cursor1->setPosition(red * pixelSize - m_Cursor1->getResAnim()->getWidth() * m_Cursor1->getScaleX() / 2, green * pixelSize - m_Cursor1->getResAnim()->getHeight()  * m_Cursor1->getScaleY() / 2);
            }
        }
    }

    for (qint32 blue = 0; blue <= 255; blue++)
    {
        m_BlueField[blue]->setColor(m_CurrentColor.red(), m_CurrentColor.green(), blue, 255);
        if (blue == m_CurrentColor.blue())
        {
            m_Cursor2->setY(blue * pixelSize - m_Cursor2->getResAnim()->getHeight() / 2);
        }
    }

    pApp->continueThread();
}

QColor ColorSelector::getCurrentColor() const
{
    return m_CurrentColor;
}
