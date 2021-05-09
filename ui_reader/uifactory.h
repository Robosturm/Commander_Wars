#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QObject>
#include <QDomDocument>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/interpreter.h"
#include "coreengine/console.h"
#include "menue/basemenu.h"

class UiFactory : public QObject
{
    Q_OBJECT
public:
    using CreatorFunction = std::function<bool(oxygine::spActor, QDomElement, oxygine::spActor & item, Basemenu* pMenu)>;
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
    /**
     * @brief createUi
     * @param uiXmlFile
     */
    void createUi(QString uiXml, Basemenu* pMenu);
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
     * optional: tooltip, onUpdate
     */
    bool createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    /**
      * Nodename: Checkbox
      * supported attributes are:
      * mandatory: x, y, onEvent, startValue
      * optional: tooltip
      */
    bool createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    /**
      * Nodename: Spinbox
      * supported attributes are:
      * mandatory: x, y, width, min, max, infinite, onEvent, startValue
      * optional: tooltip
      */
    bool createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    /**
      * Nodename: Slider
      * supported attributes are:
      * mandatory: x, y, width, min, max, onEvent, startValue
      * optional: tooltip
      */
    bool createSlider(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
    /**
      * Nodename: Textbox
      * supported attributes are:
      * mandatory: x, y, width, onEvent, startValue
      * optional: tooltip
      */
    bool createTextbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
    /**
      * Nodename: TimeSpinbox
      * supported attributes are:
      * mandatory: x, y, width, onEvent, startValue
      * optional: tooltip
      */
    bool createTimeSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
    /**
      * Nodename: Panel
      * supported attributes are:
      * mandatory: x, y, width, height, childs
      */
    bool createPanel(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
    /**
      * Nodename: Box
      * supported attributes are:
      * mandatory: x, y, width, height, sprite, childs
     */
    bool createBox(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    /**
      * Nodename: Icon
      * supported attributes are:
      * mandatory: x, y, size, startValue
      */
    bool createIcon(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
    /**
     * Nodename: Button
     * supported attributes are:
     * mandatory: x, y, text, onEvent
     * optional: tooltip, width
     */
    bool createButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    /**
     * Nodename: Button
     * supported attributes are:
     * mandatory: x, y, sprite, onEvent
     */
    bool createIconButton(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu*);
signals:
    void sigDoEvent(QString command);
private slots:
    void doEvent(QString command);

private:
    explicit UiFactory();
    /**
     * @brief checkArray
     * @param attributes
     * @return
     */
    bool checkElements(QDomNodeList element, QVector<QString> attributes);
    QString getAttribute(QDomNodeList childs, QString attribute);
    QDomNode getNode(QDomNodeList childs, QString attribute);
    bool createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor & item, Basemenu* pMenu);
    qint32 getIntValue(QString line);
    bool getBoolValue(QString line);
    QString getStringValue(QString line);
    oxygine::TextStyle getStyle(QString styleName);
    template<typename TType>
    void onEvent(QString line, TType value)
    {
        QString args;
        if constexpr(std::is_same<QString, TType>::value)
        {
            args = "var input = " + value + ";";
        }
        else
        {
            args = "var input = " + QString::number(value) + ";";
        }
        QJSValue erg = Interpreter::getInstance()->evaluate(args + line);
        if (erg.isError())
        {
            Console::print("Error while parsing " + args + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
    }
    template<typename TType>
    TType onUpdate(QString line)
    {
        TType ret;
        QJSValue erg = Interpreter::getInstance()->evaluate(line);

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
            Console::print("Unable to determine a return value while interpreting. Line: " + line, Console::eERROR);
        }
        if (erg.isError())
        {
            Console::print("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
        return ret;
    }
    static QString translate(QString line);
private:

    static UiFactory* m_pUiFactory;
    QVector<FactoryItem> m_factoryItems;
    QRect m_lastCoordinates;
};

#endif // UIFACTORY_H
