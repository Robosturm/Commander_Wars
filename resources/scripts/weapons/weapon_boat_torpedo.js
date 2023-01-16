var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Torpedo");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 30],
                        ["CANNONBOAT", 30],
                        ["BLACK_BOAT", 30],
                        ["TORPEDOBOAT", 30],
                        ["LANDER", 75],
                        ["FRIGATE", 75],
                        ["DESTROYER", 75],
                        ["CRUISER", 75],
                        ["SUBMARINE", 35],
                        ["BATTLECRUISER", 35],
                        ["BATTLESHIP", 35],
                        ["AIRCRAFTCARRIER", 35],];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_BOAT_TORPEDO.damageTable, "WEAPON_BOAT_TORPEDO");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_BOAT_TORPEDO = new Constructor();
