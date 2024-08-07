var Constructor = function()
{
    this.unitTable = [  ["INFANTRY",                700,    qsTr("Go!")],
                        ["MECH",                    500,    qsTr("Brutal!")],
                        ["SNIPER",                  500,    qsTr("You shoot nothing!")],
                        ["RECON",                   300,    qsTr("Sweet!")],
                        ["FLAK",                    300,    qsTr("Cool!")],
                        ["LIGHT_TANK",              300,    qsTr("Tanks a lot!")],
                        ["HEAVY_TANK",              250,    qsTr("Boss!")],
                        ["NEOTANK",                 250,    qsTr("Neocool!")],
                        ["MEGATANK",                150,    qsTr("w00t!")],
                        ["ARTILLERY",               300,    qsTr("Oof!")],
                        ["ROCKETTHROWER",           150,    qsTr("Rockin'!")],
                        ["MISSILE",                 150,    qsTr("Aces!")],
                        ["PIPERUNNER",              150,    qsTr("Ouch!")],
                        ["APC",                     300,    qsTr("Poor APC's!")],
                        ["FIGHTER",                 150,    qsTr("Huge!")],
                        ["BOMBER",                  150,    qsTr("Fresh!")],
                        ["STEALTHBOMBER",           150,    qsTr("Good eyes!")],
                        ["BLACK_BOMB",              150,    qsTr("Sassy!")],
                        ["K_HELI",                  300,    qsTr("Great!")],
                        ["T_HELI",                  300,    qsTr("Zing!")],
                        ["BATTLESHIP",              150,    qsTr("Hoo-ha!")],
                        ["AIRCRAFTCARRIER",         150,    qsTr("No mercy!")],
                        ["SUBMARINE",               150,    qsTr("Pwn3d!")],
                        ["LANDER",                  100,    qsTr("Sink those stinkers!")],
                        ["BLACK_BOAT",              150,    qsTr("Wow!")],
                        ["ANTITANKCANNON",          250,    qsTr("We have anti anti-tank-cannons")],
                        ["ARTILLERYCRAFT",          300,    qsTr("Boom!")],
                        ["CANNONBOAT",              250,    qsTr("Sinking feeling!")],
                        ["DESTROYER",               300,    qsTr("Destroyed!")],
                        ["HOELLIUM",                150,    qsTr("Squish!")],
                        ["DUSTER",                  300,    qsTr("Into the dust!")],
                        ["HEAVY_HOVERCRAFT",        250,    qsTr("Now it can't!")],
                        ["HOVERCRAFT",              300,    qsTr("Was it a tank?")],
                        ["HOVERFLAK",               300,    qsTr("Flak is dead!")],
                        ["MOTORBIKE",               500,    qsTr("Capture them!")],
                        ["TRANSPORTPLANE",          300,    qsTr("A poor landing!")],
                        ["WATERPLANE",              100,    qsTr("Sea won!")],
                        ["ZCOUNIT_REPAIR_TANK",     150,    qsTr("Got out damaged!")],
                        ["ZCOUNIT_KIROV",           100,    qsTr("For mother Russia!")],
                        ["ZCOUNIT_NEOSPIDER_TANK",  100,    qsTr("I hate spiders!")],
                        ["ZCOUNIT_MISSILE_SUB",     100,    qsTr("Did you have a sub!")],
                        ["ZCOUNIT_CRYSTAL_TANK",    100,    qsTr("No healing for you!")],
                        ["ZCOUNIT_IRON_SHIELD_GENERATOR", 100, qsTr("You lost your armor!")],
                        ["ZCOUNIT_ROYAL_GUARD",     100,    qsTr("Was that the last guard?")],
                        ["ZCOUNIT_TANK_HUNTER",     100,    qsTr("The hunt is over!")],
                        ["ZCOUNIT_COMMANDO",        100,    qsTr("Those bastards aren't hiding from me!")],
                        ["ZCOUNIT_PARTISAN",        100,    qsTr("The Imperium won!")],
                        ["ZCOUNIT_LOGIC_TRUCK",     100,    qsTr("What a waste of money!")],
                        ["ZCOUNIT_CHAPERON",        100,    qsTr("I can still hit you!")],];

    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_KILL_UNIT.unitTable.length; ++i)
        {
            var data = ACHIEVEMENT_KILL_UNIT.unitTable[i];
            var data0 = data[0];
            if (Global[data0] !== null && typeof Global[data0] !== "undefined")
            {
                userdata.addAchievement("KILLED_UNIT_" + data0,
                                        data[1],
                                        data[2],
                                        qsTr("Kill the given amount of ") + Global[data0].getName() + qsTr(". Only battle kills count."),
                                        data0,
                                        false, qsTr("Kill unit"));
            }
        }
    };
    this.unitKilled = function(unitID)
    {
        userdata.increaseAchievement("KILLED_UNIT_" + unitID, 1);
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_KILL_UNIT = new Constructor();
