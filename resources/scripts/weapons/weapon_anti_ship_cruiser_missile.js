var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.damageTable = [["BATTLESHIP", 38],
                        ["CANNONBOAT", 85],
                        ["CRUISER", 28],
                        ["DESTROYER", 28],
                        ["SUBMARINE", 95],
                        ["LANDER", 40],
                        ["BLACK_BOAT", 85],
                        ["AIRCRAFTCARRIER", 38]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
