#include "objects/tableView/complextableview.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

constexpr quint32 DIVIDER_SIZE = 8;
constexpr quint32 ITEM_HEIGHT = 40;

ComplexTableView::ComplexTableView(const QVector<qint32> & widths, const QStringList & header, qint32 heigth, QObject *parent)
    : QObject{parent},
      m_widths(widths)
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("ComplexTableView");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    qint32 width = DIVIDER_SIZE;
    for (qint32 i = 0; i < m_widths.size(); i++)
    {
        width += m_widths[i];
    }
    setWidth(width);
    setHeight(heigth);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pBackground = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pBackground->setResAnim(pAnim);
    pBackground->setSize(getSize());
    addChild(pBackground);
    oxygine::ResAnim* phAnim =pObjectManager->getResAnim("h_table_divider");
    oxygine::ResAnim* pvAnim =pObjectManager->getResAnim("v_table_divider");

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    // first vertical line
    oxygine::spBox9Sprite firstLine = oxygine::spBox9Sprite::create();
    firstLine->setResAnim(pvAnim);
    firstLine->setSize(width, DIVIDER_SIZE);
    addChild(firstLine);

    // first horizontal line
    oxygine::spBox9Sprite secondLine = oxygine::spBox9Sprite::create();
    secondLine->setResAnim(phAnim);
    secondLine->setSize(DIVIDER_SIZE, ITEM_HEIGHT);
    secondLine->setY(DIVIDER_SIZE);
    addChild(secondLine);
    qint32 x = 0;
    for (qint32 i = 0; i < header.size(); i++)
    {
        // header label
        spLabel pTextfield = spLabel::create(m_widths[i] - DIVIDER_SIZE - ITEM_HEIGHT);
        pTextfield->setPosition(x + DIVIDER_SIZE, 13);
        pTextfield->setStyle(style);
        pTextfield->setHtmlText(header[i]);
        addChild(pTextfield);
        m_Labels.append(pTextfield);
        x += widths[i];
        auto pSortButton = pObjectManager->createIconButton("small_arrow+down", 32);
        oxygine::Actor* pActor = pSortButton.get();
        m_sortUp.append(false);
        pSortButton->addClickListener([this, i, pActor](oxygine::Event*)
        {
            m_sortUp[i] = !m_sortUp[i];
            if (m_sortUp[i])
            {
                oxygine::static_pointer_cast<oxygine::Sprite>(pActor->getFirstChild())->setFlippedY(true);
            }
            else
            {
                oxygine::static_pointer_cast<oxygine::Sprite>(pActor->getFirstChild())->setFlippedY(false);
            }
            emit sigSortItems(i, m_sortUp[i]);
        });
        pSortButton->setPosition(x - pSortButton->getWidth(), DIVIDER_SIZE + ITEM_HEIGHT / 2 - pSortButton->getHeight() / 2);
        addChild(pSortButton);
        // following horizintal line
        oxygine::spBox9Sprite line = oxygine::spBox9Sprite::create();
        line->setResAnim(phAnim);
        line->setSize(DIVIDER_SIZE, ITEM_HEIGHT);
        line->setPosition(x, DIVIDER_SIZE);
        addChild(line);
    }
    // vertical line after header
    oxygine::spBox9Sprite line = oxygine::spBox9Sprite::create();
    line->setSize(width, DIVIDER_SIZE);
    line->setResAnim(pvAnim);
    line->setY(DIVIDER_SIZE + ITEM_HEIGHT);
    addChild(line);

    const auto yOffset = DIVIDER_SIZE * 2 + ITEM_HEIGHT;
    QSize size (width + Panel::sliderSize, heigth - yOffset);
    m_pPanel = spPanel::create(false, size, size + QSize(0, 20));
    m_pPanel->setPosition(0, yOffset);
    addChild(m_pPanel);
    connect(this, &ComplexTableView::sigSortItems, this, &ComplexTableView::sortItems, Qt::QueuedConnection);
}

const QVector<qint32> & ComplexTableView::getWidths() const
{
    return m_widths;
}


