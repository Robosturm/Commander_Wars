var Constructor = function()
{
    this.damageTable = [
                        ["DUSTER", 105],
                        ["FIGHTER", 105],
                        ["BOMBER", 105],
                        ["BLACK_BOMB", 105],
                        ["STEALTHBOMBER", 105],
                        ["TRANSPORTPLANE", 105],
                        ["WATERPLANE", 105],
                        ["K_HELI", 120],
                        ["T_HELI", 120],

                        ["HOVERFLAK", 65],
                        ["HOVERCRAFT", 35],
                        ["HEAVY_HOVERCRAFT", 20],
                        ["ARTILLERYCRAFT", 60],

                        ["BATTLESHIP", 5],
                        ["CANNONBOAT", 13],
                        ["CRUISER", 8],
                        ["DESTROYER", 3],
                        ["LANDER", 8],
                        ["BLACK_BOAT", 10],
                        ["AIRCRAFTCARRIER", 5]];

    this.getName = function()
    {
        return qsTr("Anti Air Gun");
    };
    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_A_AIR_GUN.damageTable, "WEAPON_A_AIR_GUN");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_AIR_GUN = new Constructor();
