var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Air Missile");
    };
    this.damageTable = [["DUSTER", 55],
                        ["FIGHTER", 45],
                        ["BOMBER", 55],
                        ["STEALTHBOMBER", 55],
                        ["BLACK_BOMB", 75],
                        ["TRANSPORTPLANE", 75],
                        ["WATERPLANE", 55],
                        ["K_HELI", 75],
                        ["T_HELI", 85]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_CARRIER_ANTI_AIR_MISSILE.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_CARRIER_ANTI_AIR_MISSILE = new Constructor();
