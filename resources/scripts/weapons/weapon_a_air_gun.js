var Constructor = function()
{
    this.damageTable = [["DUSTER", 105],
                        ["FIGHTER", 105],
                        ["BOMBER", 105],
                        ["BLACK_BOMB", 105],
                        ["STEALTHBOMBER", 105],
                        ["TRANSPORTPLANE", 105],
                        ["WATERPLANE", 105],
                        ["K_HELI", 120],
                        ["T_HELI", 120]];

    this.getName = function()
    {
        return qsTr("Anti Air Gun");
    };
    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_A_AIR_GUN.damageTable, "WEAPON_A_AIR_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_AIR_GUN = new Constructor();
