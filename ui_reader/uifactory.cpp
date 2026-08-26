#include "ui_reader/uifactory.h"

#include <QtGlobal>

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/gamemanager.h"

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "coreengine/mainapp.h"
#include "coreengine/virtualpaths.h"

#include "objects/base/label.h"
#include "objects/base/checkbox.h"
#include "objects/base/spinbox.h"
#include "objects/base/timespinbox.h"
#include "objects/base/textbox.h"
#include "objects/base/multilinetextbox.h"
#include "objects/base/multislider.h"
#include "objects/base/panel.h"
#include "objects/base/slider.h"
#include "objects/base/spriteobject.h"
#include "objects/base/moveinbutton.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/base/passwordbox.h"
#include "objects/base/selectkey.h"
#include "objects/base/topbar.h"
#include "objects/base/box9object.h"
#include "objects/base/coloredbar.h"
#include "objects/minimap.h"
#include "objects/boxterrainpalettepreview.h"

#include "game/gamemap.h"

#include "wiki/wikidatabase.h"

static const char* const itemColoredRect = "ColoredRect";
static const char* const itemLabel = "Label";
static const char* const itemCheckbox = "Checkbox";
static const char* const itemSpinbox = "Spinbox";
static const char* const itemTextbox = "Textbox";
static const char* const itemMultilineTextbox = "MultilineTextbox";
static const char* const itemPasswordbox = "Passwordbox";
static const char* const itemTimeSpinbox = "TimeSpinbox";
static const char* const itemPanel = "Panel";
static const char* const itemBox = "Box";
static const char* const itemIcon = "Icon";
static const char* const itemButton = "Button";
static const char* const itemIconButton = "IconButton";
static const char* const itemSlider = "Slider";
static const char* const itemMultiSlider = "MultiSlider";
static const char* const itemMoveInButton = "MoveInButton";
static const char* const itemSprite = "Sprite";
static const char* const itemTextField = "TextField";
static const char* const itemDropDownMenu = "DropDownMenu";
static const char* const itemDropDownMenuColor = "DropDownMenuColor";
static const char* const itemDropDownMenuSprite = "DropDownMenuSprite";
static const char* const itemSelectKey = "SelectKey";
static const char* const itemCustom = "Custom";
static const char* const itemLoop = "loop";
static const char* const itemIf = "if";
static const char* const itemTopbar = "Topbar";
static const char* const itemTabbedBox = "TabbedBox";
static const char* const itemTab = "Tab";
static const char* const itemMinimap = "Minimap";
static const char* const itemSlidingBox = "SlidingBox";
static const char* const itemColoredBar = "ColoredBar";
static const char* const itemBoxTerrainPalette = "BoxTerrainPalette";

static const char* const attrX = "x";
static const char* const attrY = "y";
static const char* const attrWidth = "width";
static const char* const attrHeight = "height";
static const char* const attrSize = "size";
static const char* const attrLabelWidth = "labelWidth";
static const char* const attrTooltip = "tooltip";
static const char* const attrOnEvent = "onEvent";
static const char* const attrStartValue = "startValue";
static const char* const attrStartValues = "startValues";
static const char* const attrLabels = "labels";
static const char* const attrText = "text";
static const char* const attrOnUpdate = "onUpdate";
static const char* const attrFont = "font";
static const char* const attrFontColor = "fontColor";
static const char* const attrColor = "color";
static const char* const attrInfinite = "infinite";
static const char* const attrMin = "min";
static const char* const attrMax = "max";
static const char* const attrChilds = "childs";
static const char* const attrContent = "content";
static const char* const attrSprite = "sprite";
static const char* const attrForeSprite = "foreSprite";
static const char* const attrUnit = "unit";
static const char* const attrId = "Id";
static const char* const attrEnabled = "enabled";
static const char* const attrDirection = "direction";
static const char* const attrScale = "scale";
static const char* const attrUseY = "useY";
static const char* const attrStartOffset = "startOffset";
static const char* const attrMoveInSize = "moveInSize";
static const char* const attrFontSize = "fontSize";
static const char* const attrItems = "items";
static const char* const attrSpriteType = "spriteType";
static const char* const attrSpriteSize = "spriteSize";
static const char* const attrPlayer = "player";
static const char* const attrHAlign = "hAlign";
static const char* const attrMode = "mode";
static const char* const attrVisible = "visible";
static const char* const attrSpinSpeed = "spinSpeed";
static const char* const attrShowUnitPreview = "showUnitPreview";
static const char* const attrCustomItem = "customItem";
static const char* const attrShowBorder = "showBorder";
static const char* const attrRows = "rows";
static const char* const attrGroups = "groups";
static const char* const attrItemID = "itemID";
static const char* const attrColumns = "columns";
static const char* const attrCellSize = "cellSize";
static const char* const attrLineWidth = "lineWidth";
static const char* const attrOnHovered = "onHovered";
static const char* const attrTabBarHeight = "tabBarHeight";
static const char* const attrTabBarButtonHeight = "tabBarButtonHeight";
static const char* const attrTabButtonWidth = "tabButtonWidth";
static const char* const attrTabName = "name";
static const char* const attrTabLabel = "label";
static const char* const attrInactiveResAnim = "inactiveResAnim";
static const char* const attrActiveResAnim = "activeResAnim";
static const char* const attrTabs = "tabs";
static const char* const attrTerrain = "terrain";
static const char* const attrColorPicker = "colorPicker";

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

spUiFactory UiFactory::m_pUiFactory;

UiFactory::UiFactory()
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("UiFactory");
    m_factoryItems.append({QString(itemLabel), std::bind(&UiFactory::createLabel, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemCheckbox), std::bind(&UiFactory::createCheckbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemSpinbox), std::bind(&UiFactory::createSpinbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemTextbox), std::bind(&UiFactory::createTextbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemPasswordbox), std::bind(&UiFactory::createPasswordbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemTimeSpinbox), std::bind(&UiFactory::createTimeSpinbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemPanel), std::bind(&UiFactory::createPanel, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemIcon), std::bind(&UiFactory::createIcon, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemBox), std::bind(&UiFactory::createBox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemButton), std::bind(&UiFactory::createButton, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemSelectKey), std::bind(&UiFactory::createSelectKey, this, _1, _2, _3, _4, _5)});
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
    m_factoryItems.append({QString(itemMultilineTextbox), std::bind(&UiFactory::createMultilineTextbox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemMultiSlider), std::bind(&UiFactory::createMultiSlider, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemCustom), std::bind(&UiFactory::createCustom, this, _1, _2, _3, _4, _5)});
    m_factoryItems.append({QString(itemColoredRect), std::bind(&UiFactory::createColoredRect, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemTopbar), std::bind(&UiFactory::createTopbar, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemTabbedBox), std::bind(&UiFactory::createTabbedBox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemMinimap), std::bind(&UiFactory::createMinimap, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemSlidingBox), std::bind(&UiFactory::createSlidingBox, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemColoredBar), std::bind(&UiFactory::createColoredBar, this, _1, _2, _3, _4, _5)});
    m_factoryItems.push_back({QString(itemBoxTerrainPalette), std::bind(&UiFactory::createBoxTerrainPalette, this, _1, _2, _3, _4, _5)});

    connect(this, &UiFactory::sigDoEvent, this, &UiFactory::doEvent, Qt::QueuedConnection);
}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

