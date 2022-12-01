var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Naval gun");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 75],
                        ["CANNONBOAT", 75],
                        ["BLACK_BOAT", 75],
                        ["TORPEDOBOAT", 75],
                        ["LANDER", 75],
                        ["FRIGATE", 55],
                        ["DESTROYER", 55],
                        ["CRUISER", 55],
                        ["SUBMARINE", 20],
                        ["BATTLECRUISER", 20],
                        ["BATTLESHIP", 20],
                        ["AIRCRAFTCARRIER", 20],
                        // infantry
                        ["INFANTRY", 65],
                        ["MECH", 55],
                        ["MOTORBIKE", 55],
                        ["SNIPER", 55],
                        // supporter
                        ["APC", 45],
                        ["FLARE", 45],
                        ["RECON", 45],
                        // tanks
                        ["FLAK", 35],
                        ["HOVERFLAK", 35],
                        ["LIGHT_TANK", 35],
                        ["HOVERCRAFT", 35],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 20],
                        ["HEAVY_TANK", 10],
                        ["NEOTANK", 20],
                        // very heavy tanks
                        ["MEGATANK", 5],
                        // ranged land units
                        ["ARTILLERY", 40],
                        ["ARTILLERYCRAFT", 40],
                        ["ANTITANKCANNON", 25],
                        ["MISSILE", 45],
                        ["ROCKETTHROWER", 45],
                        ["PIPERUNNER", 50],
                        ["HOELLIUM", 5]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_FRIGATE_NAVAL_GUN.damageTable, "WEAPON_FRIGATE_NAVAL_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FRIGATE_NAVAL_GUN = new Constructor();
