#include "ui_reader/uifactory.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/label.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/timespinbox.h"
#include "objects/base/textbox.h"
#include "objects/base/panel.h"
#include "objects/base/slider.h"
#include "objects/base/moveinbutton.h"

#include "wiki/wikidatabase.h"

static const char* const itemLabel = "Label";
static const char* const itemCheckbox = "Checkbox";
static const char* const itemSpinbox = "Spinbox";
static const char* const itemTextbox = "Textbox";
static const char* const itemTimeSpinbox = "TimeSpinbox";
static const char* const itemPanel = "Panel";
static const char* const itemBox = "Box";
static const char* const itemIcon = "Icon";
static const char* const itemButton = "Button";
static const char* const itemIconButton = "IconButton";
static const char* const itemSlider = "Slider";
static const char* const itemMoveInButton = "MoveInButton";

static const char* const attrX = "x";
static const char* const attrY = "y";
static const char* const attrWidth = "width";
static const char* const attrHeight = "height";
static const char* const attrSize = "size";
static const char* const attrTooltip = "tooltip";
static const char* const attrOnEvent = "onEvent";
static const char* const attrStartValue = "startValue";
static const char* const attrText = "text";
static const char* const attrOnUpdate = "onUpdate";
static const char* const attrFont = "font";
static const char* const attrInfinite = "infinite";
static const char* const attrMin = "min";
static const char* const attrMax = "max";
static const char* const attrChilds = "childs";
static const char* const attrSprite = "sprite";
static const char* const attrUnit = "unit";
static const char* const attrId = "Id";
static const char* const attrEnabled = "enabled";
static const char* const attrDirection = "direction";
static const char* const attrScale = "scale";
static const char* const attrUseY = "useY";
static const char* const attrStartOffset = "startOffset";
static const char* const attrMoveInSize = "moveInSize";

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

UiFactory* UiFactory::m_pUiFactory;

UiFactory::UiFactory()
    : QObject()
{
    setObjectName("UiFactory");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    m_factoryItems.append({QString(itemLabel), std::bind(&UiFactory::createLabel, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemCheckbox), std::bind(&UiFactory::createCheckbox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemSpinbox), std::bind(&UiFactory::createSpinbox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemTextbox), std::bind(&UiFactory::createTextbox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemTimeSpinbox), std::bind(&UiFactory::createTimeSpinbox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemPanel), std::bind(&UiFactory::createPanel, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemIcon), std::bind(&UiFactory::createIcon, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemBox), std::bind(&UiFactory::createBox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemButton), std::bind(&UiFactory::createButton, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemIconButton), std::bind(&UiFactory::createIconButton, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemSlider), std::bind(&UiFactory::createSlider, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemMoveInButton), std::bind(&UiFactory::createMoveInButton, this, _1, _2, _3, _4)});

    connect(this, &UiFactory::sigDoEvent, this, &UiFactory::doEvent, Qt::QueuedConnection);
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

void UiFactory::createUi(QString uiXml, CreatedGui* pMenu)
{
    m_creationCount = 0;
    QStringList uiFiles;
    // make sure to overwrite existing js stuff
    for (qint32 i = Settings::getMods().size() - 1; i >= 0; --i)
    {
        uiFiles.append(Settings::getMods().at(i) + "/" + uiXml);
    }
    uiFiles.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/" + uiXml);
    uiFiles.append("resources/" + uiXml);
    for (const auto & uiFile : qAsConst(uiFiles))
    {
        if (QFile::exists(uiFile))
        {
            QDomDocument document;
            QFile file(uiFile);
            file.open(QIODevice::ReadOnly);
            QString error;
            qint32 line;
            qint32 column;
            bool loaded = document.setContent(&file, &error, &line, &column);
            if (loaded)
            {
                oxygine::spActor root = oxygine::spActor::create();
                bool success = true;
                auto rootElement = document.documentElement();
                auto node = rootElement.firstChild();
                while (!node.isNull())
                {
                    while (node.isComment())
                    {
                        node = node.nextSibling();
                    }
                    if (!node.isNull())
                    {
                        oxygine::spActor item;
                        success = createItem(root, node.toElement(), item, pMenu);
                    }
                    node = node.nextSibling();
                }
                if (success)
                {
                    pMenu->addFactoryUiItem(root);
                    break;
                }
                else
                {
                    CONSOLE_PRINT("Unable to load: " + uiFile, Console::eERROR);
                }
            }
            else
            {
                CONSOLE_PRINT("Unable to load: " + uiFile, Console::eERROR);
                CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
            }
        }
    }
}

bool UiFactory::createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu)
{
    QString name = element.nodeName();
    bool success = false;
    for (qint32 i = 0; i < m_factoryItems.size(); ++i)
    {
        if (m_factoryItems[i].m_id == name)
        {
            success = m_factoryItems[i].m_creator(parent, element, item, pMenu);
            break;
        }
    }
    if (!success)
    {
        CONSOLE_PRINT("Unable to create item: " + name + ".", Console::eERROR);
    }
    else
    {
        if (item.get() != nullptr)
        {
            pMenu->addFactoryUiItem(item);
        }
    }
    return success;
}

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrText, attrFont});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        qint32 width = getIntValue(getAttribute(childs, attrWidth));
        qint32 height = getIntValue(getAttribute(childs, attrHeight));
        QString text = translate(getAttribute(childs, attrText));
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        auto style = getStyle(getAttribute(childs, attrFont));
        QString id = getId(getAttribute(childs, attrId));
        spLabel pLabel = spLabel::create(width);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        pLabel->setHeight(height);
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setHtmlText(text);
        pLabel->setTooltipText(tooltip);
        pLabel->setObjectName(id);
        pLabel->setEnabled(enabled);
        QString onUpdateLine = getAttribute(childs, attrOnUpdate);
        if (!onUpdateLine.isEmpty())
        {
            Label* pPtr = pLabel.get();
            connect(pMenu, &CreatedGui::sigOnUpdate, pLabel.get(), [=]()
            {
                pPtr->setHtmlText(onUpdate<QString>(onUpdateLine));
            });
        }
        parent->addChild(pLabel);
        item = pLabel;

        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

