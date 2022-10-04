var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Air Gun");
    };
    this.damageTable = [["DUSTER", 55],
                        ["FIGHTER", 45],
                        ["BOMBER", 65],
                        ["STEALTHBOMBER", 65],
                        ["TRANSPORTPLANE", 75],
                        ["BLACK_BOMB", 75],
                        ["WATERPLANE", 45],
                        ["K_HELI", 75],
                        ["T_HELI", 75]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_BATTLECRUISER_A_AIR_GUN.damageTable, "WEAPON_BATTLECRUISER_A_AIR_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BATTLECRUISER_A_AIR_GUN = new Constructor();