void ComplexTableView::sortItems(qint32 column, bool up)
{
    std::sort(m_items.begin(), m_items.end(), [this, column, up](const Item & lhs, const Item & rhs)
    {
        if (up)
        {
            return *(lhs.items[column].get()) > *(rhs.items[column].get());
        }
        else
        {
            return *(lhs.items[column].get()) < *(rhs.items[column].get());
        }
    });
    if (m_columns.size() > 0)
    {
        updateItemPositions();
    }
}

void ComplexTableView::createItems()
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* phAnim =pObjectManager->getResAnim("h_table_divider");
    oxygine::ResAnim* pvAnim =pObjectManager->getResAnim("v_table_divider");

    const qint32 hCount = m_widths.size() + 1;
    m_HLines.reserve(hCount * m_widths.size());
    m_VLines.reserve(hCount);

    qint32 y = 0;
    qint32 count = 0;
    for (auto & item : m_items)
    {
        oxygine::spActor column = oxygine::spActor::create();
        // first horizontal line
        oxygine::spBox9Sprite line = oxygine::spBox9Sprite::create();
        line->setResAnim(phAnim);
        line->setSize(DIVIDER_SIZE, ITEM_HEIGHT);
        m_HLines.append(line);
        column->addChild(line);
        qint32 x = 0;
        for (qint32 i = 0; i < item.items.size(); i++)
        {
            oxygine::spClipRectActor pClipRectActor = oxygine::spClipRectActor::create();
            pClipRectActor->setSize(m_widths[i] - DIVIDER_SIZE, ITEM_HEIGHT);
            pClipRectActor->setPosition(x + DIVIDER_SIZE, 0);
            pClipRectActor->addChild(item.items[i]);
            column->addChild(pClipRectActor);
            x += m_widths[i];
            // following horizintal line
            line = oxygine::spBox9Sprite::create();
            line->setResAnim(phAnim);
            line->setSize(DIVIDER_SIZE, ITEM_HEIGHT);
            line->setPosition(x, 0);
            m_HLines.append(line);
            column->addChild(line);
        }
        // vertical line after header
        line = oxygine::spBox9Sprite::create();
        line->setSize(getWidth(), DIVIDER_SIZE);
        line->setResAnim(pvAnim);
        line->setY(ITEM_HEIGHT);
        m_VLines.append(line);
        column->addChild(line);
        column->setY(y);
        m_pPanel->addItem(column);
        column->addClickListener([this, count](oxygine::Event*)
        {
            setCurrentItem(count);
            emit sigItemClicked();
        });
        m_columns.append(column);
        ++count;
        y += ITEM_HEIGHT + DIVIDER_SIZE;
    }
    m_pPanel->setContentHeigth(y + 10);
}

void ComplexTableView::setCurrentItem(qint32 item)
{
    QColor noColor{0, 0, 0, 0};
    for (qint32 i = 0; i < m_VLines.size(); i++)
    {
        if (i == item || i == item - 1)
        {
            m_VLines[i]->setAddColor(m_selectColor);
            m_VLines[i]->setPriority(1);
        }
        else
        {
            m_VLines[i]->setAddColor(noColor);
            m_VLines[i]->setPriority(0);
        }
    }
    const qint32 hCount = m_widths.size() + 1;
    for (qint32 i = 0; i < m_HLines.size(); i++)
    {
        if (i >= item * hCount &&
            i < (item + 1) * hCount)
        {
            m_HLines[i]->setAddColor(m_selectColor);
            m_HLines[i]->setPriority(1);
        }
        else
        {
            m_HLines[i]->setAddColor(noColor);
            m_HLines[i]->setPriority(0);
        }
    }
    m_currentItem = item;
}

void ComplexTableView::updateItemPositions()
{
    m_pPanel->clearContent();
    m_VLines.clear();
    m_HLines.clear();
    m_columns.clear();
    m_currentItem = -1;
    createItems();
}

qint32 ComplexTableView::getCurrentItem() const
{
    return m_currentItem;
}

const ComplexTableView::Item & ComplexTableView::getItem(qint32 i) const
{
    Q_ASSERT(i >= 0 && i < m_items.size());
    return m_items[i];
}
