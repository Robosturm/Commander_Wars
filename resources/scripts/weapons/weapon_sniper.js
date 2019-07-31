var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Sniper Rifle");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 1;
    };
    this.damageTable = [["INFANTRY", 170],
                        ["MECH", 150],
                        ["MOTORBIKE", 150],
                        ["SNIPER", 150],

                        // supporter
                        ["APC", 35],
                        ["FLARE", 35],
                        ["RECON", 35],

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
                        ["ARTILLERY", 8],
                        ["ANTITANKCANNON", 8],
                        ["MISSILE", 12],
                        ["ROCKETTHROWER", 12],
                        ["PIPERUNNER", 5]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_SNIPER.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_SNIPER = new Constructor();
