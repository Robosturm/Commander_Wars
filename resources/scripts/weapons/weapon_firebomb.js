var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Firebomb");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.damageTable = [["INFANTRY", 105],
                        ["MECH", 85],
                        ["MOTORBIKE", 85],
                        ["SNIPER", 85],

                        ["APC", 75],
                        ["FLARE", 80],
                        ["RECON", 85],

                        // tanks
                        ["FLAK", 75],
                        ["HOVERFLAK", 75],
                        ["LIGHT_TANK", 55],
                        ["HOVERCRAFT", 55],

                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 35],
                        ["HEAVY_TANK", 35],
                        ["NEOTANK", 35],

                        // very heavy tanks
                        ["MEGATANK", 20],

                        ["HOELLIUM", 20],

                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 30],
                        ["MISSILE", 85],
                        ["ROCKETTHROWER", 85],
                        ["PIPERUNNER", 85],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_FIREBOMB.damageTable, "WEAPON_FIREBOMB");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FIREBOMB = new Constructor();
