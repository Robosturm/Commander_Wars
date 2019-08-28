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
    this.damageTable = [["APC", 105],
                        ["FLARE", 105],
                        ["RECON", 105],

                        // tanks
                        ["FLAK", 105],
                        ["HOVERFLAK", 105],
                        ["LIGHT_TANK", 85],
                        ["HOVERCRAFT", 85],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 75],
                        ["HEAVY_TANK", 75],
                        ["NEOTANK", 75],

                        // very heavy tanks
                        ["MEGATANK", 55],

                        // ranged land units
                        ["ARTILLERY", 105],
                        ["ARTILLERYCRAFT", 105],
                        ["ANTITANKCANNON", 40],
                        ["MISSILE", 105],
                        ["ROCKETTHROWER", 105],
                        ["PIPERUNNER", 105],

                        ["HOELLIUM", 35],

                        // ships
                        ["BATTLESHIP", 20],
                        ["CANNONBOAT", 65],
                        ["CRUISER", 14],
                        ["DESTROYER", 14],
                        ["SUBMARINE", 14],
                        ["LANDER", 28],
                        ["BLACK_BOAT", 65],
                        ["AIRCRAFTCARRIER", 20]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_MEGATANK_GUN.damageTable, "WEAPON_MEGATANK_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_GUN = new Constructor();
