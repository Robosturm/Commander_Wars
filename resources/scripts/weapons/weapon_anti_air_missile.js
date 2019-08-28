var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Air Missile");
    };
    this.damageTable = [["DUSTER", 150],
                        ["FIGHTER", 150],
                        ["BOMBER", 150],
                        ["STEALTHBOMBER", 150],
                        ["TRANSPORTPLANE", 150],
                        ["WATERPLANE", 150],
                        ["BLACK_BOMB", 150],
                        ["K_HELI", 170],
                        ["T_HELI", 170]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_AIR_MISSILE.damageTable, "WEAPON_ANTI_AIR_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_AIR_MISSILE = new Constructor();
