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
    this.damageTable = [["INFANTRY", 140],
                        ["MECH", 110],
                        ["MOTORBIKE", 110],
                        ["SNIPER", 110],

                        // supporter
                        ["APC", 45],
                        ["FLARE", 35],
                        ["RECON", 40],

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
                        ["T_HELI", 40],
                        ["K_HELI", 25],

                        // ranged land units
                        ["ARTILLERY", 45],
                        ["ARTILLERYCRAFT", 45],
                        ["ANTITANKCANNON", 1],
                        ["MISSILE", 60],
                        ["ROCKETTHROWER", 60],
                        ["PIPERUNNER", 6]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_NEOTANK_MG.damageTable, "WEAPON_NEOTANK_MG");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_NEOTANK_MG = new Constructor();
