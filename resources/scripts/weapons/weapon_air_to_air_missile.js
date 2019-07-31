var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.damageTable = [["DUSTER", 80],
                        ["FIGHTER", 55],
                        ["BOMBER", 65],
                        ["STEALTHBOMBER", 65],
                        ["TRANSPORTPLANE", 120],
                        ["BLACK_BOMB", 120],
                        ["WATERPLANE", 65],
                        ["K_HELI", 120],
                        ["T_HELI", 120]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_A_TANK_CANNON.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIR_TO_AIR_MISSILE = new Constructor();
