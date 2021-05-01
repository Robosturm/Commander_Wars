#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QObject>
#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include <QDomDocument>

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
     * @brief createLabel
     * @param parent
     * @param element
     * @param item
     * @return
     */
    bool createLabel(oxygine::spActor parent, QDomElement element, oxygine::spActor item);

private:
    explicit UiFactory();
    bool createItem(oxygine::spActor parent, QDomElement element, oxygine::spActor item);
    qint32 getValue(QString line);
    oxygine::TextStyle getStyle(QString styleName);
private:

    static UiFactory m_pUiFactory;
    QVector<FactoryItem> m_factoryItems;
};

#endif // UIFACTORY_H
