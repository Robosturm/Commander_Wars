var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("MG");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 1;
    };
    this.damageTable = [["INFANTRY", 85],
                        ["MECH", 55],
                        ["MOTORBIKE", 55],
                        ["SNIPER", 75],

                        ["APC", 20],
                        ["FLARE", 20],
                        ["RECON", 20],

                        ["FLAK", 5],
                        ["HOVERFLAK", 5],
                        ["LIGHT_TANK", 5],
                        ["HOVERCRAFT", 5],

                        ["HEAVY_HOVERCRAFT", 3],
                        ["HEAVY_TANK", 3],
                        ["NEOTANK", 3],

                        ["MEGATANK", 1],

                        ["HOELLIUM", 20],

                        ["T_HELI", 30],
                        ["K_HELI", 10],

                        ["ARTILLERY", 15],
                        ["ARTILLERYCRAFT", 15],
                        ["ANTITANKCANNON", 45],
                        ["MISSILE", 35],
                        ["ROCKETTHROWER", 35],
                        ["PIPERUNNER", 6]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_MECH_MG.damageTable, "WEAPON_MECH_MG");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MECH_MG = new Constructor();
