#include "tableview.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

TableView::TableView(QVector<qint32> widths, QVector<QStringList> data, QStringList header, bool selectable)
    : QObject(),
      m_Header(header),
      m_data(data),
      m_widths(widths)
{
    setObjectName("TableView");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    qint32 width = 5;
    for (qint32 i = 0; i < m_widths.size(); i++)
    {
        width += m_widths[i];
    }
    setWidth(width);
    setHeight((data.size() + 1) * 45);
    QColor color(255, 127, 39);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // first vertical line
    oxygine::spColorRectSprite firstLine = oxygine::spColorRectSprite::create();
    firstLine->setSize(width, 5);
    firstLine->setColor(color);
    addChild(firstLine);
    // first horizontal line
    oxygine::spColorRectSprite secondLine = oxygine::spColorRectSprite::create();
    secondLine->setSize(5, 40);
    secondLine->setColor(color);
    secondLine->setY(5);
    addChild(secondLine);
    qint32 x = 0;
    for (qint32 i = 0; i < header.size(); i++)
    {
        // header label
        spLabel pTextfield = spLabel::create(m_widths[i] - 10);
        pTextfield->setPosition(x + 5, 13);
        pTextfield->setStyle(style);
        pTextfield->setHtmlText(header[i]);
        addChild(pTextfield);
        m_Labels.append(pTextfield);
        x += widths[i];
        // following horizintal line
        oxygine::spColorRectSprite line = oxygine::spColorRectSprite::create();
        line->setSize(5, 40);
        line->setColor(color);
        line->setPosition(x, 5);
        addChild(line);
    }
    // vertical line after header
    oxygine::spColorRectSprite line = oxygine::spColorRectSprite::create();
    line->setSize(width, 5);
    line->setColor(color);
    line->setY(5 + 40);
    addChild(line);
    m_VLines.append(line);
    x = 0;
    for (qint32 i = 0; i < data.size(); i++)
    {
        // vertical line below the item
        line = oxygine::spColorRectSprite::create();
        line->setSize(width, 5);
        line->setColor(color);
        line->setY(5 + 40 * (i + 2));
        addChild(line);
        m_VLines.append(line);
        // initial horizontal line
        line = oxygine::spColorRectSprite::create();
        line->setSize(5, 40);
        line->setColor(color);
        line->setY(5 + (i + 1) * 40);
        addChild(line);
        m_HLines.append(line);
        // final horizontal line
        oxygine::spColorRectSprite line = oxygine::spColorRectSprite::create();
        line->setSize(5, 40);
        line->setColor(color);
        line->setPosition(width - 5, 5 + (i + 1) * 40);
        addChild(line);
        m_HLines.append(line);

        for (qint32 i2 = 0; i2 < data[i].size(); i2++)
        {
            addRow(i, i2, x, selectable);
            x += widths[i2];
        }
        x = 0;
    }
}

void TableView::addRow(qint32 i, qint32 i2, qint32 x, bool selectable)
{
    QColor color(255, 127, 39);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    if (i2 < m_data[i].size() - 1)
    {
        oxygine::spColorRectSprite line = oxygine::spColorRectSprite::create();
        line->setSize(5, 40);
        line->setColor(color);
        line->setPosition(x + m_widths[i2], 5 + (i + 1) * 40);
        addChild(line);
        if (selectable)
        {
            line->addClickListener([=](oxygine::Event*)
            {
                setCurrentItem(i);
                emit sigItemClicked();
            });
        }
    }
    spLabel pTextfield = spLabel::create(m_widths[i2] - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(m_data[i][i2]);
    pTextfield->setPosition(x + 5, 13 + (i + 1) * 40);
    addChild(pTextfield);
    if (selectable)
    {
        pTextfield->addClickListener([=](oxygine::Event*)
        {
            setCurrentItem(i);
            emit sigItemClicked();
        });
    }
}

void TableView::setCurrentItem(qint32 i)
{
    
    QColor color(255, 127, 39);
    QColor color2(0, 255, 0);
    for (qint32 i3 = 0; i3 < m_HLines.size(); i3++)
    {
        m_HLines[i3]->setColor(color);
        m_HLines[i3]->setPriority(0);
    }
    for (qint32 i3 = 0; i3 < m_VLines.size(); i3++)
    {
        m_VLines[i3]->setColor(color);
        m_VLines[i3]->setPriority(0);
    }
    m_HLines[i * 2]->setColor(color2);
    m_HLines[i * 2]->setPriority(1);
    m_HLines[i * 2 + 1]->setColor(color2);
    m_HLines[i * 2 + 1]->setPriority(1);
    m_VLines[i]->setColor(color2);
    m_VLines[i]->setPriority(1);
    m_VLines[i + 1]->setColor(color2);
    m_VLines[i + 1]->setPriority(1);
    m_currentItem = i;
    
}

qint32 TableView::getCurrentItem() const
{
    return m_currentItem;
}

QStringList TableView::getItem(qint32 i)
{
    if (i >= 0 && i < m_data.size())
    {
        return m_data[i];
    }
    return QStringList();
}

