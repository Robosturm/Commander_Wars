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
                        ["HEAVY_TANK", 45],
                        ["NEOTANK", 45],

                        // very heavy tanks
                        ["MEGATANK", 35],

                        // ranged land units
                        ["ARTILLERY", 75],
                        ["ARTILLERYCRAFT", 75],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 80],
                        ["ROCKETTHROWER", 80],
                        ["PIPERUNNER", 70],

                        ["HOELLIUM", 5],

                        // ships
                        ["BATTLESHIP", 45],
                        ["CANNONBOAT", 100],
                        ["CRUISER", 35],
                        ["DESTROYER", 35],
                        ["SUBMARINE", 35],
                        ["LANDER", 65],
                        ["BLACK_BOAT", 100],
                        ["AIRCRAFTCARRIER", 45]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_CANNON.damageTable);
    };
};

Constructor.prototype = WEAPON;
WEAPON_CANNON = new Constructor();
