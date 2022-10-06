Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 45;
    };
    this.damageTable = [["INFANTRY", 130],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        // supporter
                        ["APC", 75],
                        ["FLARE", 75],
                        ["RECON", 75],

                        // tanks
                        ["FLAK", 65],
                        ["HOVERFLAK", 65],
                        ["LIGHT_TANK", 60],
                        ["HOVERCRAFT", 60],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 45],
                        ["HEAVY_TANK", 40],
                        ["NEOTANK", 45],

                        // very heavy tanks
                        ["MEGATANK", 30],

                        // ranged land units
                        ["ARTILLERY", 75],
                        ["ARTILLERYCRAFT", 75],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 80],
                        ["ROCKETTHROWER", 80],
                        ["PIPERUNNER", 70],

                        ["HOELLIUM", 5],
                        // ships
                        ["GUNBOAT", 100],
                        ["CANNONBOAT", 100],
                        ["BLACK_BOAT", 100],
                        ["TORPEDOBOAT", 100],
                        ["LANDER", 65],
                        ["FRIGATE", 100],
                        ["DESTROYER", 100],
                        ["CRUISER", 35],
                        ["SUBMARINE", 35],
                        ["BATTLECRUISER", 35],
                        ["BATTLESHIP", 45],
                        ["AIRCRAFTCARRIER", 45],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_CANNON.damageTable, "WEAPON_CANNON");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_CANNON = new Constructor();
