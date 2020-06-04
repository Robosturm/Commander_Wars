#include "tableview.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "objects/label.h"

TableView::TableView(qint32 width, QVector<QStringList> data, QStringList header, bool selectable)
    : QObject(),
      m_Header(header),
      m_data(data)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    setWidth(width);
    setHeight((data.size() + 1) * 45);
    QColor color(255, 127, 39);
    QColor color2(0, 255, 0);
    qint32 fieldWidth = (width - 5 * (header.size() + 1))  / header.size();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spColorRectSprite firstLine = new oxygine::ColorRectSprite();
    firstLine->setSize(width, 5);
    firstLine->setColor(color);
    addChild(firstLine);
    oxygine::spColorRectSprite secondLine = new oxygine::ColorRectSprite();
    secondLine->setSize(5, 40);
    secondLine->setColor(color);
    secondLine->setY(5);
    addChild(secondLine);
    for (qint32 i = 0; i < header.size(); i++)
    {
        spLabel pTextfield = new Label(fieldWidth);
        pTextfield->setPosition(i * (fieldWidth + 5) + 5, 13);
        pTextfield->setStyle(style);
        pTextfield->setHtmlText(header[i]);
        addChild(pTextfield);

        oxygine::spColorRectSprite line = new oxygine::ColorRectSprite();
        line->setSize(5, 40);
        line->setColor(color);
        line->setPosition((i + 1) * (fieldWidth + 5), 5);
        addChild(line);
    }
    oxygine::spColorRectSprite line = new oxygine::ColorRectSprite();
    line->setSize(width, 5);
    line->setColor(color);
    line->setY(5 + 40);
    addChild(line);
    m_VLines.append(line);
    for (qint32 i = 0; i < data.size(); i++)
    {
        line = new oxygine::ColorRectSprite();
        line->setSize(width, 5);
        line->setColor(color);
        line->setY(5 + 40 * (i + 2));
        addChild(line);
        m_VLines.append(line);

        line = new oxygine::ColorRectSprite();
        line->setSize(5, 40);
        line->setColor(color);
        line->setY(5 + (i + 1) * 40);
        addChild(line);
        m_HLines.append(line);
        oxygine::spColorRectSprite line = new oxygine::ColorRectSprite();
        line->setSize(5, 40);
        line->setColor(color);
        line->setPosition((data[i].size()) * (fieldWidth + 5), 5 + (i + 1) * 40);
        addChild(line);
        m_HLines.append(line);
        for (qint32 i2 = 0; i2 < data[i].size(); i2++)
        {
            if (i2 < data[i].size() - 1)
            {
                line = new oxygine::ColorRectSprite();
                line->setSize(5, 40);
                line->setColor(color);
                line->setPosition((i2 + 1) * (fieldWidth + 5), 5 + (i + 1) * 40);
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
            spLabel pTextfield = new Label(fieldWidth);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(data[i][i2]);
            pTextfield->setPosition(i2 * (fieldWidth + 5) + 5, 13 + (i + 1) * 40);
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
}

qint32 TableView::getCurrentItem() const
{
    return currentItem;
}

QStringList TableView::getItem(qint32 i)
{
    if (i >= 0 && i < m_data.size())
    {
        return m_data[i];
    }
    return QStringList();
}
