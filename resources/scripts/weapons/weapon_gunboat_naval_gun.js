var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Naval gun");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 10;
    };
    this.damageTable = [["GUNBOAT", 55],
                        ["CANNONBOAT", 55],
                        ["BLACK_BOAT", 55],
                        ["TORPEDOBOAT", 55],
                        ["LANDER", 35],
                        ["FRIGATE", 35],
                        ["DESTROYER", 35],
                        ["CRUISER", 35],
                        ["SUBMARINE", 5],
                        ["BATTLECRUISER", 5],
                        ["BATTLESHIP", 5],
                        ["AIRCRAFTCARRIER", 5],
                        // infantry
                        ["INFANTRY", 55],
                        ["MECH", 45],
                        ["MOTORBIKE", 45],
                        ["SNIPER", 45],
                        // supporter
                        ["APC", 35],
                        ["FLARE", 35],
                        ["RECON", 35],
                        // tanks
                        ["FLAK", 25],
                        ["HOVERFLAK", 25],
                        ["LIGHT_TANK", 25],
                        ["HOVERCRAFT", 25],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 5],
                        ["HEAVY_TANK", 5],
                        ["NEOTANK", 5],
                        // very heavy tanks
                        ["MEGATANK", 1],
                        // ranged land units
                        ["ARTILLERY", 25],
                        ["ARTILLERYCRAFT", 25],
                        ["ANTITANKCANNON", 25],
                        ["MISSILE", 35],
                        ["ROCKETTHROWER", 35],
                        ["PIPERUNNER", 35],
                        ["HOELLIUM", 20]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_GUNBOAT_NAVAL_GUN.damageTable, "WEAPON_GUNBOAT_NAVAL_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_GUNBOAT_NAVAL_GUN = new Constructor();
