var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 65;
    };
    this.damageTable = [["APC", 115],
                        ["FLARE", 115],
                        ["RECON", 115],

                        // tanks
                        ["FLAK", 115],
                        ["HOVERFLAK", 115],
                        ["LIGHT_TANK", 95],
                        ["HOVERCRAFT", 95],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 85],
                        ["HEAVY_TANK", 70],
                        ["NEOTANK", 85],

                        // very heavy tanks
                        ["MEGATANK", 55],

                        // ranged land units
                        ["ARTILLERY", 110],
                        ["ARTILLERYCRAFT", 105],
                        ["ANTITANKCANNON", 45],
                        ["MISSILE", 110],
                        ["ROCKETTHROWER", 110],
                        ["PIPERUNNER", 110],

                        ["HOELLIUM", 35],
                        // ships
                        ["GUNBOAT", 65],
                        ["CANNONBOAT", 65],
                        ["BLACK_BOAT", 65],
                        ["TORPEDOBOAT", 65],
                        ["LANDER", 28],
                        ["FRIGATE", 65],
                        ["DESTROYER", 65],
                        ["CRUISER", 14],
                        ["SUBMARINE", 14],
                        ["BATTLECRUISER", 14],
                        ["BATTLESHIP", 20],
                        ["AIRCRAFTCARRIER", 20],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_MEGATANK_GUN.damageTable, "WEAPON_MEGATANK_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_GUN = new Constructor();
