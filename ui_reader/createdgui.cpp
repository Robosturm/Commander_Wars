#include "ui_reader/createdgui.h"
#include "ui_reader/uifactory.h"

#include "coreengine/mainapp.h"
#include "objects/base/moveinbutton.h"

CreatedGui::~CreatedGui()
{
    for (auto & pItem : m_factoryUiItem)
    {
        pItem->detach();
    }
}

void CreatedGui::addFactoryUiItem(oxygine::spActor pItem)
{
    pItem->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_factoryUiItem.append(pItem);
    addChild(pItem);
}

void CreatedGui::setEnabled(bool value)
{
    for (auto & item : m_factoryUiItem)
    {
        spMoveInButton pMoveInButton = oxygine::dynamic_pointer_cast<MoveInButton>(item);
        if (pMoveInButton.get() == nullptr)
        {
            item->setEnabled(value);
        }
    }
}

void CreatedGui::resetUi()
{
    for (auto & item : m_factoryUiItem)
    {
        item->detach();
    }
    m_factoryUiItem.clear();
}

void CreatedGui::loadXml(QString xmlFile)
{
    UiFactory::getInstance().createUi(xmlFile, this);
}

void CreatedGui::setObjectEnabled(const QString id, bool value)
{
    for (auto & item : m_factoryUiItem)
    {
        auto* pObject = dynamic_cast<QObject*>(item.get());
        if (pObject != nullptr &&
            pObject->objectName() == id)
        {
            item->setEnabled(value);
        }
    }
}
