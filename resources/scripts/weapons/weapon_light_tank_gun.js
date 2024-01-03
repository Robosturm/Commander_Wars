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
                        ["HEAVY_TANK", 15],
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
                        ["GUNBOAT", 45],
                        ["CANNONBOAT", 45],
                        ["BLACK_BOAT", 45],
                        ["TORPEDOBOAT", 45],
                        ["LANDER", 18],
                        ["FRIGATE", 10],
                        ["DESTROYER", 10],
                        ["CRUISER", 10],
                        ["SUBMARINE", 10],
                        ["BATTLECRUISER", 10],
                        ["BATTLESHIP", 10],
                        ["AIRCRAFTCARRIER", 10],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_LIGHT_TANK_GUN.damageTable, "WEAPON_LIGHT_TANK_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_LIGHT_TANK_GUN = new Constructor();
