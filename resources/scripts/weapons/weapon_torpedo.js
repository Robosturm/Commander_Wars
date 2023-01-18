var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Torpedo");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 55],
                        ["CANNONBOAT", 55],
                        ["BLACK_BOAT", 55],
                        ["TORPEDOBOAT", 55],
                        ["LANDER", 120],
                        ["FRIGATE", 120],
                        ["DESTROYER", 120],
                        ["CRUISER", 25],
                        ["SUBMARINE", 55],
                        ["BATTLECRUISER", 95],
                        ["BATTLESHIP", 80],
                        ["AIRCRAFTCARRIER", 120],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_TORPEDO.damageTable, "WEAPON_TORPEDO");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_TORPEDO = new Constructor();
