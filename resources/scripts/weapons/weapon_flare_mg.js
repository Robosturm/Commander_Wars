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
    this.damageTable = [["INFANTRY", 70],
                        ["MECH", 60],
                        ["MOTORBIKE", 60],
                        ["SNIPER", 60],

                        // supporter
                        ["APC", 45],
                        ["FLARE", 50],
                        ["RECON", 60],

                        // tanks
                        ["FLAK", 10],
                        ["HOVERFLAK", 10],
                        ["LIGHT_TANK", 10],
                        ["HOVERCRAFT", 10],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 5],
                        ["HEAVY_TANK", 5],
                        ["NEOTANK", 5],

                        // very heavy tanks
                        ["MEGATANK", 1],

                        ["HOELLIUM", 20],

                        // heli copter
                        ["T_HELI", 35],
                        ["K_HELI", 18],

                        // ranged land units
                        ["ARTILLERY", 50],
                        ["ARTILLERYCRAFT", 50],
                        ["ANTITANKCANNON", 25],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 55],
                        ["PIPERUNNER", 6]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_FLARE_MG.damageTable, "WEAPON_FLARE_MG");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FLARE_MG = new Constructor();
