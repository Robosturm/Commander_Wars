var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon1ID("WEAPON_BAZOOKA");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_MECH_MG");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(2);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    this.getBaseCost = function()
    {
        return 3000;
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
                          ["bg", [2]],
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
        var armyName = Global.getArmyNameFromPlayerTable(player, MECH.armyData);
        // load sprites
        unit.loadSpriteV2("mech+" + armyName +"+mask", GameEnums.Recoloring_Matrix);
        unit.loadSpriteV2("mech+" + armyName, GameEnums.Recoloring_None);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];

    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MECH.armyData);
        var data = Global.getDataFromTable(armyName, MECH.animationData);
        animation.loadSpriteV2("mech+" + armyName + "+walk+mask", GameEnums.Recoloring_Matrix, data[0]);
        unit.loadSpriteV2("mech+" + armyName + "+walk", GameEnums.Recoloring_None);
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
        return qsTr("<r>High attack power. Can </r><div c='#00ff00'>capture </div><r> bases. </r><div c='#00ff00'>Vision +3 </div><r> when on mountains.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveboots.wav";
    };
}

Constructor.prototype = UNIT;
var MECH = new Constructor();
