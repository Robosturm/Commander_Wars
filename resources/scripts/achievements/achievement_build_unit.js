var Constructor = function()
{
    this.UnitTabel = [  ["INFANTRY",      700,    tr("Saucy!")],
                        ["MECH",          500,    tr("You da man!")],
                        ["SNIPER",        500,    tr("Shoot them!")],
                        ["RECON",         300,    tr("Bully for you!")],
                        ["FLAK",          300,    tr("All tremble!")],
                        ["LIGHT_TANK",    300,    tr("Don't tread on me!")],
                        ["HEAVY_TANK",    250,    tr("Good gravy!")],
                        ["NEOTANK",       250,    tr("You're the one!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],

                        ["ARTILLERY",     300,    tr("Right on!")],
                        ["ROCKETTHROWER", 150,    tr("Rainin' the hurt!")],
                        ["MISSILE",       150,    tr("Massive!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],
                        ["MEGATANK",      150,    tr("Big spenda'!")],];
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_BUILD_UNIT = new Constructor();
