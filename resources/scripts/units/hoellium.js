var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(1);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(1);
        unit.setIgnoreUnitCollision(true);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("hoellium" , false);
        unit.loadSpriteV2("hoellium+mask", GameEnums.Recoloring_Table);
    };
    this.getMovementType = function()
    {
        return "MOVE_HOELLIUM";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_HOELLIUM_WAIT";
    };
    this.getBaseCost = function()
    {
        return 10000;
    };
    this.getName = function()
    {
        return qsTr("Oozium");
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("hoellium_move.wav", -2);
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("Can move over all ground terrains and enemy units. Immediatly destroys units it moves over.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y, 200);
        animation.addSpriteAnimTable("hoellium_die+mask", 0, 0, unit.getOwner(), 0, 1.5, 1.5);
        animation.setSound("hoellium_explode.wav");
        return animation;
    };
}

Constructor.prototype = UNIT;
var HOELLIUM = new Constructor();
