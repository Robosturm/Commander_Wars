#include "ui_reader/createdgui.h"

#include "coreengine/mainapp.h"

CreatedGui::CreatedGui()
    : QObject()
{
}

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
