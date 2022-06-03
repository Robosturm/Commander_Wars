#include "ui_reader/uifactory.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/terrainmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/label.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/timespinbox.h"
#include "objects/base/textbox.h"
#include "objects/base/panel.h"
#include "objects/base/slider.h"
#include "objects/base/moveinbutton.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/dropdownmenucolor.h"

#include "game/gamemap.h"

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
static const char* const itemSprite = "Sprite";
static const char* const itemTextField = "TextField";
static const char* const itemDropDownMenu = "DropDownMenu";
static const char* const itemDropDownMenuColor = "DropDownMenuColor";
static const char* const itemDropDownMenuSprite = "DropDownMenuSprite";
static const char* const itemLoop = "loop";
static const char* const itemIf = "if";

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
static const char* const attrFontColor = "fontColor";
static const char* const attrFontScale = "fontScale";
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
static const char* const attrItems = "items";
static const char* const attrSpriteType = "spriteType";
static const char* const attrSpriteSize = "spriteSize";
static const char* const attrPlayer = "player";

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

UiFactory* UiFactory::m_pUiFactory;

UiFactory::UiFactory()
{
    setObjectName("UiFactory");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    m_factoryItems.append({QString(itemLabel), std::bind(&UiFactory::createLabel, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemCheckbox), std::bind(&UiFactory::createCheckbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemSpinbox), std::bind(&UiFactory::createSpinbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemTextbox), std::bind(&UiFactory::createTextbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemTimeSpinbox), std::bind(&UiFactory::createTimeSpinbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemPanel), std::bind(&UiFactory::createPanel, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemIcon), std::bind(&UiFactory::createIcon, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemBox), std::bind(&UiFactory::createBox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemButton), std::bind(&UiFactory::createButton, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemIconButton), std::bind(&UiFactory::createIconButton, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemSlider), std::bind(&UiFactory::createSlider, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemMoveInButton), std::bind(&UiFactory::createMoveInButton, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemSprite), std::bind(&UiFactory::createSprite, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemTextField), std::bind(&UiFactory::createTextfield, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemDropDownMenu), std::bind(&UiFactory::createDropDownMenu, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemDropDownMenuSprite), std::bind(&UiFactory::createDropDownMenuSprite, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemLoop), std::bind(&UiFactory::loop, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemDropDownMenuColor), std::bind(&UiFactory::createDropDownMenuColor, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemIf), std::bind(&UiFactory::ifCondition, this, _1, _2, _3, _4, _5)});

    connect(this, &UiFactory::sigDoEvent, this, &UiFactory::doEvent, Qt::QueuedConnection);
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

void UiFactory::shutdown()
{
    delete m_pUiFactory;
    m_pUiFactory = nullptr;
}

void UiFactory::createUi(QString uiXml, CreatedGui* pMenu)
{
    if (m_dropDownPlayer.get() == nullptr)
    {
        m_dropDownPlayer = spPlayer::create(nullptr);
        m_dropDownPlayer->init();
    }
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

bool UiFactory::loop(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    qint32 loopCount = getIntValue(element.attribute("count"), "", loopIdx);
    bool success = loopCount > 0;
    Panel* pPanel = dynamic_cast<Panel*>(parent.get());
    for (qint32 i = 0; i < loopCount; ++i)
    {
        auto node = element.firstChild();
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                oxygine::spActor loopItem;
                success = success && createItem(parent, node.toElement(), loopItem, pMenu, i);
                if (loopItem.get() != nullptr && pPanel != nullptr)
                {
                    pPanel->addItem(loopItem);
                }
            }
            node = node.nextSibling();
        }
    }
    return success;
}

bool UiFactory::ifCondition(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    bool create = getBoolValue(element.attribute("condition"), "", loopIdx);
    bool success = true;
    if (create)
    {
        Panel* pPanel = dynamic_cast<Panel*>(parent.get());
        auto node = element.firstChild();
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                oxygine::spActor loopItem;
                success = success && createItem(parent, node.toElement(), loopItem, pMenu, loopIdx);
                if (loopItem.get() != nullptr && pPanel != nullptr)
                {
                    pPanel->addItem(loopItem);
                }
            }
            node = node.nextSibling();
        }
    }
    return success;
}

