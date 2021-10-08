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
    this.armyDataStanding = [["os", "os"],
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
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MECH.armyDataStanding);
        // load sprites
        unit.loadSpriteV2("mech+" + armyName +"+mask", GameEnums.Recoloring_Matrix);
        unit.loadSpriteV2("mech+" + armyName, GameEnums.Recoloring_None);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["ma", "ma"],
                     ["ac", "ac"],];
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MECH.armyData);
        animation.loadSpriteV2("mech+" + armyName + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
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
        return qsTr("<r>Attack power high. Can </r><div c='#00ff00'>capture </div><r> bases. </r><div c='#00ff00'>Vision +3 </div><r> when on mountains.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
}

Constructor.prototype = UNIT;
var MECH = new Constructor();
