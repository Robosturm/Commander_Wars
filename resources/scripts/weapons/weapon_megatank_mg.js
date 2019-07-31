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
    this.damageTable = [["INFANTRY", 170],
                        ["MECH", 130],
                        ["MOTORBIKE", 130],
                        ["SNIPER", 130],

                        // supporter
                        ["APC", 50],
                        ["FLARE", 50],
                        ["RECON", 50],

                        // tanks
                        ["FLAK", 10],
                        ["HOVERFLAK", 10],
                        ["LIGHT_TANK", 10],
                        ["HOVERCRAFT", 10],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 10],
                        ["HEAVY_TANK", 10],
                        ["NEOTANK", 10],

                        // very heavy tanks
                        ["MEGATANK", 1],

                        ["HOELLIUM", 20],

                        // heli copter
                        ["T_HELI", 45],
                        ["K_HELI", 35],

                        // ranged land units
                        ["ARTILLERY", 45],
                        ["ANTITANKCANNON", 1],
                        ["MISSILE", 65],
                        ["ROCKETTHROWER", 65],
                        ["PIPERUNNER", 10]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_MEGATANK_MG.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MEGATANK_MG = new Constructor();
