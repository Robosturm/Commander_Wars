var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Gatling");
    };
    this.damageTable = [["DUSTER", 55],
                        ["FIGHTER", 55],
                        ["BOMBER", 55],
                        ["STEALTHBOMBER", 55],
                        ["TRANSPORTPLANE", 55],
                        ["WATERPLANE", 55],
                        ["BLACK_BOMB", 55],
                        ["K_HELI", 95],
                        ["T_HELI", 75]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_GATLING.damageTable, "WEAPON_GATLING");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_GATLING = new Constructor();
