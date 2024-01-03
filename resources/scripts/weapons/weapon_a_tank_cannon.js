var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Tank Cannon");
    };

    this.getEnviromentDamage = function(enviromentId)
    {
        return 55;
    };

    this.damageTable = [["INFANTRY", 65],
                        ["MECH", 65],
                        ["MOTORBIKE", 65],
                        ["SNIPER", 65],
                        ["APC", 65],
                        ["FLARE", 75],
                        ["RECON", 75],
                        ["FLAK", 75],
                        ["HOVERFLAK", 75],
                        ["LIGHT_TANK", 75],
                        ["HOVERCRAFT", 75],
                        ["HEAVY_HOVERCRAFT", 65],
                        ["HEAVY_TANK", 45],
                        ["NEOTANK", 65],
                        ["MEGATANK", 45],
                        ["HOELLIUM", 15],
                        ["T_HELI", 55],
                        ["K_HELI", 45],
                        ["ARTILLERY", 65],
                        ["ARTILLERYCRAFT", 65],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 70],
                        ["ROCKETTHROWER", 70],
                        ["PIPERUNNER", 55]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_A_TANK_CANNON.damageTable, "WEAPON_A_TANK_CANNON");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_TANK_CANNON = new Constructor();
