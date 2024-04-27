var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(1);
        unit.setMaxAmmo1(1);
        unit.setWeapon1ID("");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(1);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("apc+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.actionList = ["ACTION_BUILD_TEMP_HARBOUR", "ACTION_BUILD_TEMP_AIRPORT", "ACTION_LOAD",
                       "ACTION_UNLOAD", "ACTION_JOIN", "ACTION_SUPPORTALL_RATION", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.getBaseCost = function()
    {
        return 5000;
    };
    this.getName = function()
    {
        return qsTr("APC");
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 1;
    };
    this.transportList = ["INFANTRY", "MECH", "SNIPER", "ZCOUNIT_COMMANDO", "ZCOUNIT_PARTISAN", "ZCOUNIT_RANGER"];
    this.startOfTurn = function(unit, map)
    {
        // pay unit upkeep
        if (unit.getTerrain() !== null)
        {
            ACTION_SUPPORTALL_RATION.giveRation(unit, map);
        }
        UNIT.transporterRefilling(unit, map);
    };

    this.getDescription = function()
    {
        return qsTr("<div c='#00ff00'>Carries X 1</div><r> infantry unit, </r><div c='#00ff00'> supplies </div><r> adjacent units and </r><div c='#00ff00'>builds </div><r> Airports and Ports.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getEditorPlacementSound = function()
    {
        return "movetank.wav";
    };
}

Constructor.prototype = UNIT;
var APC = new Constructor();
