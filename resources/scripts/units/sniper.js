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
    // called for loading the main sprite
    this.armyDataStanding = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
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
        var armyName = Global.getArmyNameFromPlayerTable(player, INFANTRY.armyDataStanding);
        // load sprites
        unit.loadSprite("sniper+" + armyName, false, 1.5);
        unit.loadSpriteV2("sniper+" + armyName +"+mask", GameEnums.Recoloring_Table, 1.5);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_MISSILE,ACTION_CAPTURE,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, SNIPER.armyData);
        var zoom = 1.5;
        animation.loadSpriteV2("sniper+" + armyName + "+walk+mask", GameEnums.Recoloring_Table, zoom);
        animation.loadSprite("sniper+" + armyName + "+walk", false, zoom);
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
}

Constructor.prototype = UNIT;
var SNIPER = new Constructor();