void UiFactory::shutdown()
{
    m_pUiFactory = nullptr;
}

void UiFactory::createUi(QString uiXml, CreatedGui* pMenu, oxygine::spActor root)
{
    if (!Mainapp::getInstance()->getNoUi())
    {
        Mainapp::getInstance()->pauseRendering();
        CONSOLE_PRINT_MODULE("Loading ui " + uiXml, GameConsole::eDEBUG, GameConsole::eUiFactory);
        if (m_dropDownPlayer.get() == nullptr)
        {
            m_dropDownPlayer = MemoryManagement::create<Player>(nullptr);
            m_dropDownPlayer->init();
        }
        m_creationCount = 0;
        QStringList uiFiles = VirtualPaths::createSearchPath("resources/" + uiXml);
        m_lastCoordinates.setRect(0, 0, 0, 0);
        for (const auto & uiFile : std::as_const(uiFiles))
        {
            if (QFile::exists(uiFile))
            {
                QDomDocument document;
                QFile file(uiFile);
                if (file.open(QIODevice::ReadOnly))
                {
                    bool success = false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
                    auto result = document.setContent(&file);
                    bool loaded = static_cast<bool>(result);
                    QString errorMessage = result.errorMessage;
                    qsizetype errorLine = result.errorLine;
                    qsizetype errorColumn = result.errorColumn;
#else
                    QString errorMessage;
                    int errorLine = 0;
                    int errorColumn = 0;
                    bool loaded = document.setContent(&file, &errorMessage, &errorLine, &errorColumn);
#endif
                    if (loaded)
                    {
                        bool predefinedRoot = (root != nullptr);
                        success = true;
                        if (!predefinedRoot)
                        {
                            root = MemoryManagement::create<oxygine::Actor>();
                        }
                        auto rootElement = document.documentElement();

                        bool overwrite = false;
                        QVariant value = QVariant(rootElement.attribute("overwrite"));
                        if (value.typeId() == QMetaType::QString &&
                            !value.isNull() &&
                            rootElement.hasAttribute("overwrite"))
                        {
                            overwrite = value.toBool();
                        }

                        auto node = rootElement.firstChild();
                        while (!node.isNull())
                        {
                            while (node.isComment())
                            {
                                node = node.nextSibling();
                            }
                            if (!node.isNull())
                            {
                                spPanel pPanel = std::dynamic_pointer_cast<Panel>(root);
                                if (pPanel.get())
                                {
                                    success = loadPanelContent(node, pPanel, pMenu, 0);
                                }
                                else
                                {
                                    oxygine::spActor item;
                                    success = createItem(root, node.toElement(), item, pMenu);
                                }
                                if (!success)
                                {
                                    CONSOLE_PRINT("Unknown item: " + node.toElement().nodeName() + " found. UI creation failed.", GameConsole::eERROR);
                                }
                                else
                                {
                                }
                            }
                            node = node.nextSibling();
                        }
                        if (success)
                        {
                            if (!predefinedRoot)
                            {
                                pMenu->addFactoryUiItem(root);
                                pMenu->addChild(root);
                            }
                        }
                        else
                        {
                            CONSOLE_PRINT("Unable to load: " + uiFile, GameConsole::eERROR);
                        }
                        if (overwrite)
                        {
                            break;
                        }
                    }
                    else
                    {
                        CONSOLE_PRINT("Unable to load: " + uiFile, GameConsole::eERROR);
                        CONSOLE_PRINT("Error: " + errorMessage + " at line " + QString::number(errorLine) + " at column " + QString::number(errorColumn), GameConsole::eERROR);
                    }
                    if (success)
                    {
                        break;
                    }
                }
                else
                {
                    CONSOLE_PRINT("Unable to open existing file: " + uiFile, GameConsole::eERROR);
                }
            }
        }
        Mainapp::getInstance()->continueRendering();
    }
}



