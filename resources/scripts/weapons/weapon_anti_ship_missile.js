var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Ship Missile");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 80],
                        ["CANNONBOAT", 80],
                        ["BLACK_BOAT", 80],
                        ["TORPEDOBOAT", 80],
                        ["LANDER", 80],
                        ["FRIGATE", 55],
                        ["DESTROYER", 55],
                        ["CRUISER", 55],
                        ["SUBMARINE", 25],
                        ["BATTLECRUISER", 25],
                        ["BATTLESHIP", 25],
                        ["AIRCRAFTCARRIER", 25],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_MISSILE.damageTable, "WEAPON_ANTI_SHIP_MISSILE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_MISSILE = new Constructor();
