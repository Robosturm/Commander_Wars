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
