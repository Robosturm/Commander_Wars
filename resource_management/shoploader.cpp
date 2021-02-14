#include "shoploader.h"

ShopLoader::ShopLoader()
    : RessourceManagement<ShopLoader>("/images/shop/res.xml",
                                              "/scripts/shop")
{

}

void ShopLoader::loadAll()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    RessourceManagement<ShopLoader>::loadAll();
    for (auto achievement : m_loadedRessources)
    {
        pInterpreter->doFunction(achievement, "loadShopItems");
    }
}

void ShopLoader::onItemBought(qint32 itemType, QString key)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    args << itemType;
    args << key;
    for (auto shopLoader : m_loadedRessources)
    {
        pInterpreter->doFunction(shopLoader, "itemBought", args);
    }
}