bool UiFactory::loop(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    qint32 loopCount = getIntValue(element.attribute("count"), "", loopIdx, pMenu);
    bool success = loopCount >= 0;
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
    bool create = getBoolValue(element.attribute("condition"), "", loopIdx, pMenu);
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
        CONSOLE_PRINT("Unable to create item: " + name + ".", GameConsole::eERROR);
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

bool UiFactory::createColoredRect(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrColor});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString color = getStringValue(getAttribute(childs, attrColor), id, loopIdx, pMenu);
        oxygine::spColorRectSprite pRect = MemoryManagement::create<oxygine::ColorRectSprite>();
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        pRect->setVisible(visible);
        pRect->setX(x);
        pRect->setY(y);
        if (height > 0)
        {
            pRect->setHeight(height);
        }
        if (width > 0)
        {
            pRect->setWidth(width);
        }
        pRect->setColor(color);
        parent->addChild(pRect);
        item = pRect;

        m_lastCoordinates = QRect(x, y, pRect->getScaledWidth(), pRect->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrText, attrFont});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString text = translate(getStringValue(getAttribute(childs, attrText), id, loopIdx, pMenu));
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString fontColor = getStringValue(getAttribute(childs, attrFontColor), id, loopIdx, pMenu);
        auto hAlign = getHAlignment(getAttribute(childs, attrHAlign), id, loopIdx, pMenu);
        auto style = getStyle(getStringValue(getAttribute(childs, attrFont), id, loopIdx, pMenu),
                              fontColor,
                              getIntValue(getAttribute(childs, attrFontSize), id, loopIdx, pMenu, 24),
                              hAlign);
        bool showBorder = getBoolValue(getAttribute(childs, attrShowBorder), id, loopIdx, pMenu, false);                              
        spLabel pLabel = MemoryManagement::create<Label>(width, showBorder);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        pLabel->setVisible(visible);
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setHtmlText(text);
        pLabel->setTooltipText(tooltip);
        if (!id.isEmpty())
        {
            pLabel->setObjectName(id);
        }
        pLabel->setEnabled(enabled);
        if (height > 0)
        {
            pLabel->setHeight(height);
        }
        QString onUpdateLine = getAttribute(childs, attrOnUpdate);
        if (!onUpdateLine.isEmpty())
        {
            Label* pPtr = pLabel.get();
            connect(pMenu, &CreatedGui::sigOnUpdate, pLabel.get(), [this, pPtr, onUpdateLine, id, loopIdx, pMenu]()
            {
                pPtr->setHtmlText(onUpdate<QString>(onUpdateLine, id, loopIdx, pMenu));
            }, Qt::QueuedConnection);
        }        
        QString onEvent = getAttribute(childs, attrOnEvent);
        if (!onEvent.isEmpty())
        {
            pLabel->addClickListener([this, onEvent, id, loopIdx, pMenu](oxygine::Event*)
            {
                emit sigDoEvent(onEvent, id, loopIdx, pMenu);
            });
        }
        parent->addChild(pLabel);
        item = pLabel;

        m_lastCoordinates = QRect(x, y, pLabel->getScaledWidth(), pLabel->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createTextfield(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrText, attrFont});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu, -1);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu, -1);
        QString text = translate(getStringValue(getAttribute(childs, attrText), id, loopIdx, pMenu));
        QString fontColor = getStringValue(getAttribute(childs, attrFontColor), id, loopIdx, pMenu);
        auto hAlign = getHAlignment(getAttribute(childs, attrHAlign), id, loopIdx, pMenu);
        auto style = getStyle(getStringValue(getAttribute(childs, attrFont), id, loopIdx, pMenu),
                              fontColor,
                              getIntValue(getAttribute(childs, attrFontSize), id, loopIdx, pMenu, 24),
                              hAlign);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        oxygine::spTextField pLabel = MemoryManagement::create<oxygine::TextField>();
        pLabel->setX(x);
        pLabel->setY(y);
        pLabel->setStyle(style);
        pLabel->setHtmlText(text);
        const auto & textRect = pLabel->getTextRect();
        if (width > 0)
        {
            pLabel->setWidth(width);
        }
        else
        {
            pLabel->setWidth(textRect.width());
        }
        if (height > 0)
        {
            pLabel->setHeight(height);
        }
        else
        {
            pLabel->setHeight(textRect.height());
        }
        pLabel->setVisible(visible);
        pLabel->setEnabled(enabled);
        parent->addChild(pLabel);
        item = pLabel;
        m_lastCoordinates = QRect(x, y, pLabel->getScaledWidth(), pLabel->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrText, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        QString test = getAttribute(childs, attrWidth);
        qint32 width = -1;
        if (!test.isEmpty())
        {
            width = getIntValue(test, id, loopIdx, pMenu);
        }
        QString text = translate(getStringValue(getAttribute(childs, attrText), id, loopIdx, pMenu));
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        test = getAttribute(childs, attrSprite);
        QString sprite = "button";
        if (!test.isEmpty())
        {
            sprite = getStringValue(test, id, loopIdx, pMenu);
        }
        oxygine::spButton pButton = ObjectManager::createButton(text, width, tooltip, sprite);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        pButton->setVisible(visible);
        if (!id.isEmpty())
        {
            pButton->setObjectName(id);
        }
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([this, onEvent, id, loopIdx, pMenu](oxygine::Event*)
        {
            emit sigDoEvent(onEvent, id, loopIdx, pMenu);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, pButton->getScaledWidth(), pButton->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createSelectKey(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrStartValue, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 startValue = getIntValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QString onEventLine = getAttribute(childs, attrOnEvent);
        spSelectKey pButton = MemoryManagement::create<SelectKey>(static_cast<Qt::Key>(startValue));
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        pButton->setVisible(visible);
        if (!id.isEmpty())
        {
            pButton->setObjectName(id);
        }
        pButton->setTooltipText(tooltip);
        connect(pButton.get(), &SelectKey::sigKeyChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](Qt::Key key)
        {
            qint32 item = static_cast<qint32>(key);
            onEvent(onEventLine, item, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pButton);
        item = pButton;
        m_lastCoordinates = QRect(x, y, pButton->getScaledWidth(), pButton->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        QString sprite = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        oxygine::spButton pButton = ObjectManager::createIconButton(sprite, 36);
        pButton->setX(x);
        pButton->setY(y);
        pButton->setEnabled(enabled);
        pButton->setVisible(visible);
        if (!id.isEmpty())
        {
            pButton->setObjectName(id);
        }
        QString onEvent = getAttribute(childs, attrOnEvent);
        pButton->addClickListener([this, onEvent, id, loopIdx, pMenu](oxygine::Event*)
        {
            emit sigDoEvent(onEvent, id, loopIdx, pMenu);
        });
        parent->addChild(pButton);
        item = pButton;

        m_lastCoordinates = QRect(x, y, pButton->getScaledWidth(), pButton->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createMoveInButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrMoveInSize});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 moveInSize = getIntValue(getAttribute(childs, attrMoveInSize), id, loopIdx, pMenu);
        qint32 direction = getIntValue(getAttribute(childs, attrDirection), id, loopIdx, pMenu, -1);
        qint32 startOffset = getIntValue(getAttribute(childs, attrStartOffset), id, loopIdx, pMenu, -1);
        float buttonScale = getIntValue(getAttribute(childs, attrScale), id, loopIdx, pMenu, 2.0f);
        bool useY = getBoolValue(getAttribute(childs, attrUseY), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        auto pMoveButton = MemoryManagement::create<MoveInButton>(parent.get(), moveInSize, direction, startOffset, buttonScale, useY);
        if (!id.isEmpty())
        {
            pMoveButton->setObjectName(id);
        }
        pMoveButton->setEnabled(enabled);
        pMoveButton->setVisible(visible);
        parent->addChild(pMoveButton);
        item = pMoveButton;
    }
    return success;
}

bool UiFactory::createSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSprite, attrScale});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);        
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString spriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        float scale = getFloatValue(getAttribute(childs, attrScale), id, loopIdx, pMenu);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        QString onEvent = getAttribute(childs, attrOnEvent);
        spSpriteObject pSprite = MemoryManagement::create<SpriteObject>();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        if (pAnim == nullptr)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim(spriteId);
        }
        if (!id.isEmpty())
        {
            pSprite->setObjectName(id);
        }
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        pSprite->setPosition(x, y);
        pSprite->setVisible(visible);
        pSprite->setEnabled(enabled);
        if (width > 0 && height > 0)
        {
            pSprite->setSize(width, height);
        }
        else if (pAnim != nullptr)
        {
            pSprite->setSize(pAnim->getSize());
        }
        else
        {
            pSprite->setSize(40, 40);
        }
        if (!onEvent.isEmpty())
        {
            pSprite->addClickListener([this, onEvent, id, loopIdx, pMenu](oxygine::Event*)
            {
                emit sigDoEvent(onEvent, id, loopIdx, pMenu);
            });
        }
        parent->addChild(pSprite);
        item = pSprite;
        m_lastCoordinates = QRect(x, y, pSprite->getScaledWidth(), pSprite->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createMinimap(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QString onEventLine = getAttribute(childs, attrOnEvent);
        spMinimap pMinimap = MemoryManagement::create<Minimap>();
        pMinimap->setPosition(x, y);
        pMinimap->setVisible(visible);
        pMinimap->setEnabled(enabled);
        if (!id.isEmpty())
        {
            pMinimap->setObjectName(id);
        }
        if (!onEventLine.isEmpty())
        {
            connect(pMinimap.get(), &Minimap::clicked, pMenu, [this, onEventLine, id, loopIdx, pMenu](qint32 x, qint32 y, bool updateMinimapPosition)
            {
                onEvent(onEventLine, x, y, id, loopIdx, pMenu);
            }, Qt::QueuedConnection);
        }
        parent->addChild(pMinimap);
        item = pMinimap;
        m_lastCoordinates = QRect(x, y, 10, 10);
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createSlidingBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrContent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        auto pSlidingbox = MemoryManagement::create<oxygine::SlidingActor>();
        pSlidingbox->setPosition(x, y);
        pSlidingbox->setSize(width, height);
        if (!id.isEmpty())
        {
            pSlidingbox->setObjectName(id);
        }
        auto node = getNode(childs, attrContent).firstChild();
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                oxygine::spActor panelItem;
                success = success && createItem(pSlidingbox, node.toElement(), panelItem, pMenu, loopIdx);
                if (panelItem.get() != nullptr)
                {
                    pSlidingbox->setContent(panelItem);
                }
            }
            node = node.nextSibling();
        }
        parent->addChild(pSlidingbox);
        item = pSlidingbox;
        m_lastCoordinates = QRect(x, y, width, height);
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createColoredBar(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrSprite, attrForeSprite});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString backSpriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        QString foreSpriteId = getStringValue(getAttribute(childs, attrForeSprite), id, loopIdx, pMenu);
        QString fontColor = getStringValue(getAttribute(childs, attrFontColor), id, loopIdx, pMenu);
        auto hAlign = getHAlignment(getAttribute(childs, attrHAlign), id, loopIdx, pMenu);
        auto style = getStyle(getStringValue(getAttribute(childs, attrFont), id, loopIdx, pMenu),
                              fontColor,
                              getIntValue(getAttribute(childs, attrFontSize), id, loopIdx, pMenu, 24),
                              hAlign);
        spColoredbar pBar = MemoryManagement::create<ColoredBar>(width, height, backSpriteId, foreSpriteId);
        pBar->setPosition(x, y);
        pBar->getLabel()->setStyle(style);
        if (!id.isEmpty())
        {
            pBar->setObjectName(id);
        }
        parent->addChild(pBar);
        item = pBar;
        m_lastCoordinates = QRect(x, y, pBar->getScaledWidth(), pBar->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createTopbar(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrWidth, attrGroups, attrOnEvent});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QString onEventLine = getAttribute(childs, attrOnEvent);

        spTopbar pTopbar = MemoryManagement::create<Topbar>(x, width);
        if (!id.isEmpty())
        {
            pTopbar->setObjectName(id);
        }
        pTopbar->setVisible(visible);

        // Parse groups
        auto groupsNode = getNode(childs, attrGroups);
        auto groupNode = groupsNode.firstChild();
        qint32 groupId = 0;
        while (!groupNode.isNull())
        {
            while (groupNode.isComment())
            {
                groupNode = groupNode.nextSibling();
            }
            if (!groupNode.isNull())
            {
                QDomElement groupElement = groupNode.toElement();
                QString groupText = getStringValue(getAttribute(groupElement.childNodes(), attrText), "", loopIdx, pMenu);
                if (!groupText.isEmpty())
                {
                    pTopbar->addGroup(groupText);

                    // Parse items within this group
                    auto itemsNode = getNode(groupElement.childNodes(), attrChilds);
                    auto itemNode = itemsNode.firstChild();
                    while (!itemNode.isNull())
                    {
                        while (itemNode.isComment())
                        {
                            itemNode = itemNode.nextSibling();
                        }
                        QDomElement itemElement = itemNode.toElement();
                        if (!itemElement.isNull() && itemElement.toElement().nodeName() == itemIf)
                        {
                            auto ifNode = itemElement.toElement();
                            bool create = getBoolValue(ifNode.attribute("condition"), "", loopIdx, pMenu);
                            if (create)
                            {
                                auto node = itemNode.firstChild();
                                while (!node.isNull())
                                {
                                    addItemNode(node, pTopbar, groupId, pMenu, loopIdx);
                                }
                            }
                            itemNode = itemNode.nextSibling();
                        }
                        else
                        {
                            itemNode = addItemNode(itemNode, pTopbar, groupId, pMenu, loopIdx);
                        }
                    }
                    ++groupId;
                }
            }
            groupNode = groupNode.nextSibling();
        }

        pTopbar->finishCreation();

        connect(pTopbar.get(), &Topbar::sigItemClicked, pMenu, [this, onEventLine, id, loopIdx, pMenu](const QString& itemId)
                {
                    onEvent(onEventLine, itemId, id, loopIdx, pMenu);
                }, Qt::QueuedConnection);

        parent->addChild(pTopbar);
        item = pTopbar;

        m_lastCoordinates = QRect(x, 0, pTopbar->getScaledWidth(), pTopbar->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

QDomNode UiFactory::addItemNode(QDomNode & itemNode, spTopbar & pTopbar, qint32 groupId, CreatedGui* pMenu, qint32 loopIdx)
{
    while (itemNode.isComment())
    {
        itemNode = itemNode.nextSibling();
    }
    if (!itemNode.isNull())
    {
        QDomElement itemElement = itemNode.toElement();
        QString itemText = getStringValue(getAttribute(itemElement.childNodes(), attrText), "", loopIdx, pMenu);
        QString itemID = getStringValue(getAttribute(itemElement.childNodes(), attrItemID), "", loopIdx, pMenu);
        QString itemTooltip = getStringValue(getAttribute(itemElement.childNodes(), attrTooltip), "", loopIdx, pMenu);
        if (!itemText.isEmpty() && !itemID.isEmpty())
        {
            pTopbar->addItem(itemText, itemID, groupId, itemTooltip);
        }
        itemNode = itemNode.nextSibling();
    }
    return itemNode;
}


bool UiFactory::createTabbedBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        qint32 tabBarHeight = getIntValue(getAttribute(childs, attrTabBarHeight), id, loopIdx, pMenu, 40);
        qint32 tabBarButtonHeight = getIntValue(getAttribute(childs, attrTabBarButtonHeight), id, loopIdx, pMenu, 40);
        qint32 tabButtonWidth = getIntValue(getAttribute(childs, attrTabButtonWidth), id, loopIdx, pMenu, 100);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);

        spTabbedBox pTabbedBox = MemoryManagement::create<TabbedBox>(QSize(width, height), tabBarHeight, tabBarButtonHeight, tabButtonWidth);
        pTabbedBox->setX(x);
        pTabbedBox->setY(y);
        pTabbedBox->setVisible(visible);
        pTabbedBox->setEnabled(enabled);
        if (!id.isEmpty())
        {
            pTabbedBox->setObjectName(id);
        }

        // Store the parent size for processing tab contents
        QSize savedParentSize = m_parentSize;
        m_parentSize = QSize(width, height - tabBarHeight);

        // Parse Tab elements
        auto tabsNode = getNode(childs, attrTabs);
        auto tabNode = tabsNode.firstChild();
        while (!tabNode.isNull())
        {
            while (tabNode.isComment())
            {
                tabNode = tabNode.nextSibling();
            }
            if (!tabNode.isNull() && tabNode.toElement().nodeName() == itemIf)
            {
                auto ifNode = tabNode.toElement();
                bool create = getBoolValue(ifNode.attribute("condition"), "", loopIdx, pMenu);
                if (create)
                {
                    auto node = tabNode.firstChild();
                    while (!node.isNull())
                    {
                        addTabNode(node, pTabbedBox, pMenu, loopIdx, success);
                    }
                }
                tabNode = tabNode.nextSibling();
            }
            else
            {
                tabNode = addTabNode(tabNode, pTabbedBox, pMenu, loopIdx, success);
            }
        }

        m_parentSize = savedParentSize;
        parent->addChild(pTabbedBox);
        m_lastCoordinates = QRect(x, y, pTabbedBox->getScaledWidth(), pTabbedBox->getScaledHeight());
        updateMenuSize(pMenu);
        item = pTabbedBox;
    }
    return success;
}

