var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.damageTable = [["DUSTER", 80],
                        ["FIGHTER", 55],
                        ["BOMBER", 100],
                        ["STEALTHBOMBER", 85],
                        ["TRANSPORTPLANE", 120],
                        ["BLACK_BOMB", 120],
                        ["WATERPLANE", 85],
                        ["K_HELI", 120],
                        ["T_HELI", 120]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_AIR_TO_AIR_MISSILE.damageTable, "WEAPON_AIR_TO_AIR_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIR_TO_AIR_MISSILE = new Constructor();
