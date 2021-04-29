#ifndef UIFACTORY_H
#define UIFACTORY_H

#include <QObject>
#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include <QDomDocument>

class UiFactory : public QObject
{
    Q_OBJECT
public:
    struct FactoryItem
    {
        QString id;
        std::function<bool(oxygine::spActor, QDomNode)> creator;
    };

    UiFactory & getInstance()
    {
        return m_pUiFactory;
    }
    /**
     * @brief createUi
     * @param uiXmlFile
     */
    static oxygine::spActor createUi(QString uiXmlFile, bool & success);

    static QVector<UiFactory::FactoryItem> & getFactoryItems();

private:
    explicit UiFactory();
    static bool createItem(oxygine::spActor parent, QDomNode element);
    static bool createLabel(oxygine::spActor parent, QDomNode element);

private:
    static UiFactory m_pUiFactory;
    static QVector<FactoryItem> m_factoryItems;
};

#endif // UIFACTORY_H