QDomNode UiFactory::addTabNode(QDomNode & tabNode, spTabbedBox & pTabbedBox, CreatedGui* pMenu, qint32 loopIdx, bool & success)
{
    while (tabNode.isComment())
    {
        tabNode = tabNode.nextSibling();
    }
    if (!tabNode.isNull() && tabNode.toElement().nodeName() == itemTab)
    {
        QDomElement tabElement = tabNode.toElement();
        auto tabChilds = tabElement.childNodes();

        QString tabName = getStringValue(getAttribute(tabChilds, attrTabName), "", loopIdx, pMenu);
        QString tabLabel = getStringValue(getAttribute(tabChilds, attrTabLabel), "", loopIdx, pMenu);
        QString inactiveResAnim = getStringValue(getAttribute(tabChilds, attrInactiveResAnim), "", loopIdx, pMenu);
        QString activeResAnim = getStringValue(getAttribute(tabChilds, attrActiveResAnim), "", loopIdx, pMenu);

        // Use default values if not provided
        if (inactiveResAnim.isEmpty())
        {
            inactiveResAnim = "tab_inactive";
        }
        if (activeResAnim.isEmpty())
        {
            activeResAnim = "tab_active";
        }

        if (!tabName.isEmpty() && !tabLabel.isEmpty())
        {
            spPanel tabPanel = pTabbedBox->addTab(tabName, tabLabel, inactiveResAnim, activeResAnim);

            // Add child elements to the tab panel
            auto childNode = tabElement.firstChild();
            qint32 maxWidth = 0;
            qint32 maxHeight = 0;
            while (!childNode.isNull())
            {
                while (childNode.isComment())
                {
                    childNode = childNode.nextSibling();
                }
                // Skip the attribute nodes (name, label, etc.)
                if (!childNode.isNull() && childNode.nodeName() != attrTabName &&
                    childNode.nodeName() != attrTabLabel &&
                    childNode.nodeName() != attrInactiveResAnim &&
                    childNode.nodeName() != attrActiveResAnim)
                {
                    QDomElement childElement = childNode.toElement();
                    if (!childElement.isNull() && childElement.nodeName() != "")
                    {
                        oxygine::spActor tabItem;
                        success = success && createItem(tabPanel, childElement, tabItem, pMenu, loopIdx);
                        if (tabItem.get() != nullptr)
                        {
                            tabPanel->addItem(tabItem);
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
                }
                childNode = childNode.nextSibling();
            }
            if (maxHeight >= tabPanel->getScaledHeight() - 80)
            {
                tabPanel->setContentHeigth(maxHeight + 40);
            }
            else
            {
                tabPanel->setContentHeigth(tabPanel->getScaledHeight() - 80);
            }
            if (maxWidth >= tabPanel->getScaledWidth() - 80)
            {
                tabPanel->setContentWidth(maxWidth + 80);
            }
            else
            {
                tabPanel->setContentWidth(tabPanel->getScaledWidth() - 80);
            }
        }        
    }

    tabNode = tabNode.nextSibling();
    return tabNode;
}

bool UiFactory::createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        bool value = getBoolValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spCheckbox pCheckbox = MemoryManagement::create<Checkbox>();
        pCheckbox->setX(x);
        pCheckbox->setY(y);
        pCheckbox->setChecked(value);
        pCheckbox->setTooltipText(tooltip);
        if (!id.isEmpty())
        {
            pCheckbox->setObjectName(id);
        }
        pCheckbox->setEnabled(enabled);
        pCheckbox->setVisible(visible);
        parent->addChild(pCheckbox);
        connect(pCheckbox.get(), &Checkbox::checkChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](bool value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        item = pCheckbox;
        m_lastCoordinates = QRect(x, y, pCheckbox->getScaledWidth(), pCheckbox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrInfinite, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx, pMenu);
        float min = getFloatValue(getAttribute(childs,attrMin), id, loopIdx, pMenu);
        float max = getFloatValue(getAttribute(childs,attrMax), id, loopIdx, pMenu);
        float infinite = getFloatValue(getAttribute(childs, attrInfinite), id, loopIdx, pMenu);
        float spinSpeed = getFloatValue(getAttribute(childs, attrSpinSpeed), id, loopIdx, pMenu, 1.0f);
        QString tooltip = translate(getStringValue(getAttribute(childs,attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        QString unit = getStringValue(getAttribute(childs, attrUnit), id, loopIdx, pMenu);
        float value = getFloatValue(getAttribute(childs,attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QString test = getAttribute(childs, attrMode);
        SpinBox::Mode mode = SpinBox::Mode::Int;
        if (!test.isEmpty())
        {
            mode = getEnumValue(test, id, loopIdx, pMenu, {"Int", "Float", "Real,"}, SpinBox::Mode::Int);
        }

        spSpinBox pSpinBox = MemoryManagement::create<SpinBox>(width, min, max, mode);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setInfinityValue(infinite);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setUnit(unit);
        pSpinBox->setCurrentValue(value);
        if (!id.isEmpty())
        {
            pSpinBox->setObjectName(id);
        }
        pSpinBox->setEnabled(enabled);
        pSpinBox->setSpinSpeed(spinSpeed);
        pSpinBox->setVisible(visible);
        connect(pSpinBox.get(), &SpinBox::sigValueChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](qreal value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, pSpinBox->getScaledWidth(), pSpinBox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrMin,
                                          attrMax, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx, pMenu);
        qint32 min = getIntValue(getAttribute(childs,attrMin), id, loopIdx, pMenu);
        qint32 max = getIntValue(getAttribute(childs,attrMax), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs,attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        qint32 value = getIntValue(getAttribute(childs,attrStartValue), id, loopIdx, pMenu);
        QString unit = "%";
        QString test = getAttribute(childs, attrUnit);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        if (!test.isEmpty())
        {
            unit = getStringValue(test, id, loopIdx, pMenu);
        }
        spSlider pSlider = MemoryManagement::create<Slider>(width - 75, min, max, unit);
        pSlider->setX(x);
        pSlider->setY(y);
        pSlider->setTooltipText(tooltip);
        pSlider->setCurrentValue(value);
        if (!id.isEmpty())
        {
            pSlider->setObjectName(id);
        }
        pSlider->setEnabled(enabled);
        pSlider->setVisible(visible);
        connect(pSlider.get(), &Slider::sliderValueChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](qint32 value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pSlider);
        item = pSlider;
        m_lastCoordinates = QRect(x, y, pSlider->getScaledWidth(), pSlider->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createMultiSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrLabels, attrStartValues});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx, pMenu);
        qint32 labelWidth = getIntValue(getAttribute(childs, attrLabelWidth), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs,attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QStringList labels = getStringListValue(getAttribute(childs, attrLabels), id, loopIdx, pMenu);
        QVector<qint32> chances = getInt32ListValue(getAttribute(childs, attrStartValues), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spMultislider pMultislider = MemoryManagement::create<Multislider>(labels, width, chances, labelWidth);
        pMultislider->setX(x);
        pMultislider->setY(y);
        if (!id.isEmpty())
        {
            pMultislider->setObjectName(id);
        }
        pMultislider->setEnabled(enabled);
        pMultislider->setVisible(visible);
        pMultislider->setTooltipText(tooltip);
        connect(pMultislider.get(), &Multislider::signalSliderChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu]()
        {
            onEvent(onEventLine, 0, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pMultislider);
        item = pMultislider;
        m_lastCoordinates = QRect(x, y, pMultislider->getScaledWidth(), pMultislider->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createCustom(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrCustomItem});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        QString customItem = translate(getStringValue(getAttribute(childs, attrCustomItem), id, loopIdx, pMenu));
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        float scale = getFloatValue(getAttribute(childs, attrScale), id, loopIdx, pMenu);
        QString onEventLine = getAttribute(childs, attrOnEvent);
        qint32 scaledWidth = 0;
        qint32 scaledHeight = 0;
        item = pMenu->loadCustomId(customItem, x, y, enabled, visible, scale,id, tooltip, onEventLine,
                                   this, pMenu, loopIdx, scaledWidth, scaledHeight);
        parent->addChild(item);
        m_lastCoordinates = QRect(x, y, scaledWidth, scaledHeight);
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = -1;
        if (hasChild(childs, attrHeight))
        {
            height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        }
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spTextbox pTextbox = MemoryManagement::create<Textbox>(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
        if (!id.isEmpty())
        {
            pTextbox->setObjectName(id);
        }
        pTextbox->setEnabled(enabled);
        pTextbox->setVisible(visible);
        connect(pTextbox.get(), &Textbox::sigTextChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](QString value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pTextbox);
        item = pTextbox;
        m_lastCoordinates = QRect(x, y, pTextbox->getScaledWidth(), pTextbox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createMultilineTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spMultilineTextbox pTextbox = MemoryManagement::create<MultilineTextbox>(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
        if (!id.isEmpty())
        {
            pTextbox->setObjectName(id);
        }
        pTextbox->setEnabled(enabled);
        pTextbox->setVisible(visible);
        connect(pTextbox.get(), &MultilineTextbox::sigTextChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](QString value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pTextbox);
        item = pTextbox;
        m_lastCoordinates = QRect(x, y, pTextbox->getScaledWidth(), pTextbox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createPasswordbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = -1;
        if (hasChild(childs, attrHeight))
        {
            height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        }
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spPasswordbox pTextbox = MemoryManagement::create<Passwordbox>(width, height);
        pTextbox->setPosition(x, y);
        pTextbox->setTooltipText(tooltip);
        pTextbox->setCurrentText(value);
        if (!id.isEmpty())
        {
            pTextbox->setObjectName(id);
        }
        pTextbox->setEnabled(enabled);
        pTextbox->setVisible(visible);
        connect(pTextbox.get(), &Textbox::sigTextChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](QString value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pTextbox);
        item = pTextbox;
        m_lastCoordinates = QRect(x, y, pTextbox->getScaledWidth(), pTextbox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrOnEvent, attrStartValue});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs,attrWidth), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs,attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs,attrOnEvent);
        quint64 value = getUInt64Value(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spTimeSpinBox pSpinBox = MemoryManagement::create<TimeSpinBox>(width);
        pSpinBox->setX(x);
        pSpinBox->setY(y);
        pSpinBox->setTooltipText(tooltip);
        pSpinBox->setCurrentValue(value);
        if (!id.isEmpty())
        {
            pSpinBox->setObjectName(id);
        }
        pSpinBox->setEnabled(enabled);
        pSpinBox->setVisible(visible);
        connect(pSpinBox.get(), &TimeSpinBox::sigValueChanged, pMenu, [this, onEventLine, id, loopIdx, pMenu](qint32 value)
        {
            onEvent(onEventLine, value, id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pSpinBox);
        item = pSpinBox;
        m_lastCoordinates = QRect(x, y, pSpinBox->getScaledWidth(), pSpinBox->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrSize, attrSprite});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs,attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs,attrY), id, loopIdx, pMenu);
        qint32 size = getIntValue(getAttribute(childs,attrSize), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QString icon = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        Player* pPlayer = getPlayerValue(getAttribute(childs, attrPlayer), id, loopIdx, pMenu);
        oxygine::spSprite pIcon = pWikiDatabase->getIcon(nullptr, icon, size, pPlayer);
        pIcon->setPosition(x, y);
        parent->addChild(pIcon);
        pIcon->setEnabled(enabled);
        pIcon->setVisible(visible);
        item = pIcon;
        m_lastCoordinates = QRect(x, y, pIcon->getScaledWidth(), pIcon->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrChilds});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        QSize size = QSize(width, height);
        spPanel pPanel = MemoryManagement::create<Panel>(true, size, size);
        pPanel->setX(x);
        pPanel->setY(y);
        pPanel->setVisible(visible);
        if (!id.isEmpty())
        {
            pPanel->setObjectName(id);
        }
        pPanel->setEnabled(enabled);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        updateMenuSize(pMenu);
        m_parentSize = QSize(pPanel->getScaledWidth(), pPanel->getScaledHeight());
        auto node = getNode(childs, attrChilds).firstChild();
        loadPanelContent(node, pPanel, pMenu, loopIdx);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        parent->addChild(pPanel);
        m_parentSize = QSize(0, 0);
        item = pPanel;
    }
    return success;
}

bool UiFactory::loadPanelContent(QDomNode node, spPanel & pPanel, CreatedGui* pMenu, qint32 loopIdx)
{
    bool success = true;
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
    // restore last coordinates after iterating over child elements
    if (maxHeight >= pPanel->getScaledHeight() - 80)
    {
        pPanel->setContentHeigth(maxHeight + 80);
    }
    else
    {
        pPanel->setContentHeigth(pPanel->getScaledHeight() - 80);
    }
    if (maxWidth >= pPanel->getScaledWidth() - 80)
    {
        pPanel->setContentWidth(maxWidth + 80);
    }
    else
    {
        pPanel->setContentWidth(pPanel->getScaledWidth() - 80);
    }
    return success;
}

bool UiFactory::createDropDownMenu(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);

        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        spDropDownmenu pDropDownmenu = MemoryManagement::create<DropDownmenu>(width, items);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        pDropDownmenu->setVisible(visible);
        pDropDownmenu->setEnabled(enabled);
        if (!id.isEmpty())
        {
            pDropDownmenu->setObjectName(id);
        }
        if (hasChild(childs, attrStartValue))
        {
            bool success = false;
            qint32 intValue = getIntValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu, 0, &success);
            if (success)
            {
                pDropDownmenu->setCurrentItem(intValue);
            }
            else
            {
                QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
                pDropDownmenu->setCurrentItem(value);

            }
        }
        DropDownmenu* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, pMenu, [this, onEventLine, pDropDownmenuPtr, id, loopIdx, pMenu](qint32 value)
        {
            onEvent(onEventLine, value, pDropDownmenuPtr->getCurrentItemText(), id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, pDropDownmenu->getScaledWidth(), pDropDownmenu->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createDropDownMenuColor(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QString value;
        if (hasChild(childs, attrStartValue))
        {
            value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
        }
        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx, pMenu);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool showUnitPreview = getBoolValue(getAttribute(childs, attrShowUnitPreview), id, loopIdx, pMenu, false);
        bool colorPicker = getBoolValue(getAttribute(childs, attrColorPicker), id, loopIdx, pMenu, true);
        QVector<QColor> colorItems;
        colorItems.reserve(items.size());
        for (auto & item : items)
        {
            colorItems.append(QColor(item));
        }
        spDropDownmenuColor pDropDownmenu = MemoryManagement::create<DropDownmenuColor>(width, colorItems, colorPicker);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        pDropDownmenu->setVisible(visible);
        pDropDownmenu->setShowUnitPreview(showUnitPreview);
        if (!id.isEmpty())
        {
            pDropDownmenu->setObjectName(id);
        }
        pDropDownmenu->setEnabled(enabled);
        if (hasChild(childs, attrStartValue))
        {
            bool success = false;
            qint32 intValue = getIntValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu, 0, &success);
            if (success)
            {
                pDropDownmenu->setCurrentItem(intValue);
            }
            else
            {
                QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
                pDropDownmenu->setCurrentItem(value);

            }
        }
        DropDownmenuColor* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenuColor::sigItemChanged, pMenu, [this, onEventLine, pDropDownmenuPtr, id, loopIdx, pMenu](QColor value)
        {
            onEvent(onEventLine, value.name(), pDropDownmenuPtr->getCurrentItem(), id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, pDropDownmenu->getScaledWidth(), pDropDownmenu->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createDropDownMenuSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrItems, attrSpriteType});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 spriteSize = getIntValue(getAttribute(childs, attrSpriteSize), id, loopIdx, pMenu);
        QString tooltip = translate(getStringValue(getAttribute(childs, attrTooltip), id, loopIdx, pMenu));
        QString onEventLine = getAttribute(childs, attrOnEvent);
        QStringList items = getStringListValue(getAttribute(childs, attrItems), id, loopIdx, pMenu);
        QString spriteCreator = getStringValue(getAttribute(childs, attrSpriteType), id, loopIdx, pMenu);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        std::function<oxygine::spActor(QString item)> creator;
        if (spriteCreator == "unit")
        {
            Player* pPlayer = m_dropDownPlayer.get();
            creator = [this, pPlayer](QString id)
            {
                spUnit pSprite = MemoryManagement::create<Unit>(id, pPlayer, false, nullptr);
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
                oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                if (pAnim != nullptr)
                {
                    pSprite->setResAnim(pAnim);
                    pSprite->setScale(static_cast<float>(pAnim->getWidth()) / 32.0f);
                    pSprite->setSize(pAnim->getSize());
                }
                return pSprite;
            };
        }
        else if (spriteCreator == "army")
        {
            GameManager* pGameManager = GameManager::getInstance();
            creator = [pGameManager](QString army)
            {
                oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_" + army.toLower());
                oxygine::spSprite ret = MemoryManagement::create<oxygine::Sprite>();
                ret->setResAnim(pAnim);
                return ret;
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
                    spBuilding building = MemoryManagement::create<Building>(id, nullptr);
                    building->setTooltipText(building->getName());
                    building->updateBuildingSprites(false);
                    building->addChild(pTerrain);
                    pRet = building;
                }
                return pRet;
            };
        }
        spDropDownmenuSprite pDropDownmenu = MemoryManagement::create<DropDownmenuSprite>(width, items, creator, spriteSize);
        pDropDownmenu->setPosition(x, y);
        pDropDownmenu->setTooltipText(tooltip);
        pDropDownmenu->setVisible(visible);
        pDropDownmenu->setEnabled(enabled);
        if (!id.isEmpty())
        {
            pDropDownmenu->setObjectName(id);
        }
        if (hasChild(childs, attrStartValue))
        {
            bool success = false;
            qint32 intValue = getIntValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu, 0, &success);
            if (success)
            {
                pDropDownmenu->setCurrentItem(intValue);
            }
            else
            {
                QString value = getStringValue(getAttribute(childs, attrStartValue), id, loopIdx, pMenu);
                pDropDownmenu->setCurrentItem(value);

            }
        }
        DropDownmenuSprite* pDropDownmenuPtr = pDropDownmenu.get();
        connect(pDropDownmenu.get(), &DropDownmenuSprite::sigItemChanged, pMenu, [this, onEventLine, pDropDownmenuPtr, id, loopIdx, pMenu](qint32 value)
        {
            onEvent(onEventLine, value, pDropDownmenuPtr->getCurrentItemText(), id, loopIdx, pMenu);
        }, Qt::QueuedConnection);
        parent->addChild(pDropDownmenu);
        item = pDropDownmenu;
        m_lastCoordinates = QRect(x, y, pDropDownmenu->getScaledWidth(), pDropDownmenu->getScaledHeight());
        updateMenuSize(pMenu);
    }
    return success;
}

bool UiFactory::createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrSprite});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString spriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        spBox9Object pPanel = MemoryManagement::create<Box9Object>();
        if (!id.isEmpty())
        {
            pPanel->setObjectName(id);
        }
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        pPanel->setResAnim(pAnim);
        pPanel->setX(x);
        pPanel->setY(y);
        pPanel->setSize(width, height);
        pPanel->setScale(1);
        pPanel->setVisible(visible);
        pPanel->setEnabled(enabled);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        updateMenuSize(pMenu);
        m_parentSize = QSize(pPanel->getScaledWidth(), pPanel->getScaledHeight());
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
                success = success && createItem(pPanel, node.toElement(), panelItem, pMenu, loopIdx);
                if (panelItem.get() != nullptr)
                {
                    pPanel->addChild(panelItem);
                }
            }
            node = node.nextSibling();
        }
        parent->addChild(pPanel);
        m_parentSize = QSize(0, 0);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        item = pPanel;
    }
    return success;
}

