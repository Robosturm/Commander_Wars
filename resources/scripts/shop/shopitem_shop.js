var Constructor = function()
{

    // some notes for item and id matches
    // Skin and CO items always refer to the COID. So you can't enable a certain skin in the shop
    // maps refer to either a folder a map or a whole campaign relative to the commander wars exe e.g.: maps/war_room.jsm would disable the war room until it's bought/unlocked
    // units and perks refer to the id of said perk/unit
    // items can always be banned in the rule selection independent of the unlock state. However you want be able to select them for yourself.
    // However clients/hosts which enabled a co or perk may still use it even if you as a host didn't unlock them.
    // for testing or unlocking all items start a map and open the console (F1) and enter the command userdata.unlockAllShopItems(true)
    // use userdata.unlockAllShopItems(false) to make them all available in the shop but not bought yet.
    // deleting a .dat userfile will delete all the progress made.

    this.items = [// co's
                  [GameEnums.ShopItemType_CO,         "CO_NELL",            qsTr("Nell"),        900,    true],
                  [GameEnums.ShopItemType_CO,         "CO_HACHI",           qsTr("Hachi"),      1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_ALEXANDER",       qsTr("Alexander"),   300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_PETER",           qsTr("Peter"),       300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_SMITAN",          qsTr("Smitan"),     1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_RATTIGAN",        qsTr("Rattigan"),    300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_HAWKE",           qsTr("Hawke"),      1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_STURM",           qsTr("Sturm"),      1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_VON_BOLT",        qsTr("Von Bolt"),   1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_IK_486_B7",       qsTr("IK-486-B7"),  1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_NAPOLEON",        qsTr("Napoleon"),   1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_ROBOSTURM",       qsTr("Robo-Sturm"), 1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_SABAKI",          qsTr("Sabaki"),      300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_SOPHIE",          qsTr("Sophie"),      300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_OZZY",            qsTr("Ozzy"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_MARY",            qsTr("Mary"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_ADAM",            qsTr("Adam"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_ZANDRA",          qsTr("Zandra"),     1000,    true],
                  [GameEnums.ShopItemType_CO,         "CO_MINAMOTO",        qsTr("Minamoto"),    300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_XAVIER",          qsTr("Xavier"),     1000,    true],
                  [GameEnums.ShopItemType_CO,         "CO_SANJURO",         qsTr("Sanjuro"),     300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_MEIYO",           qsTr("Meiyo"),      1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_MINA",            qsTr("Mina"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_AIRA",            qsTr("Aira"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_ALEXIS",          qsTr("Alexis"),      300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_NANA",            qsTr("Nana"),       1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_AMY",             qsTr("Amy"),        1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_WALTER",          qsTr("Walter"),      300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_JOEY",            qsTr("Joey"),        300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_CASSIDY",         qsTr("Cassidy"),     300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_CONRAD",          qsTr("Conrad"),      900,    true],
                  [GameEnums.ShopItemType_CO,         "CO_BEAST",           qsTr("Beast"),       300,    true],
                  [GameEnums.ShopItemType_CO,         "CO_MELANTHE",        qsTr("Melanthe"),   1200,    true],
                  [GameEnums.ShopItemType_CO,         "CO_GRAVES",          qsTr("Graves"),     1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_TABITHA",         qsTr("Tabitha"),    1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_VARLOT",          qsTr("Varlot"),     1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_JULIA",           qsTr("Julia"),      1600,    true],
                  [GameEnums.ShopItemType_CO,         "CO_CAULDER",         qsTr("Caulder"),    2000,    true],
                  [GameEnums.ShopItemType_CO,         "CO_YUKIO",           qsTr("Yukio"),      2000,    true],
                  // skins
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ANDY",            qsTr("Andy's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MAX",             qsTr("Max's Outfits"),      300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SAMI",            qsTr("Sami's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JAKE",            qsTr("Jake's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_RACHEL",          qsTr("Rachel's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_NELL",            qsTr("Nell's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_HACHI",           qsTr("Hachi's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_GRIT",            qsTr("Grit's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_COLIN",           qsTr("Colin's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SASHA",           qsTr("Sasha's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_OLAF",            qsTr("Olaf's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ALEXANDER",       qsTr("Alexander's Outfits"),300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_DRAKE",           qsTr("Drake's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_EAGLE",           qsTr("Eagle's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JESS",            qsTr("Jess's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JAVIER",          qsTr("Javier's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_PETER",           qsTr("Peter's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SMITAN",          qsTr("Smitan's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SONJA",           qsTr("Sonja's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SENSEI",          qsTr("Sensei's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_GRIMM",           qsTr("Grimm's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_KANBEI",          qsTr("Kanbei's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_RATTIGAN",        qsTr("Rattigan's Outfits"), 300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_FLAK",            qsTr("Flak's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ADDER",           qsTr("Adder's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_LASH",            qsTr("Lash's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_HAWKE",           qsTr("Hawke's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_STURM",           qsTr("Sturm's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JUGGER",          qsTr("Jugger's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_KOAL",            qsTr("Koal's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_KINDLE",          qsTr("Kindle's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_VON_BOLT",        qsTr("Von Bolt's Outfits"), 300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_EPOCH",           qsTr("Epoch's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ROBOANDY",        qsTr("Robo-Andy's Outfits"),300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_IK_486_B7",       qsTr("IK-486-B7's Outfits"),300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_NAPOLEON",        qsTr("Napoleon's Outfits"), 300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ROBOSTURM",       qsTr("Robo-Sturm's Outfits"),300,   true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SABAKI",          qsTr("Sabaki's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SOPHIE",          qsTr("Sophie's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_OZZY",            qsTr("Ozzy's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MARY",            qsTr("Mary's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ADAM",            qsTr("Adam's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ZANDRA",          qsTr("Zandra's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MINAMOTO",        qsTr("Minamoto's Outfits"), 300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_XAVIER",          qsTr("Xavier's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_SANJURO",         qsTr("Sanjuro's Outfits"),  300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MEIYO",           qsTr("Meiyo's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_AIRA",            qsTr("Aira's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MINA",            qsTr("Mina's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_ALEXIS",          qsTr("Alexis's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_NANA",            qsTr("Nana's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_AMY",             qsTr("Amy's Outfits"),      300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_CONRAD",          qsTr("Conrad's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_CASSIDY",         qsTr("Cassidy's Outfits"),  300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JOEY",            qsTr("Joey's Outfits"),     300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_WALTER",          qsTr("Walter's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_MELANTHE",        qsTr("Melanthe's Outfits"), 300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_JULIA",           qsTr("Julia's Outfits"),    300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_GRAVES",          qsTr("Graves's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_VARLOT",          qsTr("Varlot's Outfits"),   300,    true],
                  [GameEnums.ShopItemType_CO_Skin,    "CO_YUKIO",           qsTr("Yukio's Outfits"),    300,    true],
                 ];

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
