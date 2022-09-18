#include "shoploader.h"

ShopLoader::ShopLoader()
    : RessourceManagement<ShopLoader>("/images/shop/res.xml",
                                              "/scripts/shop")
{
#ifdef GRAPHICSUPPORT
    setObjectName("ShopLoader");
#endif
    Interpreter::setCppOwnerShip(this);
}

void ShopLoader::loadAll()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    RessourceManagement<ShopLoader>::loadAll();
    for (const auto & achievement : qAsConst(m_loadedRessources))
    {
        pInterpreter->doFunction(achievement, "loadShopItems");
    }
}

void ShopLoader::onItemBought(qint32 itemType, QString key)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({itemType, key});
    for (const auto &  shopLoader : qAsConst(m_loadedRessources))
    {
        pInterpreter->doFunction(shopLoader, "itemBought", args);
    }
}