bool UiFactory::createBoxTerrainPalette(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)
{
    auto childs = element.childNodes();
    bool success = checkElements(childs, {attrX, attrY, attrWidth, attrHeight, attrSprite});
    if (success)
    {
        QString id = getId(getStringValue(getAttribute(childs, attrId), "", loopIdx, pMenu));
        qint32 x = getIntValue(getAttribute(childs, attrX), id, loopIdx, pMenu);
        qint32 y = getIntValue(getAttribute(childs, attrY), id, loopIdx, pMenu);
        qint32 width = getIntValue(getAttribute(childs, attrWidth), id, loopIdx, pMenu);
        qint32 height = getIntValue(getAttribute(childs, attrHeight), id, loopIdx, pMenu);
        QString spriteId = getStringValue(getAttribute(childs, attrSprite), id, loopIdx, pMenu);
        bool visible = getBoolValue(getAttribute(childs, attrVisible), id, loopIdx, pMenu, true);
        bool enabled = getBoolValue(getAttribute(childs, attrEnabled), id, loopIdx, pMenu, true);
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        spBoxTerrainPalettePreview pPanel = MemoryManagement::create<BoxTerrainPalettePreview>();
        if (!id.isEmpty())
        {
            pPanel->setObjectName(id);
        }
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteId);
        pPanel->setResAnim(pAnim);
        pPanel->setX(x);
        pPanel->setY(y);
        pPanel->setSize(width, height);
        pPanel->setScale(1);
        pPanel->setVisible(visible);
        pPanel->setEnabled(enabled);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        updateMenuSize(pMenu);
        m_parentSize = QSize(pPanel->getScaledWidth(), pPanel->getScaledHeight());
        auto node = getNode(childs, attrChilds).firstChild();
        while (!node.isNull())
        {
            while (node.isComment())
            {
                node = node.nextSibling();
            }
            if (!node.isNull())
            {
                auto terrainElement = node.toElement();
                if (terrainElement.nodeName() == attrTerrain)
                {
                    auto terrainChilds = terrainElement.childNodes();
                    auto terrainX = getIntValue(getAttribute(terrainChilds, attrX), id, loopIdx, pMenu);
                    auto terrainY = getIntValue(getAttribute(terrainChilds, attrY), id, loopIdx, pMenu);
                    auto terrainId = getStringValue(getAttribute(terrainChilds, attrId), id, loopIdx, pMenu);
                    pPanel->addTerrain(terrainId, terrainX, terrainY);
                }
            }
            node = node.nextSibling();
        }
        parent->addChild(pPanel);
        m_parentSize = QSize(0, 0);
        m_lastCoordinates = QRect(x, y, pPanel->getScaledWidth(), pPanel->getScaledHeight());
        item = pPanel;
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
    for (const auto & attr : std::as_const(attributes))
    {
        for (qint32 i = 0; i < childCount; ++i)
        {
            if (childs.at(i).nodeName() == attr)
            {
                break;
            }
            else if (i == childCount - 1)
            {
                CONSOLE_PRINT("Missing attribute: " + attr, GameConsole::eERROR);
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

qint32 UiFactory::getIntValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, qint32 defaultValue, bool * success)
{
    qint32 value = defaultValue;

    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";" +
                              "var parentWidth = " + QString::number(m_parentSize.width()) + ";" +
                              "var parentHeight = " + QString::number(m_parentSize.height()) + ";" +
                              "var objectId = \"" + objectId + "\";" +
                              "var loopIdx = " + QString::number(loopIdx) + ";";
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(coordinates + line);
        if (success != nullptr)
        {
            *success = true;
        }
        if (erg.isNumber())
        {
            value = erg.toInt();
        }
        else if (erg.isBool())
        {
            value = erg.toBool();
        }
        else if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            }
        }
        else
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Unable to determine a int value while interpreting. Line: " + line, GameConsole::eERROR);
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

quint64 UiFactory::getUInt64Value(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, quint64 defaultValue, bool * success)
{
    quint64 value = defaultValue;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";" +
                              "var parentWidth = " + QString::number(m_parentSize.width()) + ";" +
                              "var parentHeight = " + QString::number(m_parentSize.height()) + ";" +
                              "var objectId = \"" + objectId + "\";" +
                              "var loopIdx = " + QString::number(loopIdx) + ";";
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(coordinates + line);
        if (success != nullptr)
        {
            *success = true;
        }
        if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        else if (erg.isBool())
        {
            value = erg.toBool();
        }
        else if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            }
        }
        else
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Unable to determine a int value while interpreting. Line: " + line, GameConsole::eERROR);
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

float UiFactory::getFloatValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, float defaultValue, bool * success)
{
    float value = defaultValue;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString coordinates = "var lastX = " + QString::number(m_lastCoordinates.x()) + ";" +
                              "var lastY = " + QString::number(m_lastCoordinates.y()) + ";" +
                              "var lastWidth = " + QString::number(m_lastCoordinates.width()) + ";" +
                              "var lastHeight = " + QString::number(m_lastCoordinates.height()) + ";" +
                              "var parentWidth = " + QString::number(m_parentSize.width()) + ";" +
                              "var parentHeight = " + QString::number(m_parentSize.height()) + ";" +
                              "var objectId = \"" + objectId + "\";" +
                              "var loopIdx = " + QString::number(loopIdx) + ";";
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(coordinates + line);
        if (success != nullptr)
        {
            *success = true;
        }
        if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        else if (erg.isBool())
        {
            value = erg.toBool();
        }
        else if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            }
        }
        else
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Unable to determine a int value while interpreting. Line: " + line, GameConsole::eERROR);
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

