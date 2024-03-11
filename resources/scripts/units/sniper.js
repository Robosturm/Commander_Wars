var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_SNIPER");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
        unit.setMinRange(2);
        unit.setMaxRange(3);
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

    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, SNIPER.armyData);
        // load sprites        
        unit.loadSpriteV2("sniper+" + armyName +"+mask", GameEnums.Recoloring_Matrix);
        unit.loadSpriteV2("sniper+" + armyName, GameEnums.Recoloring_None);
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
        var armyName = Global.getArmyNameFromPlayerTable(player, SNIPER.armyData);
        animation.loadSpriteV2("sniper+" + armyName + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        unit.loadSpriteV2("sniper+" + armyName + "+walk", GameEnums.Recoloring_None);
        animation.setSound("movewalk.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 2500;
    };
    this.getName = function()
    {
        return qsTr("Sniper");
    };

    this.getDescription = function()
    {
        return qsTr("<r>Indirect infantry unit exceptionly strong against other infantry units. Can </r><div c='#00ff00'>capture </div><r> bases. </><div c='#00ff00'>Vision +3 </div><r> when on mountains.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };
    this.getEditorPlacementSound = function()
    {
        return "movewalk.wav";
    };
}

Constructor.prototype = UNIT;
var SNIPER = new Constructor();
