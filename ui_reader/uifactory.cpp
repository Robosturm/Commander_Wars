#include "ui_reader/uifactory.h"

#include "resource_management/fontmanager.h"

#include "objects/base/label.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

UiFactory UiFactory::m_pUiFactory;

UiFactory::UiFactory()
    : QObject()
{
    m_factoryItems.append({QString("Label"), std::bind(&UiFactory::createLabel, this, _1, _2, _3)});
    m_factoryItems.append({QString("Checkbox"), std::bind(&UiFactory::createCheckbox, this, _1, _2, _3)});
    m_factoryItems.append({QString("Spinbox"), std::bind(&UiFactory::createSpinbox, this, _1, _2, _3)});
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

oxygine::spActor UiFactory::createUi(QString uiXmlFile, bool & success)
{
    oxygine::spActor root = oxygine::spActor::create();
    success = false;
    if (QFile::exists(uiXmlFile))
    {
        QDomDocument document;
        QFile file(uiXmlFile);
        file.open(QIODevice::ReadOnly);
        bool loaded = document.setContent(&file);
        if (loaded)
        {
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
                    createItem(root, node.toElement(), item);
                }
                node = node.nextSibling();
            }
            success = true;
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
    return root;
}

bool UiFactory::createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor item)
{
    QString name = element.nodeName();
    bool success = false;
    for (qint32 i = 0; i < m_factoryItems.size(); ++i)
    {
        if (m_factoryItems[i].m_id == name)
        {
            success = m_factoryItems[i].m_creator(parent, element, item);
            break;
        }
    }
    if (!success)
    {
        Console::print("Unable to create item: " + name + ".", Console::eERROR);
    }
    return success;
}

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor item)
{
    qint32 x = getIntValue(element.attribute("x"));
    qint32 y = getIntValue(element.attribute("y"));
    qint32 width = getIntValue(element.attribute("width"));
    qint32 height = getIntValue(element.attribute("height"));
    QString text = element.attribute("text");
    QString tooltip = element.attribute("tooltip");
    auto style = getStyle(element.attribute("font"));
    spLabel pLabel = spLabel::create(width);
    pLabel->setHeight(height);
    pLabel->setX(x);
    pLabel->setY(y);
    pLabel->setStyle(style);
    pLabel->setText(text);
    pLabel->setTooltipText(tooltip);
    parent->addChild(pLabel);
    item = pLabel;
    return true;
}

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item)
{
    qint32 x = getIntValue(element.attribute("x"));
    qint32 y = getIntValue(element.attribute("y"));
    QString tooltip = element.attribute("tooltip");
    QString onEventLine = element.attribute("onEvent");
    bool value = getBoolValue(element.attribute("startValue"));
    spCheckbox pCheckbox = spCheckbox::create();
    pCheckbox->setX(x);
    pCheckbox->setY(y);
    pCheckbox->setChecked(value);
    pCheckbox->setTooltipText(tooltip);
    parent->addChild(pCheckbox);
    connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
    {
        onEvent(onEventLine, value);
    });
    item = pCheckbox;
    return true;
}

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item)
{
    qint32 x = getIntValue(element.attribute("x"));
    qint32 y = getIntValue(element.attribute("y"));
    qint32 width = getIntValue(element.attribute("width"));
    qint32 min = getIntValue(element.attribute("min"));
    qint32 max = getIntValue(element.attribute("max"));
    qint32 infinite = getIntValue(element.attribute("infinite"));
    QString tooltip = element.attribute("tooltip");
    QString onEventLine = element.attribute("onEvent");
    qint32 value = getBoolValue(element.attribute("startValue"));
    spSpinBox pSpinBox = spSpinBox::create(width, min, max, SpinBox::Mode::Int);
    pSpinBox->setX(x);
    pSpinBox->setY(y);
    pSpinBox->setInfinityValue(infinite);
    pSpinBox->setTooltipText(tooltip);
    pSpinBox->setCurrentValue(value);
    connect(pSpinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        onEvent(onEventLine, value);
    });
    parent->addChild(pSpinBox);
    item = pSpinBox;
    return true;
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

