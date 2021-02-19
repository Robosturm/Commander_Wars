#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>

#include <qimage.h>

#include <qvector.h>
#include <qmap.h>


#include "coreengine/fileserializable.h"
#include "game/GameEnums.h"

class Userdata : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    /**
     * @brief The Achievement struct
     */
    struct Achievement
    {
        QString id;
        qint32 progress{0};
        qint32 targetValue{0};
        QString name;
        QString description;
        QString icon;
        bool hide{false};
        bool loaded{false};
    };

    static constexpr qint32 MAX_VICTORY_INFO_PER_MAP = 5;
    /**
     * @brief The MapVictoryInfo struct
     */
    struct MapVictoryInfo
    {
        QStringList co1;
        QStringList co2;
        QVector<qint32> score;
    };

    struct ShopItem
    {
        QString key;
        /**
         * @brief name in the shop
         */
        QString name;
        /**
         * @brief price in the shop
         */
        qint32 price;
        /**
         * @brief buyable unlocked for the shop
         */
        bool buyable;
        /**
         * @brief bought in the shop and available for the player
         */
        bool bought;
        /**
         * @brief itemType used for filterin and showing item icons
         */
        GameEnums::ShopItemType itemType;
    };

    static Userdata* getInstance();
    void changeUser();
    void storeUser();
    void loadShopData();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 6;
    }
    /**
     * @brief addCOStyle
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void addCOStyle(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
    /**
     * @brief removeCOStyle
     * @param coid
     */
    void removeCOStyle(QString coid);
    /**
     * @brief getCOStyle
     * @param coid
     * @return
     */
    std::tuple<QString, QString, QImage, QImage, bool>* getCOStyle(QString coid);
    /**
     * @brief getAchievements
     * @return
     */
    QVector<Achievement>* getAchievements();
    /**
     * @brief addVictoryForMap
     * @param mapPath
     * @param co1
     * @param co2
     * @param score
     */
    void addVictoryForMap(QString mapPath, QString co1, QString co2, qint32 score);
    /**
     * @brief getVictoryForMap
     * @param mapPath
     * @return
     */
    const MapVictoryInfo * getVictoryForMap(QString mapPath);
    /**
     * @brief getItems
     * @param type
     * @param buyable
     * @param bought
     * @return
     */
    QVector<ShopItem> getItems(GameEnums::ShopItemType type, bool buyable, bool bought);
    /**
     * @brief Userdata::getItems
     * @param type
     * @param bought
     * @return
     */
    QVector<Userdata::ShopItem> getItems(GameEnums::ShopItemType type, bool bought);
    /**
     * @brief Userdata::getItems
     * @param type
     * @param bought
     * @return
     */
    QStringList getItemsList(GameEnums::ShopItemType type, bool bought);
signals:

public slots:
    /**
     * @brief getCredtis
     * @return
     */
    qint32 getCredtis() const;
    /**
     * @brief setCredtis
     * @param credtis
     */
    void setCredtis(const qint32 &credtis);
    /**
     * @brief addCredtis
     * @param credtis
     */
    void addCredtis(const qint32 &credtis);
    /**
     * @brief addAchievement
     * @param id
     * @param value
     */
    void increaseAchievement(QString id, qint32 value);
    /**
     * @brief addAchievement
     * @param id
     * @param targetValue
     * @param name
     * @param description
     * @param icon
     * @param hide
     */
    void addAchievement(QString id, qint32 targetValue, QString name, QString description, QString icon, bool hide = false);
    /**
     * @brief deleteAchievement
     * @param id
     */
    void deleteAchievement(QString id);
    /**
     * @brief achieved
     * @param id
     * @return
     */
    bool achieved(QString id);
    /**
     * @brief getActiveCoStyle
     * @param coid
     */
    QString getActiveCoStyle(QString coid);
    /**
     * @brief addShopItem adds an item to the shop list
     * @param itemType what sort of item it's used to disable the item and for showing an icon in the shop
     * @param key identifier of the item disabled for a co/skin it's his coid. For a map/campaign it's the path relative to the exe seperated by /
     * @param name shown in the shop
     * @param price for the item to be bought
     * @param buyable if the item is buyable right from the start
     */
    void addShopItem(GameEnums::ShopItemType itemType, QString key, QString name, qint32 price, bool buyable = false);
    /**
     * @brief setShopItemBuyable changes if an item can be bought in the shop or not
     * @param itemType
     * @param key
     * @param buyable
     */
    void setShopItemBuyable(GameEnums::ShopItemType itemType, QString key, bool buyable);
    /**
     * @brief setShopItemBought changes if the item has been bought already can be used to enabe items by progress in a campaign or map.
     * @param itemType
     * @param key
     * @param bought
     */
    void setShopItemBought(GameEnums::ShopItemType itemType, QString key, bool bought);
    /**
     * @brief unlockAllShopItems (little cheat for testing and for people who don't wanna grind)
     * @param bought if true the items get bought as well
     */
    void unlockAllShopItems(bool bought);
private:
    void showAchieved();

private:
    explicit Userdata();

    static Userdata* m_pInstance;

    QVector<std::tuple<QString, QString, QImage, QImage, bool>> m_customCOStyles;
    QVector<Achievement> m_achievements;
    QMap<QString, MapVictoryInfo> m_mapVictoryInfo;
    QVector<ShopItem> m_shopItems;
    /**
     * @brief m_credtis money available to spend in the shop
     */
    qint32 m_credtis{0};
};

#endif // USERDATA_H
