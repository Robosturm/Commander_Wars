var Constructor = function()
{
    this.unitTable = [  ["INFANTRY",                700,    qsTr("Saucy!")],
                        ["MECH",                    500,    qsTr("You da man!")],
                        ["SNIPER",                  500,    qsTr("Shoot them!")],
                        ["RECON",                   300,    qsTr("Bully for you!")],
                        ["FLAK",                    300,    qsTr("All tremble!")],
                        ["LIGHT_TANK",              300,    qsTr("Don't tread on me!")],
                        ["HEAVY_TANK",              250,    qsTr("Good gravy!")],
                        ["NEOTANK",                 250,    qsTr("You're the one!")],
                        ["MEGATANK",                150,    qsTr("Big spenda'!")],
                        ["ARTILLERY",               300,    qsTr("Right on!")],
                        ["ROCKETTHROWER",           150,    qsTr("Rainin' the hurt!")],
                        ["MISSILE",                 150,    qsTr("Massive!")],
                        ["PIPERUNNER",              150,    qsTr("Run, runner!")],
                        ["APC",                     300,    qsTr("That's a ton!")],
                        ["FIGHTER",                 150,    qsTr("Eye of the tiger!")],
                        ["BOMBER",                  150,    qsTr("Make them pay!")],
                        ["STEALTHBOMBER",           150,    qsTr("You're like an air ninja!")],
                        ["BLACK_BOMB",              150,    qsTr("You're the bomb!")],
                        ["K_HELI",                  300,    qsTr("B cool!")],
                        ["T_HELI",                  300,    qsTr("T-riffic!")],
                        ["BATTLESHIP",              150,    qsTr("Ahoy!")],
                        ["AIRCRAFTCARRIER",         150,    qsTr("Carry on!")],
                        ["SUBMARINE",               150,    qsTr("Time for a snak!")],
                        ["LANDER",                  100,    qsTr("Punish everyone!")],
                        ["BLACK_BOAT",              150,    qsTr("Impressive!")],
                        ["ANTITANKCANNON",          250,    qsTr("Where's your tank now?")],
                        ["ARTILLERYCRAFT",          300,    qsTr("Floating cannon!")],
                        ["CANNONBOAT",              250,    qsTr("Prepare for mines!")],
                        ["DESTROYER",               300,    qsTr("I have a ship!")],
                        ["HOELLIUM",                150,    qsTr("Blobs of Blobs!")],
                        ["DUSTER",                  300,    qsTr("This is a plan!")],
                        ["HEAVY_HOVERCRAFT",        250,    qsTr("This can cross the sea?")],
                        ["HOVERCRAFT",              300,    qsTr("Floating enemies ahead!")],
                        ["HOVERFLAK",               300,    qsTr("It's a flak!")],
                        ["MOTORBIKE",               500,    qsTr("Capture them!")],
                        ["WATERPLANE",              100,    qsTr("Air over Sea!")],
                        ["TRANSPORTPLANE",          300,    qsTr("I carry this!")],
                        ["ZCOUNIT_REPAIR_TANK",     150,    qsTr("I can fix this!")],
                        ["ZCOUNIT_KIROV",           100,    qsTr("Kirov reporting!")],
                        ["ZCOUNIT_NEOSPIDER_TANK",  100,    qsTr("A spider is here!")],
                        ["ZCOUNIT_MISSILE_SUB",     100,    qsTr("Rockets from below!")],
                        ["ZCOUNIT_CRYSTAL_TANK",    100,    qsTr("A healing rank!")],
                        ["ZCOUNIT_IRON_SHIELD_GENERATOR", 100, qsTr("Better armor for me!")],
                        ["ZCOUNIT_ROYAL_GUARD",     100,    qsTr("Protect the king!")],
                        ["ZCOUNIT_TANK_HUNTER",     100,    qsTr("The hunt begins!")],
                        ["ZCOUNIT_COMMANDO",        100,    qsTr("Onto the mountains!")],
                        ["ZCOUNIT_PARTISAN",        100,    qsTr("For the rebellion!")],
                        ["ZCOUNIT_LOGIC_TRUCK",     100,    qsTr("Money is all i need!")],
                        ["ZCOUNIT_CHAPERON",        100,    qsTr("No missile zone!")],];



    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_BUILD_UNIT.unitTable.length; ++i)
        {
            var data = ACHIEVEMENT_BUILD_UNIT.unitTable[i];
            userdata.addAchievement("BUILD_UNIT_" + data[0],
                                    data[1],
                                    data[2],
                                    qsTr("Build a given amount of ") + Global[data[0]].getName(),
                                    data[0],
                                    false);
        }
    };
    this.unitProduced = function(unitID)
    {
        userdata.increaseAchievement("BUILD_UNIT_" + unitID, 1);
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_BUILD_UNIT = new Constructor();
