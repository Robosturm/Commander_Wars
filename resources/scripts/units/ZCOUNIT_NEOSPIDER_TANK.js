var Constructor = function()
{	
	this.getUnitDamageID = function(unit)
    {
        return "NEOTANK";
    };
	
    this.init = function(unit)
    {
        unit.setAmmo1(4);
        unit.setMaxAmmo1(4);
        unit.setWeapon1ID("WEAPON_NEOTANK_GUN");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_NEOTANK_MG");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("neospider_tank", false);
        unit.loadSpriteV2("neospider_tank+mask", GameEnums.Recoloring_Table);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSpriteV2("neospider_tank+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("neospider_tank+walk", false, 1.5);
        animation.setSound("movewalker.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_CRAWLER";
    };
    this.getBaseCost = function()
    {
        return 13500;
    };
    this.getName = function()
    {
        return qsTr("Neo Spider Tank");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Lash's signature spider tank. Her NeoSpider Tank uses revolutionary Black Hole design to allow the tank to mantain as much weight as possbile while still having the agility to navigate mountains.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_NEOSPIDER_TANK = new Constructor();
