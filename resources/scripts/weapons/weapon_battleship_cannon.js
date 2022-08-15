var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };
    this.damageTable = [["INFANTRY", 125],
                        ["MECH", 75],
                        ["MOTORBIKE", 75],
                        ["SNIPER", 75],
                        // supporter
                        ["APC", 75],
                        ["FLARE", 75],
                        ["RECON", 75],
                        // tanks
                        ["FLAK", 65],
                        ["HOVERFLAK", 65],
                        ["LIGHT_TANK", 65],
                        ["HOVERCRAFT", 65],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 50],
                        ["HEAVY_TANK", 40],
                        ["NEOTANK", 50],
                        // very heavy tanks
                        ["MEGATANK", 30],
                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 75],
                        ["ROCKETTHROWER", 75],
                        ["PIPERUNNER", 80],
                        ["HOELLIUM", 20],
                        // ships
                        ["BATTLESHIP", 45],
                        ["CANNONBOAT", 95],
                        ["CRUISER", 65],
                        ["DESTROYER", 55],
                        ["SUBMARINE", 65],
                        ["LANDER", 75],
                        ["BLACK_BOAT", 95],
                        ["AIRCRAFTCARRIER", 50]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_BATTLESHIP_CANNON.damageTable, "WEAPON_BATTLESHIP_CANNON");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BATTLESHIP_CANNON = new Constructor();
