var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Bazooka");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return 15;
    };
    this.damageTable = [["APC", 85],
                        ["FLARE", 85],
                        ["RECON", 75],
                        // tanks
                        ["FLAK", 55],
                        ["HOVERFLAK", 55],
                        ["LIGHT_TANK", 55],
                        ["HOVERCRAFT", 55],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 25],
                        ["HEAVY_TANK", 15],
                        ["NEOTANK", 25],
                        // very heavy tanks
                        ["MEGATANK", 15],
                        ["HOELLIUM", 30],
                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 85],
                        ["ROCKETTHROWER", 85],
                        ["PIPERUNNER", 55]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_BAZOOKA.damageTable, "WEAPON_BAZOOKA");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BAZOOKA = new Constructor();