bool UiFactory::createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    QString name = element.nodeName();
    bool success = false;
    for (auto & itemFunc : m_factoryItems)
    {
        if (itemFunc.m_id == name)
        {
            success = itemFunc.m_creator(parent, element, item, pMenu, loopIdx);
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

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrText, attrFont});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx);
        QString text = translate(getStringValue(getAttribute(childs, attrText), id, loopIdx));
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx));
        QString fontColor = getStringValue(getAttribute(childs, attrFontColor), id, loopIdx);
        if (fontColor.isEmpty())
        {
            fontColor = FontManager::getFontColor().name();
        }
        auto style = getStyle(getStringValue(getAttribute(childs, attrFont), id, loopIdx), fontColor);
        float fontScale = getFloatValue(getAttribute(childs, attrFontScale), id, loopIdx, 1.0f);
        spLabel pLabel = spLabel::create(width);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        pLabel->setHeight(height);
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setScale(fontScale);
        pLabel->setHtmlText(text);
        pLabel->setTooltipText(tooltip);
        pLabel->setObjectName(id);
        pLabel->setEnabled(enabled);
        QString onUpdateLine = getAttribute(childs, attrOnUpdate);
        if (!onUpdateLine.isEmpty())
        {
            Label* pPtr = pLabel.get();
            connect(pMenu, &CreatedGui::sigOnUpdate, pLabel.get(), [this, pPtr, onUpdateLine, id, loopIdx]()
            {
                pPtr->setHtmlText(onUpdate<QString>(onUpdateLine, id, loopIdx));
            }, Qt::QueuedConnection);
        }
        parent->addChild(pLabel);
        item = pLabel;

        m_lastCoordinates = QRect(x, y, width, height);
    }
    return success;
}

bool UiFactory::createTextfield(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrText, attrFont});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        QString text = translate(getAttribute(childs, attrText));
        QString fontColor = getStringValue(getAttribute(childs, attrFontColor), id, loopIdx);
        if (fontColor.isEmpty())
        {
            fontColor = FontManager::getFontColor().name();
        }
        auto style = getStyle(getStringValue(getAttribute(childs, attrFont), id, loopIdx), fontColor);
        float fontScale = getFloatValue(getAttribute(childs, attrFontScale), id, loopIdx, 1.0f);
        oxygine::spTextField pLabel = oxygine::spTextField::create();
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setScale(fontScale);
        pLabel->setHtmlText(text);
        parent->addChild(pLabel);
        item = pLabel;
        const auto & textRect = pLabel->getTextRect();
        m_lastCoordinates = QRect(x, y, textRect.getWidth(), textRect.getHeight());
    }
    return success;
}

bool UiFactory::createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrText, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        QString test = getAttribute(childs, attrWidth);
        qint32 width = -1;
        if (!test.isEmpty())
        {
            width = getIntValue(test, id, loopIdx);
        }
        QString text = translate(getAttribute(childs, attrText));
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        test = getAttribute(childs, attrSprite);
        QString sprite = "button";
        if (!test.isEmpty())
        {
            sprite = getStringValue(test, id, loopIdx);
        }
        oxygine::spButton pButton = ObjectManager::createButton(text, width, tooltip, sprite);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([this, onEvent, id, loopIdx](oxygine::Event*)
        {
            emit sigDoEvent(onEvent, id, loopIdx);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        QString sprite = getStringValue(getAttribute(childs, attrSprite), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        oxygine::spButton pButton = ObjectManager::createIconButton(sprite, 36);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([this, onEvent, id, loopIdx](oxygine::Event*)
        {
            emit sigDoEvent(onEvent, id, loopIdx);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, 30, 30);
    }
    return success;
}

bool UiFactory::createMoveInButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrMoveInSize});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 moveInSize = getIntValue(getAttribute(childs, attrMoveInSize), id, loopIdx);
        qint32 direction = getIntValue(getAttribute(childs, attrDirection), id, loopIdx, -1);
        qint32 startOffset = getIntValue(getAttribute(childs, attrStartOffset), id, loopIdx, -1);
        float buttonScale = getIntValue(getAttribute(childs, attrScale), id, loopIdx, 2.0f);
        bool useY = getBoolValue(getAttribute(childs, attrUseY), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        auto pMoveButton = spMoveInButton::create(parent.get(), moveInSize, direction,
                                                 startOffset, buttonScale, useY);
        pMoveButton->setEnabled(enabled);
        parent->addChild(pMoveButton);
        item = pMoveButton;
    }
    return success;
}

