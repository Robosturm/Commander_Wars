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

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_T_HELI.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, alive = true)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_T_HELI.armyData);
        sprite.loadMovingSpriteV2("t_heli+" + armyName + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
        sprite.loadMovingSprite("t_heli+" + armyName,  false,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime,
                          false, -1, 1.0, 0, 0, false, 50);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    }

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_T_HELI.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1, "resources/sounds/");
    };

    this.getDyingDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 2000;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_T_HELI = new Constructor();
