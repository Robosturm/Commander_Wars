#include "ui_reader/uifactory.h"

#include "resource_management/fontmanager.h"

#include "objects/base/label.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"

static const char* const itemLabel = "Label";
static const char* const itemCheckbox = "Checkbox";
static const char* const itemSpinbox = "Spinbox";

static const char* const attrX = "x";
static const char* const attrY = "y";
static const char* const attrWidth = "width";
static const char* const attrHeight = "height";
static const char* const attrTooltip = "tooltip";
static const char* const attrOnEvent = "onEvent";
static const char* const attrStartValue = "startValue";
static const char* const attrText = "text";
static const char* const attrOnUpdate = "onUpdate";
static const char* const attrFont = "font";
static const char* const attrInfinite = "infinite";
static const char* const attrMin = "min";
static const char* const attrMax = "max";

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

UiFactory UiFactory::m_pUiFactory;

UiFactory::UiFactory()
    : QObject()
{
    m_factoryItems.append({QString(itemLabel), std::bind(&UiFactory::createLabel, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemCheckbox), std::bind(&UiFactory::createCheckbox, this, _1, _2, _3, _4)});
    m_factoryItems.append({QString(itemSpinbox), std::bind(&UiFactory::createSpinbox, this, _1, _2, _3, _4)});
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

void UiFactory::createUi(QString uiXmlFile, Basemenu* pMenu)
{
    if (QFile::exists(uiXmlFile))
    {
        QDomDocument document;
        QFile file(uiXmlFile);
        file.open(QIODevice::ReadOnly);
        bool loaded = document.setContent(&file);
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
                pMenu->addChild(root);
            }
            else
            {
                Console::print("Unable to load: " + uiXmlFile + ".", Console::eERROR);
            }
        }
        else
        {
            Console::print("Unable to load: " + uiXmlFile + ".", Console::eERROR);
        }
    }
    else
    {
        Console::print("Unable to locate: " + uiXmlFile + " while loading ui.", Console::eDEBUG);
    }
}

bool UiFactory::createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor item, Basemenu* pMenu)
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

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor item, Basemenu* pMenu)
{
    bool success = checkElement(element, {attrX, attrY, attrWidth, attrHeight, attrText, attrFont});
    if (success)
    {
        qint32 x = getIntValue(element.attribute(attrX));
        qint32 y = getIntValue(element.attribute(attrY));
        qint32 width = getIntValue(element.attribute(attrWidth));
        qint32 height = getIntValue(element.attribute(attrHeight));
        QString text = element.attribute(attrText);
        QString tooltip = element.attribute(attrTooltip);
        auto style = getStyle(element.attribute(attrFont));
        spLabel pLabel = spLabel::create(width);
        pLabel->setHeight(height);
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setText(text);
        pLabel->setTooltipText(tooltip);
        if (element.hasAttribute(attrOnUpdate))
        {
            QString onUpdateLine = element.attribute(attrOnUpdate);
            connect(pMenu, &Basemenu::sigOnUpdate, pLabel.get(), [=]()
            {
                pLabel->setText(onUpdate<QString>(onUpdateLine));
            });
        }
        parent->addChild(pLabel);
        item = pLabel;
    }
    return success;
}

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item, Basemenu*)
{
    bool success = checkElement(element, {attrX, attrY, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(element.attribute(attrX));
        qint32 y = getIntValue(element.attribute(attrY));
        QString tooltip = element.attribute(attrTooltip);
        QString onEventLine = element.attribute(attrOnEvent);
        bool value = getBoolValue(element.attribute(attrStartValue));
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
    }
    return success;
}

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item, Basemenu*)
{
    bool success = checkElement(element, {attrX, attrY, attrWidth, attrMin,
                              attrMax, attrInfinite, attrOnEvent, attrStartValue});
    if (success)
    {
        qint32 x = getIntValue(element.attribute(attrX));
        qint32 y = getIntValue(element.attribute(attrY));
        qint32 width = getIntValue(element.attribute(attrWidth));
        qint32 min = getIntValue(element.attribute(attrMin));
        qint32 max = getIntValue(element.attribute(attrMax));
        qint32 infinite = getIntValue(element.attribute(attrInfinite));
        QString tooltip = element.attribute(attrTooltip);
        QString onEventLine = element.attribute(attrOnEvent);
        qint32 value = getBoolValue(element.attribute(attrStartValue));
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
    }
    return success;
}

bool UiFactory::checkElement(QDomElement element, QVector<QString> attributes)
{
    bool ret = true;
    for (const auto & attr : attributes)
    {
        if (!element.hasAttribute(attr))
        {
            Console::print("Missing attribute: " + attr, Console::eERROR);
            ret = false;
        }
    }
    return ret;
}

qint32 UiFactory::getIntValue(QString line)
{
    QJSValue erg = Interpreter::getInstance()->evaluate(line);
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
    oxygine::TextStyle style;
    if (styleName == "font16")
    {
        style = FontManager::getMainFont16();
    }
    else if (styleName == "font48")
    {
        style = FontManager::getMainFont48();
    }
    else if (styleName == "font72")
    {
        style = FontManager::getMainFont72();
    }
    else
    {
       style = FontManager::getMainFont24();
    }
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    return style;
}