bool UiFactory::createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrText, attrOnEvent});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        QString test = getAttribute(childs, attrWidth);
        qint32 width = -1;
        if (!test.isEmpty())
        {
            width = getIntValue(test);
        }
        QString text = translate(getAttribute(childs, attrText));
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        test = getAttribute(childs, attrSprite);
        QString sprite = "button";
        if (!test.isEmpty())
        {
            sprite = getStringValue(test);
        }
        oxygine::spButton pButton = ObjectManager::createButton(text, width, tooltip, sprite);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([=](oxygine::Event*)
        {
            emit sigDoEvent(onEvent);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrOnEvent});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        QString sprite = getStringValue(getAttribute(childs, attrSprite));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        oxygine::spButton pButton = ObjectManager::createIconButton(sprite, 36);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([=](oxygine::Event*)
        {
            emit sigDoEvent(onEvent);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, 30, 30);
    }
    return success;
}

bool UiFactory::createMoveInButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrMoveInSize});
    if (success)
    {
        qint32 moveInSize = getIntValue(getAttribute(childs, attrMoveInSize));
        qint32 direction = getIntValue(getAttribute(childs, attrDirection), -1);
        qint32 startOffset = getIntValue(getAttribute(childs, attrStartOffset), -1);
        float buttonScale = getIntValue(getAttribute(childs, attrScale), 2.0f);
        bool useY = getBoolValue(getAttribute(childs, attrUseY));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        auto pMoveButton = spMoveInButton::create(parent.get(), moveInSize, direction,
                                                 startOffset, buttonScale, useY);
        pMoveButton->setEnabled(enabled);
        parent->addChild(pMoveButton);
        item = pMoveButton;
    }
    return success;
}

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString id = getId(getAttribute(childs, attrId));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        bool value = getBoolValue(getAttribute(childs, attrStartValue));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setX(x);
        pCheckbox->setY(y);
        pCheckbox->setChecked(value);
        pCheckbox->setTooltipText(tooltip);
        pCheckbox->setObjectName(id);
        pCheckbox->setEnabled(enabled);
        parent->addChild(pCheckbox);
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [=](bool value)
        {
            onEvent(onEventLine, value);
        });
        item = pCheckbox;
        m_lastCoordinates = QRect(x, y, 40, 40);
    }
    return success;
}

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrInfinite, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 width = getIntValue(getAttribute(childs,attrWidth));
        qint32 min = getIntValue(getAttribute(childs,attrMin));
        qint32 max = getIntValue(getAttribute(childs,attrMax));
        qint32 infinite = getIntValue(getAttribute(childs,attrInfinite));
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue));
        QString id = getId(getAttribute(childs, attrId));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        spSpinBox pSpinBox = spSpinBox::create(width, min, max, SpinBox::Mode::Int);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setInfinityValue(infinite);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
        pSpinBox->setObjectName(id);
        pSpinBox->setEnabled(enabled);
        connect(pSpinBox.get(), &SpinBox::sigValueChanged, this, [=](qreal value)
        {
            onEvent(onEventLine, value);
        });
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getAttribute(childs, attrId));
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 width = getIntValue(getAttribute(childs,attrWidth));
        qint32 min = getIntValue(getAttribute(childs,attrMin));
        qint32 max = getIntValue(getAttribute(childs,attrMax));
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue));
        QString unit = "%";
        QString test = getAttribute(childs, attrUnit);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        if (!test.isEmpty())
        {
            unit = getStringValue(test);
        }
        spSlider pSlider = spSlider::create(width - 75, min, max, unit);
        pSlider->setX(x);
        pSlider->setY(y);
        pSlider->setTooltipText(tooltip);
        pSlider->setCurrentValue(value);
        pSlider->setObjectName(id);
        pSlider->setEnabled(enabled);
        connect(pSlider.get(), &Slider::sliderValueChanged, this, [=](qint32 value)
        {
            onEvent(onEventLine, value);
        });
        parent->addChild(pSlider);
        item = pSlider;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getAttribute(childs, attrId));
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 width = getIntValue(getAttribute(childs,attrWidth));
        qint32 height = -1;
        if (element.hasAttribute(attrHeight))
        {
            height = getIntValue(getAttribute(childs,attrHeight));
        }
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        QString value = getStringValue(getAttribute(childs,attrStartValue));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        spTextbox pTextbox = spTextbox::create(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
        pTextbox->setObjectName(id);
        pTextbox->setEnabled(enabled);
        connect(pTextbox.get(), &Textbox::sigTextChanged, this, [=](QString value)
        {
            onEvent(onEventLine, value);
        });
        parent->addChild(pTextbox);
        item = pTextbox;
        if (height <= 0)
        {
            height = 40;
        }
        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

bool UiFactory::createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getAttribute(childs, attrId));
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 width = getIntValue(getAttribute(childs,attrWidth));
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        spTimeSpinBox pSpinBox = spTimeSpinBox::create(width);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
        pSpinBox->setObjectName(id);
        pSpinBox->setEnabled(enabled);
        connect(pSpinBox.get(), &TimeSpinBox::sigValueChanged, this, [=](qint32 value)
        {
            onEvent(onEventLine, value);
        });
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSize, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 size = getIntValue(getAttribute(childs,attrSize));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), true);
        QString icon = getStringValue(getAttribute(childs, attrStartValue));
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        oxygine::spSprite pIcon = pWikiDatabase->getIcon(icon, size);
        pIcon->setPosition(x, y);
        parent->addChild(pIcon);
        pIcon->setEnabled(enabled);
        item = pIcon;
        m_lastCoordinates = QRect(x, y, size, size);
    }
    return success;
}

