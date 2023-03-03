var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_INFANTRY_MG");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
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
                     ["dm", "dm"],
                     ["gs", "gs"],
                     ["pf", "pf"],
                     ["ti", "ti"],];

    this.animationData = [["os", [1]],
                          ["bm", [1]],
                          ["ge", [1]],
                          ["yc", [1]],
                          ["bh", [1]],
                          ["bg", [1]],
                          ["ma", [2]],
                          ["ac", [2]],
                          ["bd", [2]],
                          ["dm", [2]],
                          ["gs", [2]],
                          ["pf", [2]],
                          ["ti", [2]],];

    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, INFANTRY.armyData);
        // load sprites
        unit.loadSpriteV2("infantry+" + armyName +"+mask", GameEnums.Recoloring_Matrix);
        unit.loadSpriteV2("infantry+" + armyName, GameEnums.Recoloring_None);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];

    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, INFANTRY.armyData);
        var data = Global.getDataFromTable(armyName, INFANTRY.animationData);
        animation.loadSpriteV2("infantry+" + armyName + "+walk+mask", GameEnums.Recoloring_Matrix, data[0]);
        animation.setSound("movewalk.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 1000;
    };
    this.getName = function()
    {
        return qsTr("Infantry");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Cheapest unit. Can </r><div c='#00ff00'>capture </div><r>bases. </r><div c='#00ff00'>Vision +3 </div><r>when on mountains.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getEditorPlacementSound = function()
    {
        return "movewalk.wav";
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
