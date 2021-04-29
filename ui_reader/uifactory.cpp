#include "ui_reader/uifactory.h"

#include "coreengine/console.h"

QVector<UiFactory::FactoryItem> UiFactory::m_factoryItems;
UiFactory UiFactory::m_pUiFactory;

UiFactory::UiFactory()
    : QObject()
{

}

QVector<UiFactory::FactoryItem> & UiFactory::getFactoryItems()
{
    return m_factoryItems;
}

oxygine::spActor UiFactory::createUi(QString uiXmlFile, bool & success)
{
    oxygine::spActor root = new oxygine::Actor();
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
                    createItem(root, node);
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

bool UiFactory::createItem(oxygine::spActor parent, QDomNode element)
{
    QString name = element.nodeName();
    bool success = false;
    for (auto & item : m_factoryItems)
    {
        if (item.id == name)
        {
            success = item.creator(parent, element);
            break;
        }
    }
    if (!success)
    {
        Console::print("Unable to create item: " + name + ".", Console::eERROR);
    }
    return success;
}

bool UiFactory::createLabel(oxygine::spActor parent, QDomNode element)
{
    return false;
}
