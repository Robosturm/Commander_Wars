var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];

    this.animationData = [["os", [Qt.point(-33, 20)]],
                          ["bm", [Qt.point(-33, 20)]],
                          ["ge", [Qt.point(-33, 20)]],
                          ["yc", [Qt.point(-33, 20)]],
                          ["bh", [Qt.point(-33, 20)]],
                          ["bg", [Qt.point(-33, 20)]],
                          ["ma", [Qt.point(-13, 20)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LANDER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_LANDER.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_LANDER.animationData);
        sprite.loadMovingSprite("lander+" + armyName,  false,
                                BATTLEANIMATION_LANDER.getMaxUnitCount(), data[0], movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("lander+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_LANDER.getMaxUnitCount(), data[0], movement, moveTime, false, -1);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LANDER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LANDER = new Constructor();
