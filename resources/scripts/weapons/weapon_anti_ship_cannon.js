var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.damageTable = [["BATTLESHIP", 95],
            ["CANNONBOAT", 120],				
            ["CRUISER", 75],
            ["DESTROYER", 55],
            ["SUBMARINE", 65],				
            ["LANDER", 120],
            ["BLACK_BOAT", 120],
            ["AIRCRAFTCARRIER", 120]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_CANNON.damageTable);
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CANNON = new Constructor();
