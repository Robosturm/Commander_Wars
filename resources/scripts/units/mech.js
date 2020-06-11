var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_MECH_MG");

        unit.setAmmo2(3);
        unit.setMaxAmmo2(3);
        unit.setWeapon2ID("WEAPON_BAZOOKA");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(2);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    this.getBaseCost = function()
    {
        return 2500;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        // bh and bg have the same sprites
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        // load sprites
        unit.loadSprite("mech+" + armyName, false, 1.5);
        unit.loadSpriteV2("mech+" + armyName +"+mask", GameEnums.Recoloring_Table, 1.5);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_MISSILE,ACTION_CAPTURE,ACTION_FIRE,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        var zoom = 1.5;
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        animation.loadSpriteV2("mech+" + armyName + "+walk+mask", GameEnums.Recoloring_Table, zoom);
        animation.loadSprite("mech+" + armyName + "+walk", false, zoom);
        animation.setSound("moveboots.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Mech");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("Attack power high. Can ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("capture ") + "</div>" +
               "<r>" + qsTr(" bases. ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("Vision +3 ") + "</div>" +
               "<r>" + qsTr(" when on mountains.") + "</r>";
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
}

Constructor.prototype = UNIT;
var MECH = new Constructor();
