BLACK_BOAT.init = function(unit)
{
    unit.setAmmo1(1);
    unit.setMaxAmmo1(1);
    unit.setWeapon1ID("WEAPON_ANTI_SHIP_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(7);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
CANNONBOAT.getLoadingPlace = function()
{
    return 1;
};
CANNONBOAT.transportList = ["INFANTRY", "MECH", "SNIPER"];
CANNONBOAT.getBaseCost = function()
{
    return 6000;
};
CANNONBOAT.getDescription = function()
{
    return "<r>" + qsTr("Naval unit can attack Naval units. Can ") + "</r>" +
            "<div c='#00ff00'>" + qsTr("transport X 2") + "</div>" +
            "<r>" + qsTr("infantry units.") + "</r>";
};
CANNONBOAT.actionList = ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
