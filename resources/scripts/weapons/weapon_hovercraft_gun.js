var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.damageTable = [["APC", 75],
                        ["FLARE", 80],
                        ["RECON", 85],

                        // tanks
                        ["FLAK", 75],
                        ["HOVERFLAK", 75],
                        ["LIGHT_TANK", 55],
                        ["HOVERCRAFT", 55],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 35],
                        ["HEAVY_TANK", 25],
                        ["NEOTANK", 35],

                        // very heavy tanks
                        ["MEGATANK", 20],

                        ["HOELLIUM", 20],

                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 30],
                        ["MISSILE", 85],
                        ["ROCKETTHROWER", 85],
                        ["PIPERUNNER", 85],

                        // ships
                        ["BATTLESHIP", 10],
                        ["CANNONBOAT", 45],
                        ["CRUISER", 10],
                        ["BATTLECRUISER", 10],
                        ["SUBMARINE", 10],
                        ["LANDER", 18],
                        ["BLACK_BOAT", 45],
                        ["AIRCRAFTCARRIER", 10]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_HOVERCRAFT_GUN.damageTable, "WEAPON_HOVERCRAFT_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_HOVERCRAFT_GUN = new Constructor();
