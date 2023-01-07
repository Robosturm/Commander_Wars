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

        unit.setFuel(0);
        unit.setMaxFuel(0);
        unit.setBaseMovementPoints(0);
        unit.setMinRange(0);
        unit.setMaxRange(0);
        unit.setVision(1);
		
		unit.setHidden(true);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("watermine+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 1000;
    };

    this.getName = function()
    {
        return qsTr("Watermine");
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.createExplosionAnimation = function(x, y, unit, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 0;
    };
    this.getTerrainAnimationBase = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "base_" + weatherModifier + "air";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.actionList = [];
    this.getDescription = function()
    {
        return qsTr("Naval Mine. Explodes when trapping a none air unit and dealing 5 damage to the trapped unit.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveship.wav";
    };
}

Constructor.prototype = UNIT;
var WATERMINE = new Constructor();