bool UiFactory::createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrChilds});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        qint32 width = getIntValue(getAttribute(childs, attrWidth));
        qint32 height = getIntValue(getAttribute(childs, attrHeight));
        QSize size = QSize(width, height);
        spPanel pPanel = spPanel::create(true, size, size);
        pPanel->setX(x);
        pPanel->setY(y);
        auto node = getNode(childs, attrChilds).firstChild();
        qint32 maxWidth = 0;
        qint32 maxHeight = 0;
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                oxygine::spActor panelItem;
                success = success && createItem(pPanel, node.toElement(), panelItem, pMenu);
                if (panelItem.get() != nullptr)
                {
                    pPanel->addItem(panelItem);
                }
                if (maxHeight < m_lastCoordinates.y() + m_lastCoordinates.height())
                {
                    maxHeight = m_lastCoordinates.y() + m_lastCoordinates.height();
                }
                if (maxWidth < m_lastCoordinates.x() + m_lastCoordinates.width())
                {
                    maxWidth = m_lastCoordinates.x() + m_lastCoordinates.width();
                }
            }
            node = node.nextSibling();
        }
        pPanel->setContentHeigth(maxHeight + 40);
        pPanel->setContentWidth(maxWidth + 40);
        parent->addChild(pPanel);
        item = pPanel;
        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

