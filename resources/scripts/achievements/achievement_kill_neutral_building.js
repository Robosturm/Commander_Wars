var Constructor = function()
{
    this.buildingTable = [  ["ZBLACKHOLE_CANNON",   "ZBLACKHOLE_CANNON_S",       50,     qsTr("Nice!")],
                            ["ZCRYSTALL",           "ZCRYSTALL",                 100,    qsTr("Smashing!")],
                            ["ZDEATHRAY",           "ZDEATHRAY_S",               50,     qsTr("Beaming!")],
                            ["ZMINICANNON",         "ZMINICANNON_S",             200,    qsTr("Yeah!")],
                            ["ZMONOLITH",           "ZMONOLITH",                 50,     qsTr("Oh, boy!")],
                            ["ZTALON_GUN",          "ZTALON_GUN",                50,     qsTr("Aurora explodes!")],
                            ["ZLASER",              "ZLASER",                    100,    qsTr("No lasers!")],
                            ["ZGATE",               "ZGATE_E_W",                 100,    qsTr("We can walk here!")],
                            ["WELD",                "WELD",                      100,    qsTr("Why?!")],
                            ["WEAK_WALL",           "WEAK_WALL",                 100,    qsTr("We can pass here!")],
                            ["METEOR",              "METEOR",                    100,    qsTr("Was that Sturm?")],];

    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_KILL_NEUTRAL_BUILDING.buildingTable.length; ++i)
        {
            var data = ACHIEVEMENT_KILL_NEUTRAL_BUILDING.buildingTable[i];
            userdata.addAchievement("KILL_NEUTRAL_BUILDING_" + data[0],
                                    data[2],
                                    data[3],
                                    qsTr("Kill the given amount of ") + Global[data[1]].getName() + qsTr(". Only battle kills count."),
                                    data[1],
                                    false);
        }
    };
    this.killed = function(id)
    {
        id = id.replace("DESERT_", "");
        id = id.replace("SNOW_", "");
        id = id.replace("_S", "");
        id = id.replace("_E", "");
        id = id.replace("_N", "");
        id = id.replace("_W", "");
        id = id.replace("_E_W", "");
        id = id.replace("_N_S", "");
        userdata.increaseAchievement("KILL_NEUTRAL_BUILDING_" + id, 1);
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_KILL_NEUTRAL_BUILDING = new Constructor();