bool UiFactory::getBoolValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, bool defaultValue, bool * success)
{
    bool value = defaultValue;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(line);
        if (success != nullptr)
        {
            *success = true;
        }
        if (erg.isBool())
        {
            value = erg.toBool();
        }
        else if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        else if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            }
        }
        else
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Unable to determine a bool value while interpreting. Line: " + line, GameConsole::eERROR);
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

QString UiFactory::getStringValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, bool * success)
{
    QString value = line;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(line);
        if (success != nullptr)
        {
            *success = true;
        }
        if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            CONSOLE_PRINT_MODULE("Error while parsing " + line + " Error: " + erg.toString() + ". Using \"" + value + "\" as value.", GameConsole::eDEBUG, GameConsole::eUiFactory);
        }
        else if (erg.isString())
        {
            value = erg.toString();
        }
        else if (erg.isNumber())
        {
            value = QString::number(erg.toNumber());
        }
        else
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT_MODULE("Unable to determine a string value while interpreting. Line: " + line + " using line as value", GameConsole::eDEBUG, GameConsole::eUiFactory);
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

Player* UiFactory::getPlayerValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, bool * success)
{
    Player* value = nullptr;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            if (success != nullptr)
            {
                *success = false;
            }
            else
            {
                CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            }
        }
        else
        {
            value = erg.toVariant().value<Player*>();
            if (success != nullptr)
            {
                *success = true;
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

QStringList UiFactory::getStringListValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, bool * success)
{
    QStringList value;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            if (success != nullptr)
            {
                *success = false;
            }
        }
        else
        {
            value = erg.toVariant().toStringList();

            if (success != nullptr)
            {
                *success = true;
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return value;
}

QVector<qint32> UiFactory::getInt32ListValue(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu, bool * success)
{
    QVector<qint32> result;
    if (!line.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        line = "var objectId = \"" + objectId + "\";" +
               "var loopIdx = " + QString::number(loopIdx) + ";" + line;
        QJSValue erg = pInterpreter->evaluate(line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", GameConsole::eERROR);
            if (success != nullptr)
            {
                *success = false;
            }
        }
        else
        {
            auto values = erg.toVariant().toList();
            for (const auto & value : std::as_const(values))
            {
                result.append(value.toInt());
            }
            if (success != nullptr)
            {
                *success = true;
            }
        }
    }
    else
    {
        if (success != nullptr)
        {
            *success = false;
        }
    }
    return result;
}

oxygine::TextStyle UiFactory::getStyle(const QString & styleName, const QString & fontColor, qint32 size, oxygine::TextStyle::HorizontalAlign hAlign)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getInstance()->getFont(styleName, size));
    if (!fontColor.isEmpty())
    {
        style.color = QColor(fontColor);
    }
    style.hAlign = hAlign;
    style.multiline = false;
    return style;
}

