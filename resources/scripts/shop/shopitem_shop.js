var Constructor = function()
{

    this.items = [  [GameEnums.ShopItemType_CO_Skin,    "CO_NELL",                      qsTr("Nell"),     800,    true],
                    [GameEnums.ShopItemType_CO,         "CO_NELL",                      qsTr("Nell"),     800,    true],

                    // maps and campaigns
                    [GameEnums.ShopItemType_Map,        "maps/2_player",                qsTr("Test"),     800,    true],
                    [GameEnums.ShopItemType_Map,        "maps/test.map",                qsTr("Test"),     800,    true],
                    [GameEnums.ShopItemType_Map,        "maps/war_room.jsm",            qsTr("Test"),     800,    true],];

    this.loadShopItems = function()
    {
        for (var i = 0; i < SHOPITEM_SHOP.items.length; ++i)
        {
            var data = SHOPITEM_SHOP.items[i];
            userdata.addShopItem(data[0],
                                 data[1],
                                 data[2],
                                 data[3],
                                 data[4]);
        }
    };
};

Constructor.prototype = SHOPITEM;
var SHOPITEM_SHOP = new Constructor();