bool UiFactory::createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrSprite, attrChilds});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        qint32 width = getIntValue(getAttribute(childs, attrWidth));
        qint32 height = getIntValue(getAttribute(childs, attrHeight));
        QString spriteId = getStringValue(getAttribute(childs, attrSprite));
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::spBox9Sprite pPanel = oxygine::spBox9Sprite::create();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        pPanel->setResAnim(pAnim);
        pPanel->setX(x);
        pPanel->setY(y);
        pPanel->setSize(width, height);
        pPanel->setScale(1);
        auto node = getNode(childs, attrChilds).firstChild();
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                oxygine::spActor panelItem;
                success = success && createItem(pPanel, node.toElement(), panelItem, pMenu);
                if (panelItem.get() != nullptr)
                {
                    pPanel->addChild(panelItem);
                }
            }
            node = node.nextSibling();
        }
        parent->addChild(pPanel);
        item = pPanel;
        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

QString UiFactory::getAttribute(QDomNodeList childs, QString attribute)
{
    qint32 childCount = childs.count();
    for (qint32 i = 0; i < childCount; ++i)
    {
        if (childs.at(i).nodeName() == attribute)
        {
            QString value = childs.at(i).toElement().text();
            return value;
        }
    }
    return "";
}

QDomNode UiFactory::getNode(QDomNodeList childs, QString attribute)
{
    qint32 childCount = childs.count();
    for (qint32 i = 0; i < childCount; ++i)
    {
        if (childs.at(i).nodeName() == attribute)
        {
            return childs.at(i);
        }
    }
    return QDomElement();
}

bool UiFactory::checkElements(QDomNodeList childs, const QStringList & attributes)
{
    bool ret = true;
    qint32 childCount = childs.count();
    for (const auto & attr : qAsConst(attributes))
    {
        for (qint32 i = 0; i < childCount; ++i)
        {
            if (childs.at(i).nodeName() == attr)
            {
                break;
            }
            else if (i == childCount - 1)
            {
                CONSOLE_PRINT("Missing attribute: " + attr, Console::eERROR);
                ret = false;
            }
        }
    }
    return ret;
}

qint32 UiFactory::getIntValue(QString line, qint32 defaultValue)
{
    qint32 value = defaultValue;
    if (!line.isEmpty())
    {
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";";
        QJSValue erg = Interpreter::getInstance()->evaluate(coordinates + line);
        if (erg.isNumber())
        {
            value = erg.toInt();
        }
        else if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Unable to determine a int value while interpreting. Line: " + line, Console::eERROR);
        }
    }
    return value;
}

float UiFactory::getFloatValue(QString line, float defaultValue)
{
    float value = defaultValue;
    if (!line.isEmpty())
    {
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";";
        QJSValue erg = Interpreter::getInstance()->evaluate(coordinates + line);
        if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        else if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Unable to determine a int value while interpreting. Line: " + line, Console::eERROR);
        }
    }
    return value;
}

bool UiFactory::getBoolValue(QString line, bool defaultValue)
{
    bool value = defaultValue;
    if (!line.isEmpty())
    {
        QJSValue erg = Interpreter::getInstance()->evaluate(line);
        if (erg.isBool())
        {
            value = erg.toBool();
        }
        else if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Unable to determine a bool value while interpreting. Line: " + line, Console::eERROR);
        }
    }
    return value;
}

QString UiFactory::getStringValue(QString line)
{
    QString value;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            value = erg.toString();
        }
    }
    return value;
}

oxygine::TextStyle UiFactory::getStyle(QString styleName)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getInstance()->getResFont(styleName));
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    return style;
}

QString UiFactory::getId(QString attribute)
{
    QString ret = "object" + QString::number(m_creationCount);
    if (attribute.isEmpty())
    {
        ret = attribute;
    }
    else
    {
        ++m_creationCount;
    }
    return ret;
}

QString UiFactory::translate(QString line)
{
    line = line.replace("QT_TRANSLATE_NOOP(\"GAME\", \"", "");
    line = line.replace("QT_TRANSLATE_NOOP(\"GAME\",\"", "");
    line = line.trimmed();
    line.remove(line.length() - 2, 2);
    return Mainapp::qsTr(line);
}

void UiFactory::doEvent(QString command)
{
    QJSValue erg = Interpreter::getInstance()->evaluate(command);
    if (erg.isError())
    {
        CONSOLE_PRINT("Error while parsing " + command + " Error: " + erg.toString() + ".", Console::eERROR);
    }
}