oxygine::TextStyle::HorizontalAlign UiFactory::getHAlignment(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu)
{
    return getEnumValue(line, objectId, loopIdx, pMenu, {"Default", "Left", "Middle", "Right"}, oxygine::TextStyle::HALIGN_LEFT);
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

void UiFactory::doEvent(QString command, QString objectId, qint32 loopIdx, CreatedGui* pMenu)
{
    QString args;
    args += "var objectId = \"" + objectId + "\";";
    args += "var loopIdx = " + QString::number(loopIdx) + ";";
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue obj = pInterpreter->newQObject(pMenu);
    pInterpreter->setGlobal("currentMenu", obj);
    args += command;
    QJSValue erg = pInterpreter->evaluate(args);
    if (erg.isError())
    {
        CONSOLE_PRINT("Error while parsing " + command + " Error: " + erg.toString() + ".", GameConsole::eERROR);
    }
}

void UiFactory::updateMenuSize(CreatedGui* pMenu)
{
    qint32 width = m_lastCoordinates.x() + m_lastCoordinates.width();
    if (pMenu->getWidth() < width)
    {
        pMenu->setWidth(width);
    }
    qint32 height = m_lastCoordinates.y() + m_lastCoordinates.height();
    if (pMenu->getHeight() < height)
    {
        pMenu->setHeight(height);
    }
}
