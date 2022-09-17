var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Bomb");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 95;
    };
    this.damageTable = [["INFANTRY", 150],
                        ["MECH", 110],
                        ["MOTORBIKE", 110],
                        ["SNIPER", 110],

                        // supporter
                        ["APC", 105],
                        ["FLARE", 105],
                        ["RECON", 105],

                        // tanks
                        ["FLAK", 55],
                        ["HOVERFLAK", 55],
                        ["LIGHT_TANK", 105],
                        ["HOVERCRAFT", 105],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 95],
                        ["HEAVY_TANK", 80],
                        ["NEOTANK", 95],

                        // very heavy tanks
                        ["MEGATANK", 70],

                        ["HOELLIUM", 35],

                        // ranged land units
                        ["ARTILLERY", 105],
                        ["ARTILLERYCRAFT", 105],
                        ["ANTITANKCANNON", 80],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 105],
                        ["PIPERUNNER", 105],

                        // ships
                        ["BATTLECRUISER", 50],
                        ["BATTLESHIP", 85],
                        ["CRUISER", 50],
                        ["DESTROYER", 120],
                        ["SUBMARINE", 85],
                        ["LANDER", 95],
                        ["BLACK_BOAT", 120],
                        ["AIRCRAFTCARRIER", 85],
                        ["WATERMINE", 400]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_BOMB.damageTable, "WEAPON_BOMB");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BOMB = new Constructor();
