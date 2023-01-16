var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 85],
                        ["CANNONBOAT", 85],
                        ["BLACK_BOAT", 85],
                        ["TORPEDOBOAT", 85],
                        ["LANDER", 40],
                        ["FRIGATE", 65],
                        ["DESTROYER", 65],
                        ["CRUISER", 55],
                        ["SUBMARINE", 95],
                        ["BATTLECRUISER", 25],
                        ["BATTLESHIP", 38],
                        ["AIRCRAFTCARRIER", 38],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable, "WEAPON_ANTI_SHIP_CRUISER_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CRUISER_MISSILE = new Constructor();
