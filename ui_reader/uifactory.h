#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QObject>
#include <QDomDocument>
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "coreengine/interpreter.h"
#include "coreengine/console.h"

#include "game/player.h"

#include "ui_reader/createdgui.h"

class UiFactory : public QObject
{
    Q_OBJECT
public:
    using CreatorFunction = std::function<bool(oxygine::spActor, QDomElement, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx)>;
    struct FactoryItem
    {
        QString m_id;
        CreatorFunction m_creator;
    };
    static UiFactory & getInstance()
    {
        if (m_pUiFactory == nullptr)
        {
            m_pUiFactory = new UiFactory();
        }
        return *m_pUiFactory;
    }
    static void shutdown();
    /**
     * @brief createUi
     * @param uiXmlFile
     */
    void createUi(QString uiXml, CreatedGui* pMenu);
    /**
     * @brief getFactoryItems
     * @return
     */
    QVector<UiFactory::FactoryItem> & getFactoryItems();
private:
    // this section contains the information about the elements and the attributes supported by the xml
    /**
     * Nodename: Label
     * supported attributes are:
     * mandatory: x, y, width, heigth, text, font,
     * optional: tooltip, onUpdate, id, enabled, fontColor
     */
    bool createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: DropDownMenu
     * supported attributes are:
     * mandatory: x, y, width, items
     * optional: tooltip, onEvent, id, enabled, startValue
     */
    bool createDropDownMenu(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: DropDownMenuColor
     * supported attributes are:
     * mandatory: x, y, width, items
     * optional: tooltip, onEvent, id, enabled, startValue
     */
    bool createDropDownMenuColor(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: DropDownMenuSprite
     * supported attributes are:
     * mandatory: x, y, width, items, spriteType, spriteSize
     * optional: tooltip, onEvent, id, enabled, startValue
     */
    bool createDropDownMenuSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: TextField
     * supported attributes are:
     * mandatory: x, y, text, font
     * optional: fontColor, id
     */
    bool createTextfield(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Checkbox
      * supported attributes are:
      * mandatory: x, y, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Spinbox
      * supported attributes are:
      * mandatory: x, y, width, min, max, infinite, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Slider
      * supported attributes are:
      * mandatory: x, y, width, min, max, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Textbox
      * supported attributes are:
      * mandatory: x, y, width, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Passwordbox
      * supported attributes are:
      * mandatory: x, y, width, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createPasswordbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: TimeSpinbox
      * supported attributes are:
      * mandatory: x, y, width, onEvent, startValue
      * optional: tooltip, id, enabled
      */
    bool createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Panel
      * supported attributes are:
      * mandatory: x, y, width, height, childs
      * optional: tooltip, id, enabled
      */
    bool createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Box
      * supported attributes are:
      * mandatory: x, y, width, height, sprite, childs
     * optional: id
     */
    bool createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
      * Nodename: Icon
      * supported attributes are:
      * mandatory: x, y, size, sprite
      * optional: id, player
      */
    bool createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: Button
     * supported attributes are:
     * mandatory: x, y, text, onEvent
     * optional: tooltip, enabled, id
     */
    bool createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: SelectKey
     * supported attributes are:
     * mandatory: x, y, startValue, onEvent
     * optional: tooltip, enabled, id
     */
    bool createSelectKey(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: IconButton
     * supported attributes are:
     * mandatory: x, y, sprite, onEvent
     * optional: enabled, id
     */
    bool createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: MoveInButton
     * supported attributes are:
     * mandatory: moveInSize
     * optional: enabled, direction, scale, useY, startOffset
     */
    bool createMoveInButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: Sprite
     * supported attributes are:
     * mandatory: x, y, sprite, scale
     * optional: id
     */
    bool createSprite(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: loop
     * attribute: count
     * Example:
     * <loop count=2>
     * </loop>
     */
    bool loop(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    /**
     * Nodename: if
     * attribute: condition
     * Example:
     * <if condition=2>
     * </if>
     */
    bool ifCondition(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
signals:
    void sigDoEvent(QString command, QString objectId, qint32 loopIdx, CreatedGui* pMenu);
private slots:
    void doEvent(QString command, QString objectId, qint32 loopIdx, CreatedGui* pMenu);

private:
    explicit UiFactory();
    /**
     * @brief checkArray
     * @param attributes
     * @return
     */
    bool checkElements(const QDomNodeList & childs, const QStringList & attributes);
    QString getAttribute(const QDomNodeList & childs, const QString & attribute);
    QDomNode getNode(const QDomNodeList & childs, const QString & attribute);
    bool hasChild(const QDomNodeList & childs, const QString & attribute);
    bool createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, CreatedGui* pMenu, qint32 loopIdx = 0);
    qint32 getIntValue(QString line, QString objectId, qint32 loopIdx, qint32 defaultValue = 0);
    quint64 getUInt64Value(QString line, QString objectId, qint32 loopIdx, quint64 defaultValue = 0);
    float getFloatValue(QString line, QString objectId, qint32 loopIdx, float defaultValue = 0.0f);
    bool getBoolValue(QString line, QString objectId, qint32 loopIdx, bool defaultValue = false);
    QString getStringValue(QString line, QString objectId, qint32 loopIdx);

    Player* getPlayerValue(QString line, QString objectId, qint32 loopIdx);
    QStringList getStringListValue(QString line, QString objectId, qint32 loopIdx);
    oxygine::TextStyle getStyle(QString styleName, QColor fontColor, oxygine::TextStyle::HorizontalAlign hAlign);
    QString getId(QString attribute);
    oxygine::TextStyle::HorizontalAlign getHAlignment(QString line, QString objectId, qint32 loopIdx);

    template<typename TType>
    void onEvent(QString line, TType value, QString objectId, qint32 loopIdx, CreatedGui* pMenu)
    {
        QString args;
        if constexpr(std::is_same<QString, TType>::value)
        {
            args += "var input = \"" + value + "\";";
        }
        else
        {
            args += "var input = " + QString::number(value) + ";";
        }
        args += "var objectId = \"" + objectId + "\";";
        args += "var loopIdx = " + QString::number(loopIdx) + ";";
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(args + line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + args + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
    }

    template<typename TType, typename TType2>
    void onEvent(QString line, TType value, TType2 value2, QString objectId, qint32 loopIdx, CreatedGui* pMenu)
    {
        QString args;
        if constexpr(std::is_same<QString, TType>::value)
        {
            args += "var input = \"" + value + "\";";
        }
        else
        {
            args += "var input = " + QString::number(value) + ";";
        }
        if constexpr(std::is_same<QString, TType2>::value)
        {
            args += "var input2 = \"" + value2 + "\";";
        }
        else
        {
            args += "var input2 = " + QString::number(value2) + ";";
        }
        args += "var objectId = \"" + objectId + "\";";
        args += "var loopIdx = " + QString::number(loopIdx) + ";";
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(args + line);
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + args + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
    }

    template<typename TType>
    TType onUpdate(QString line, QString objectId, qint32 loopIdx, CreatedGui* pMenu)
    {
        QString args;
        args += "var objectId = \"" + objectId + "\";";
        args += "var loopIdx = " + QString::number(loopIdx) + ";";
        args += line;
        TType ret;
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue obj = pInterpreter->newQObject(pMenu);
        pInterpreter->setGlobal("currentMenu", obj);
        QJSValue erg = pInterpreter->evaluate(args);
        if constexpr(std::is_same<QString, TType>::value)
        {
            ret = erg.toString();
        }
        else if constexpr(std::is_arithmetic<TType>::value)
        {
            ret = erg.toNumber();
        }
        else
        {
            CONSOLE_PRINT("Unable to determine a return value while interpreting. Line: " + line, Console::eERROR);
        }
        if (erg.isError())
        {
            CONSOLE_PRINT("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        return ret;
    }
    static QString translate(QString line);

    template<typename TEnum>
    TEnum getEnumValue(QString line, QString objectId, qint32 loopIdx, QStringList values, TEnum defaultValue = 0)
    {
        TEnum ret = defaultValue;
        QString value = getStringValue(line, objectId, loopIdx);
        auto index = values.indexOf(value);
        if (index >= 0)
        {
            ret = static_cast<TEnum>(index);
        }
        return ret;
    }

private:

    static UiFactory* m_pUiFactory;
    QVector<FactoryItem> m_factoryItems;
    QRect m_lastCoordinates;
    quint32 m_creationCount{0};
    spPlayer m_dropDownPlayer;
};

Q_DECLARE_INTERFACE(UiFactory, "UiFactory");

#endif // UIFACTORY_H
