var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.damageTable = [["APC", 100],
                        ["FLARE", 100],
                        ["RECON", 105],

                        // tanks
                        ["FLAK", 100],
                        ["HOVERFLAK", 100],
                        ["LIGHT_TANK", 80],
                        ["HOVERCRAFT", 80],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 55],
                        ["HEAVY_TANK", 35],
                        ["NEOTANK", 55],

                        // very heavy tanks
                        ["MEGATANK", 30],

                        ["HOELLIUM", 20],

                        // ranged land units
                        ["ARTILLERY", 95],
                        ["ARTILLERYCRAFT", 95],
                        ["ANTITANKCANNON", 45],
                        ["MISSILE", 100],
                        ["ROCKETTHROWER", 100],
                        ["PIPERUNNER", 100],
                        // ships
                        ["GUNBOAT", 55],
                        ["CANNONBOAT", 55],
                        ["BLACK_BOAT", 55],
                        ["TORPEDOBOAT", 55],
                        ["LANDER", 22],
                        ["FRIGATE", 12],
                        ["DESTROYER", 12],
                        ["CRUISER", 12],
                        ["SUBMARINE", 12],
                        ["BATTLECRUISER", 12],
                        ["BATTLESHIP", 10],
                        ["AIRCRAFTCARRIER", 10],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_NEOTANK_GUN.damageTable, "WEAPON_NEOTANK_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_NEOTANK_GUN = new Constructor();