bool UiFactory::createSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrScale});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        QString spriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx);
        float scale = getFloatValue(getAttribute(childs, attrScale), id, loopIdx);
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        pSprite->setPosition(x, y);
        parent->addChild(pSprite);
        item = pSprite;
        m_lastCoordinates = QRect(x, y, 40, 40);
    }
    return success;
}

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        bool value = getBoolValue(getAttribute(childs, attrStartValue), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setX(x);
        pCheckbox->setY(y);
        pCheckbox->setChecked(value);
        pCheckbox->setTooltipText(tooltip);
        pCheckbox->setObjectName(id);
        pCheckbox->setEnabled(enabled);
        parent->addChild(pCheckbox);
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, onEventLine, id, loopIdx](bool value)
        {
            onEvent(onEventLine, value, id, loopIdx);
        }, Qt::QueuedConnection);
        item = pCheckbox;
        m_lastCoordinates = QRect(x, y, 40, 40);
    }
    return success;
}

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrInfinite, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx);
        qint32 min = getIntValue(getAttribute(childs,attrMin), id, loopIdx);
        qint32 max = getIntValue(getAttribute(childs,attrMax), id, loopIdx);
        qint32 infinite = getIntValue(getAttribute(childs, attrInfinite), id, loopIdx);
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        spSpinBox pSpinBox = spSpinBox::create(width, min, max, SpinBox::Mode::Int);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setInfinityValue(infinite);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
        pSpinBox->setObjectName(id);
        pSpinBox->setEnabled(enabled);
        connect(pSpinBox.get(), &SpinBox::sigValueChanged, this, [this, onEventLine, id, loopIdx](qreal value)
        {
            onEvent(onEventLine, value, id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx);
        qint32 min = getIntValue(getAttribute(childs,attrMin), id, loopIdx);
        qint32 max = getIntValue(getAttribute(childs,attrMax), id, loopIdx);
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue), id, loopIdx);
        QString unit = "%";
        QString test = getAttribute(childs, attrUnit);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        if (!test.isEmpty())
        {
            unit = getStringValue(test, id, loopIdx);
        }
        spSlider pSlider = spSlider::create(width - 75, min, max, unit);
        pSlider->setX(x);
        pSlider->setY(y);
        pSlider->setTooltipText(tooltip);
        pSlider->setCurrentValue(value);
        pSlider->setObjectName(id);
        pSlider->setEnabled(enabled);
        connect(pSlider.get(), &Slider::sliderValueChanged, this, [this, onEventLine, id, loopIdx](qint32 value)
        {
            onEvent(onEventLine, value, id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pSlider);
        item = pSlider;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        qint32 height = -1;
        if (hasChild(childs, attrHeight))
        {
            height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx);
        }
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        spTextbox pTextbox = spTextbox::create(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
        pTextbox->setObjectName(id);
        pTextbox->setEnabled(enabled);
        connect(pTextbox.get(), &Textbox::sigTextChanged, this, [this, onEventLine, id, loopIdx](QString value)
        {
            onEvent(onEventLine, value, id, loopIdx);
        }, Qt::QueuedConnection);
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

bool UiFactory::createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx);
        QString tooltip = translate(getAttribute(childs,attrTooltip));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs, attrStartValue), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        spTimeSpinBox pSpinBox = spTimeSpinBox::create(width);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
        pSpinBox->setObjectName(id);
        pSpinBox->setEnabled(enabled);
        connect(pSpinBox.get(), &TimeSpinBox::sigValueChanged, this, [this, onEventLine, id, loopIdx](qint32 value)
        {
            onEvent(onEventLine, value, id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui*, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSize, attrSprite});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx);
        qint32 size = getIntValue(getAttribute(childs,attrSize), id, loopIdx);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, true);
        QString icon = getStringValue(getAttribute(childs, attrSprite), id, loopIdx);
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        Player* pPlayer = getPlayerValue(getAttribute(childs, attrPlayer), id, loopIdx);;
        oxygine::spSprite pIcon = pWikiDatabase->getIcon(nullptr, icon, size, pPlayer);
        pIcon->setPosition(x, y);
        parent->addChild(pIcon);
        pIcon->setEnabled(enabled);
        item = pIcon;
        m_lastCoordinates = QRect(x, y, size, size);
    }
    return success;
}

