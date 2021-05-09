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

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

UiFactory* UiFactory::m_pUiFactory;

UiFactory::UiFactory()
    : QObject()
{
    setObjectName("UiFactory");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
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

    connect(this, &UiFactory::sigDoEvent, this, &UiFactory::doEvent, Qt::QueuedConnection);
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

void UiFactory::createUi(QString uiXml, Basemenu* pMenu)
{
    QStringList uiFiles;
    uiFiles.append("resources/" + uiXml);
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        uiFiles.append(Settings::getMods().at(i) + "/" + uiXml);
    }
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
                }
                else
                {
                    Console::print("Unable to load: " + uiFile + ".", Console::eERROR);
                }
            }
            else
            {
                Console::print("Unable to load: " + uiFile + ".", Console::eERROR);
                Console::print("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
            }
        }
    }
}

bool UiFactory::createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu)
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
        Console::print("Unable to create item: " + name + ".", Console::eERROR);
    }
    return success;
}

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu)
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
        spLabel pLabel = spLabel::create(width);
        pLabel->setHeight(height);
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setText(text);
        pLabel->setTooltipText(tooltip);
        QString onUpdateLine = getAttribute(childs, attrOnUpdate);
        if (!onUpdateLine.isEmpty())
        {
            connect(pMenu, &Basemenu::sigOnUpdate, pLabel.get(), [=]()
            {
                pLabel->setText(onUpdate<QString>(onUpdateLine));
            });
        }
        parent->addChild(pLabel);
        item = pLabel;

        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

bool UiFactory::createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
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
        test = getAttribute(childs, attrSprite);
        QString sprite = "button";
        if (!test.isEmpty())
        {
            sprite = getStringValue(test);
        }
        oxygine::spButton pButton = ObjectManager::createButton(text, width, tooltip, sprite);
        pButton->setX(x);
        pButton->setY(y);
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

bool UiFactory::createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrOnEvent});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        QString sprite = getStringValue(getAttribute(childs, attrSprite));
        oxygine::spButton pButton = ObjectManager::createIconButton(sprite);
        pButton->setX(x);
        pButton->setY(y);
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

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs, attrX));
        qint32 y = getIntValue(getAttribute(childs, attrY));
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        bool value = getBoolValue(getAttribute(childs, attrStartValue));
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setX(x);
        pCheckbox->setY(y);
        pCheckbox->setChecked(value);
        pCheckbox->setTooltipText(tooltip);
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

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
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
        spSpinBox pSpinBox = spSpinBox::create(width, min, max, SpinBox::Mode::Int);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setInfinityValue(infinite);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
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

bool UiFactory::createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrOnEvent, attrStartValue});
    if (success)
    {
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
        if (!test.isEmpty())
        {
            unit = getStringValue(test);
        }
        spSlider pSlider = spSlider::create(width - 75, min, max, unit);
        pSlider->setX(x);
        pSlider->setY(y);
        pSlider->setTooltipText(tooltip);
        pSlider->setCurrentValue(value);
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

bool UiFactory::createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
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
        spTextbox pTextbox = new Textbox(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
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

bool UiFactory::createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 width = getIntValue(getAttribute(childs,attrWidth));
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue));
        spTimeSpinBox pSpinBox = spTimeSpinBox::create(width);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
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

bool UiFactory::createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSize, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(getAttribute(childs,attrX));
        qint32 y = getIntValue(getAttribute(childs,attrY));
        qint32 size = getIntValue(getAttribute(childs,attrSize));
        QString icon = getStringValue(getAttribute(childs, attrStartValue));
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        oxygine::spSprite pIcon = pWikiDatabase->getIcon(icon, size);
        pIcon->setPosition(x, y);
        parent->addChild(pIcon);
        item = pIcon;
        m_lastCoordinates = QRect(x, y, size, size);
    }
    return success;
}

bool UiFactory::createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu)
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
                success = success && createItem(parent, node.toElement(), panelItem, pMenu);
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

bool UiFactory::createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu)
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
        QSize size = QSize(width, height);
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::spBox9Sprite pPanel = oxygine::spBox9Sprite::create();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        pPanel->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pPanel->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pPanel->setResAnim(pAnim);
        pPanel->setX(x);
        pPanel->setY(y);
        pPanel->setSize(width, height);
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
                success = success && createItem(parent, node.toElement(), panelItem, pMenu);
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

bool UiFactory::checkElements(QDomNodeList childs, QVector<QString> attributes)
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
            else if (i == childCount)
            {
                Console::print("Missing attribute: " + attr, Console::eERROR);
                ret = false;
            }
        }
    }
    return ret;
}

qint32 UiFactory::getIntValue(QString line)
{
    QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";"
                          "var lastY = " + QString::number(m_lastCoordinates.y()) + ";"
                          "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";"
                          "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";";
    QJSValue erg = Interpreter::getInstance()->evaluate(coordinates + line);
    qint32 value = 0;
    if (erg.isNumber())
    {
        value = erg.toInt();
    }
    else if (erg.isError())
    {
        Console::print("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
    }
    else
    {
        Console::print("Unable to determine a int value while interpreting. Line: " + line, Console::eERROR);
    }
    return value;
}

bool UiFactory::getBoolValue(QString line)
{
    QJSValue erg = Interpreter::getInstance()->evaluate(line);
    bool value = false;
    if (erg.isBool())
    {
        value = erg.toBool();
    }
    else if (erg.isError())
    {
        Console::print("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
    }
    else
    {
        Console::print("Unable to determine a bool value while interpreting. Line: " + line, Console::eERROR);
    }
    return value;
}

QString UiFactory::getStringValue(QString line)
{
    QJSValue erg = Interpreter::getInstance()->evaluate(line);
    QString value;
    if (erg.isError())
    {
        Console::print("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
    }
    else
    {
        value = erg.toString();
    }
    return value;
}

oxygine::TextStyle UiFactory::getStyle(QString styleName)
{
    oxygine::TextStyle style = FontManager::getInstance()->getResFont(styleName);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    return style;
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
        Console::print("Error while parsing " + command + " Error: " + erg.toString() + ".", Console::eERROR);
    }
}
