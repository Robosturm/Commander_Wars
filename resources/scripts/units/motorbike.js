var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_MOTORBIKE_MG");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["ma", "ma"],
                     ["ac", "ac"],
                     ["bd", "bd"],
                     ["gs", "gs"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["dm", "dm"],];
    this.getBaseCost = function()
    {
        return 2500;
    };
    
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MOTORBIKE.armyData);
        // load sprites
        unit.loadSpriteV2("motorbike+" + armyName +"+mask", GameEnums.Recoloring_Matrix);
        unit.loadSpriteV2("motorbike+" + armyName, GameEnums.Recoloring_None);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_B";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.getName = function()
    {
        return qsTr("Motorbike");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MOTORBIKE.armyData);
        animation.loadSpriteV2("motorbike+" + armyName + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        unit.loadSpriteV2("motorbike+" + armyName + "+walk", GameEnums.Recoloring_None);
        animation.setSound("movebike.wav", -2);
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Infantry unit with high mobility. Able to </r><div c='#00ff00'>capture </div><r> bases.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getEditorPlacementSound = function()
    {
        return "movebike.wav";
    };
}

Constructor.prototype = UNIT;
var MOTORBIKE = new Constructor();
