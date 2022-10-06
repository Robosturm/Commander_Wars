var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 25;
    };
    this.damageTable = [["APC", 75],
                        ["FLARE", 75],
                        ["RECON", 75],
                        ["FLAK", 40],
                        ["HOVERFLAK", 40],
                        ["LIGHT_TANK", 70],
                        ["HOVERCRAFT", 70],
                        ["HEAVY_HOVERCRAFT", 45],
                        ["HEAVY_TANK", 35],
                        ["NEOTANK", 45],
                        ["MEGATANK", 25],
                        ["ARTILLERY", 65],
                        ["ARTILLERYCRAFT", 65],
                        ["ANTITANKCANNON", 35],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 75],
                        ["PIPERUNNER", 55],
                        ["HOELLIUM", 25],
                        // ships
                        ["GUNBOAT", 85],
                        ["CANNONBOAT", 85],
                        ["BLACK_BOAT", 85],
                        ["TORPEDOBOAT", 85],
                        ["LANDER", 25],
                        ["FRIGATE", 65],
                        ["DESTROYER", 65],
                        ["CRUISER", 10],
                        ["SUBMARINE", 25],
                        ["BATTLECRUISER", 10],
                        ["BATTLESHIP", 25],
                        ["AIRCRAFTCARRIER", 25]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_AIR_TO_GROUND_MISSILE.damageTable, "WEAPON_AIR_TO_GROUND_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIR_TO_GROUND_MISSILE = new Constructor();