bool UiFactory::createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrChilds});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx);
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
                success = success && createItem(pPanel, node.toElement(), panelItem, pMenu, loopIdx);
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

bool UiFactory::createDropDownMenu(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value;
        if (hasChild(childs, attrStartValue))
        {
            value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx);
        }
        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx);
        spDropDownmenu pDropDownmenu = spDropDownmenu::create(width, items);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        if (!value.isEmpty())
        {
            pDropDownmenu->setCurrentItem(value);
        }
        DropDownmenu* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, [this, onEventLine, pDropDownmenuPtr, id, loopIdx](qint32 value)
        {
            onEvent(onEventLine, value, pDropDownmenuPtr->getCurrentItemText(), id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createDropDownMenuColor(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value;
        if (hasChild(childs, attrStartValue))
        {
            value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx);
        }
        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx);
        QVector<QColor> colorItems;
        colorItems.reserve(items.size());
        for (auto & item : items)
        {
            colorItems.append(QColor(item));
        }
        spDropDownmenuColor pDropDownmenu = spDropDownmenuColor::create(width, colorItems);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        if (!value.isEmpty())
        {
            pDropDownmenu->setCurrentItem(value);
        }
        DropDownmenuColor* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenuColor::sigItemChanged, this, [this, onEventLine, pDropDownmenuPtr, id, loopIdx](QColor value)
        {
            onEvent(onEventLine, value.name(), id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createDropDownMenuSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems, attrSpriteType});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        qint32 spriteSize = getIntValue(getAttribute(childs, attrSpriteSize), id, loopIdx);
        QString tooltip = translate(getAttribute(childs, attrTooltip));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value;
        if (hasChild(childs, attrStartValue))
        {
            value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx);
        }
        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx);
        QString spriteCreator = getStringValue(getAttribute(childs, attrSpriteType), id, loopIdx);

        std::function<oxygine::spActor(QString item)> creator;
        if (spriteCreator == "unit")
        {
            Player* pPlayer = m_dropDownPlayer.get();
            creator = [this, pPlayer](QString id)
            {
                spUnit pSprite = spUnit::create(id, pPlayer, false, nullptr);
                return pSprite;
            };
        }
        else if (spriteCreator == "co")
        {
            COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
            creator = [this, pCOSpriteManager](QString id)
            {
                oxygine::ResAnim* pAnim = nullptr;
                if (id.isEmpty())
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+info");
                }
                else
                {
                    pAnim = pCOSpriteManager->getResAnim(id + "+info");
                }
                oxygine::spSprite pSprite = oxygine::spSprite::create();
                pSprite->setResAnim(pAnim);
                pSprite->setScale(pAnim->getWidth() / 32.0f);
                pSprite->setSize(pAnim->getSize());
                return pSprite;
            };
        }
        else
        {
            BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
            QStringList buildingIds = pBuildingSpriteManager->getLoadedBuildings();
            creator = [this, buildingIds](QString id)
            {
                QString terrainId = GameMap::PLAINS;
                bool isBuilding = buildingIds.contains(id);
                if (!isBuilding)
                {
                    terrainId = id;
                }
                spTerrain pTerrain = Terrain::createTerrain(terrainId, -1, -1, "", nullptr);
                pTerrain->loadSprites();
                pTerrain->setPriority(-100);
                oxygine::spSprite pRet = pTerrain;
                if (isBuilding)
                {
                    spBuilding building = spBuilding::create(id, nullptr);
                    building->setTooltipText(building->getName());
                    building->updateBuildingSprites(false);
                    building->addChild(pTerrain);
                    pRet = building;
                }
                return pRet;
            };
        }
        spDropDownmenuSprite pDropDownmenu = spDropDownmenuSprite::create(width, items, creator, spriteSize);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        if (!value.isEmpty())
        {
            pDropDownmenu->setCurrentItem(value);
        }
        DropDownmenuSprite* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenuSprite::sigItemChanged, this, [this, onEventLine, pDropDownmenuPtr, id, loopIdx](qint32 value)
        {
            onEvent(onEventLine, value, pDropDownmenuPtr->getCurrentItemText(), id, loopIdx);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, width, 40);
    }
    return success;
}

