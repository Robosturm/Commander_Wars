var Constructor = function()
{
    this.UnitTabel = [  ["INFANTRY",      700,    qsTr("Saucy!")],
                        ["MECH",          500,    qsTr("You da man!")],
                        ["SNIPER",        500,    qsTr("Shoot them!")],
                        ["RECON",         300,    qsTr("Bully for you!")],
                        ["FLAK",          300,    qsTr("All tremble!")],
                        ["LIGHT_TANK",    300,    qsTr("Don't tread on me!")],
                        ["HEAVY_TANK",    250,    qsTr("Good gravy!")],
                        ["NEOTANK",       250,    qsTr("You're the one!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],

                        ["ARTILLERY",     300,    qsTr("Right on!")],
                        ["ROCKETTHROWER", 150,    qsTr("Rainin' the hurt!")],
                        ["MISSILE",       150,    qsTr("Massive!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],
                        ["MEGATANK",      150,    qsTr("Big spenda'!")],];
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_BUILD_UNIT = new Constructor();
