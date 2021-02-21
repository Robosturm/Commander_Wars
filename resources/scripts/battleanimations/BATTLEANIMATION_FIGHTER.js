var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
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
		var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FIGHTER.armyData);
        var offset = Qt.point(-40, 20);
        if (armyName === "ma")
        {
            offset = Qt.point(-50, 20);
        }
        sprite.loadSprite("fighter+" + armyName,  false,
                          BATTLEANIMATION_FIGHTER.getMaxUnitCount(), offset);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        sprite.loadSpriteV2("fighter+" + armyName + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_FIGHTER.getMaxUnitCount(), offset);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FIGHTER.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FIGHTER.armyData);
        var offset = Qt.point(-5, 37);
        // gun
        if (armyName === "yc")
        {
            offset = Qt.point(4, 34);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(3, 36);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(0, 33);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(0, 42);
        }
        sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(127, 0), 400, false,
                                1, 1, -1);
        sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 20),
                                Qt.point(-127, 0), 400, true,
                                1, 1, 0, 0, true);
        sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_FIGHTER.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FIGHTER = new Constructor();