bool UiFactory::createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrSprite, attrChilds});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx);
        QString spriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx);
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

QString UiFactory::getAttribute(const QDomNodeList & childs, const QString & attribute)
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

QDomNode UiFactory::getNode(const QDomNodeList & childs, const QString & attribute)
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

bool UiFactory::checkElements(const QDomNodeList & childs, const QStringList & attributes)
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

bool UiFactory::hasChild(const QDomNodeList & childs, const QString & attribute)
{
    qint32 childCount = childs.count();
    for (qint32 i = 0; i < childCount; ++i)
    {
        if (childs.at(i).nodeName() == attribute)
        {
            return true;
        }
    }
    return false;
}

qint32 UiFactory::getIntValue(QString line, QString objectId, qint32 loopIdx, qint32 defaultValue)
{
    qint32 value = defaultValue;
    if (!line.isEmpty())
    {
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";"; +
                              "var objectId = \"" + objectId + "\";" +
                              "var loopIdx = " + QString::number(loopIdx) + ";";
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

float UiFactory::getFloatValue(QString line, QString objectId, qint32 loopIdx, float defaultValue)
{
    float value = defaultValue;
    if (!line.isEmpty())
    {
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";" +
                              "var objectId = \"" + objectId + "\";" +
                              "var loopIdx = " + QString::number(loopIdx) + ";";
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

bool UiFactory::getBoolValue(QString line, QString objectId, qint32 loopIdx, bool defaultValue)
{
    bool value = defaultValue;
    if (!line.isEmpty())
    {
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
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

QString UiFactory::getStringValue(QString line, QString objectId, qint32 loopIdx)
{
    QString value;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
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

Player* UiFactory::getPlayerValue(QString line, QString objectId, qint32 loopIdx)
{
    Player* value = nullptr;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            value = erg.toVariant().value<Player*>();
        }
    }
    return value;
}

QStringList UiFactory::getStringListValue(QString line, QString objectId, qint32 loopIdx)
{
    QStringList value;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();

        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        else
        {
            value = erg.toVariant().toStringList();
        }
    }
    return value;
}

oxygine::TextStyle UiFactory::getStyle(QString styleName, QColor fontColor)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getInstance()->getResFont(styleName));
    style.color = fontColor;
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    return style;
}

QString UiFactory::getId(QString attribute)
{
    QString ret = "object" + QString::number(m_creationCount);
    if (!attribute.isEmpty())
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
    if (line.startsWith("QT_TRANSLATE_NOOP"))
    {
        line = line.replace("QT_TRANSLATE_NOOP(\"GAME\", \"", "");
        line = line.replace("QT_TRANSLATE_NOOP(\"GAME\",\"", "");
        line = line.trimmed();
        line.remove(line.length() - 2, 2);
    }
    else
    {
        line = line.trimmed();
    }
    return Mainapp::qsTr(line);
}

void UiFactory::doEvent(QString command, QString objectId, qint32 loopIdx)
{
    QString args;
    args += "var objectId = \"" + objectId + "\";";
    args += "var loopIdx = " + QString::number(loopIdx) + ";";
    args += command;
    QJSValue erg = Interpreter::getInstance()->evaluate(args);
    if (erg.isError())
    {
        CONSOLE_PRINT("Error while parsing " + command + " Error: " + erg.toString() + ".", Console::eERROR);
    }
}
