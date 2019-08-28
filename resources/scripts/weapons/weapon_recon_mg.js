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
    this.damageTable = [["INFANTRY", 135],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        // supporter
                        ["APC", 45],
                        ["FLARE", 45],
                        ["RECON", 45],

                        // tanks
                        ["FLAK", 8],
                        ["HOVERFLAK", 8],
                        ["LIGHT_TANK", 8],
                        ["HOVERCRAFT", 8],

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
                        ["ARTILLERY", 45],
                        ["ARTILLERYCRAFT", 45],
                        ["ANTITANKCANNON", 1],
                        ["MISSILE", 55],
                        ["ROCKETTHROWER", 55],
                        ["PIPERUNNER", 6]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_RECON_MG.damageTable, "WEAPON_RECON_MG");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_RECON_MG = new Constructor();
