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
    this.damageTable = [["INFANTRY", 55],
                        ["MECH", 45],
                        ["MOTORBIKE", 45],
                        ["SNIPER", 55],

                        // supporter
                        ["APC", 14],
                        ["FLARE", 10],
                        ["RECON", 12],

                        // tanks
                        ["FLAK", 5],
                        ["HOVERFLAK", 5],
                        ["LIGHT_TANK", 5],
                        ["HOVERCRAFT", 5],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 3],
                        ["HEAVY_TANK", 3],
                        ["NEOTANK", 3],

                        // very heavy tanks
                        ["MEGATANK", 1],

                        ["HOELLIUM", 20],

                        // heli copter
                        ["T_HELI", 30],
                        ["K_HELI", 8],

                        // ranged land units
                        ["ARTILLERY", 10],
                        ["ANTITANKCANNON", 45],
                        ["MISSILE", 20],
                        ["ROCKETTHROWER", 20],
                        ["PIPERUNNER", 5]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_INFANTRY_MG.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_INFANTRY_MG = new Constructor();
