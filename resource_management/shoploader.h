#ifndef SHOPLOADER_H
#define SHOPLOADER_H

#include <QObject>
#include "resource_management/ressourcemanagement.h"

class ShopLoader : public QObject, public RessourceManagement<ShopLoader>
{
    Q_OBJECT
public:
    virtual void loadAll() override;
    void onItemBought(qint32 itemType, QString key);
protected:
    friend RessourceManagement<ShopLoader>;
    ShopLoader();

private:
    virtual ~ShopLoader() = default;
};

#endif // SHOPLOADER_H
