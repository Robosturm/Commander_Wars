#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QObject>
#include <QDomDocument>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/interpreter.h"
#include "coreengine/console.h"

class UiFactory : public QObject
{
    Q_OBJECT
public:
    using CreatorFunction = std::function<bool(oxygine::spActor, QDomElement, oxygine::spActor item)>;
    struct FactoryItem
    {
        QString m_id;
        CreatorFunction m_creator;
    };
    static UiFactory & getInstance()
    {
        return m_pUiFactory;
    }
    /**
     * @brief createUi
     * @param uiXmlFile
     */
    oxygine::spActor createUi(QString uiXmlFile, bool & success);
    /**
     * @brief getFactoryItems
     * @return
     */
    QVector<UiFactory::FactoryItem> & getFactoryItems();

private:
    /**
     * Nodename: Label
     * supported attributes are:
     * x, y, width, heigth, text, tooltip and font
     */
    bool createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor item);
    /**
      * Nodename: Checkbox
      * supported attributes are:
      * x, y, tooltip, onEvent, startValue
      */
    bool createCheckbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item);
    /**
      * Nodename: Spinbox
      * supported attributes are:
      * x, y, width, min, max, infinite, tooltip, onEvent, startValue
      */
    bool createSpinbox(oxygine::spActor parent, QDomElement element, oxygine::spActor item);

private:
    explicit UiFactory();
    bool createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor item);
    qint32 getIntValue(QString line);
    bool getBoolValue(QString line);
    QString getStringValue(QString line);
    oxygine::TextStyle getStyle(QString styleName);
    template<typename TType>
    void onEvent(QString line, TType value)
    {
        QString args = "var input = " + QString::number(value) + ";";
        QJSValue erg = Interpreter::getInstance()->evaluate(args + line);
        if (erg.isError())
        {
            Console::print("Error while parsing " + line + " Error: " + erg.toString() + ".", Console::eERROR);
        }
    }
private:

    static UiFactory m_pUiFactory;
    QVector<FactoryItem> m_factoryItems;
};

#endif // UIFACTORY_H
