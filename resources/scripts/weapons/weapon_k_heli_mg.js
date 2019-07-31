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
    this.damageTable = [["INFANTRY", 115],
                        ["MECH", 75],
                        ["MOTORBIKE", 75],
                        ["SNIPER", 75],

                        // supporter
                        ["APC", 20],
                        ["FLARE", 30],
                        ["RECON", 30],

                        // tanks
                        ["FLAK", 1],
                        ["HOVERFLAK", 1],
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
                        ["T_HELI", 85],
                        ["K_HELI", 65],

                        // ranged land units
                        ["ARTILLERY", 25],
                        ["ANTITANKCANNON", 1],
                        ["MISSILE", 25],
                        ["ROCKETTHROWER", 35],
                        ["PIPERUNNER", 6]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_K_HELI_MG.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_K_HELI_MG = new Constructor();
