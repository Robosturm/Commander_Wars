var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_A_TANK_CANNON");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(1);
        unit.setMaxRange(3);
        unit.setVision(2);

    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("antitankcannon+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_B";
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1.5);
        animation.setSound("moveheavytire.wav", -2);
        return animation;
    };

    this.getBaseCost = function()
    {
        return 11000;
    };

    this.getName = function()
    {
        return qsTr("Anti Tank Cannon");
    };

    this.getDescription = function()
    {
        return qsTr("<r>Indirect-attack unit. When under direct enemy attack it can </r><div c='#00ff00'>counter-attack</div><r>.</r>" Can target helicopters, but not Naval units.);
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveheavytire.wav";
    };
}

Constructor.prototype = UNIT;
var ANTITANKCANNON = new Constructor();
