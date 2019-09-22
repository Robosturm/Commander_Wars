var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.damageTable = [["BATTLESHIP", 40],
                        ["CANNONBOAT", 75],
                        ["CRUISER", 40],
                        ["DESTROYER", 40],
                        ["SUBMARINE", 40],
                        ["LANDER", 55],
                        ["BLACK_BOAT", 75],
                        ["AIRCRAFTCARRIER", 40],
                        ["WATERMINE", 400]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_MISSILE.damageTable, "WEAPON_ANTI_SHIP_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_MISSILE = new Constructor();
