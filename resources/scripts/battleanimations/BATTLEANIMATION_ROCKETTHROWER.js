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
                     ["ma", "ma"],
                     ["ac", "ac"],
                     ["dm", "dm"],
                     ["ti", "ti"],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        var offset = Qt.point(-10, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-25, 5);
        }
        sprite.loadSprite("rocketthrower+" + armyName + ending,  false,
                          BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("rocketthrower+" + armyName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {

        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fired");
        var offset = Qt.point(0, 31);
        // rocket
        if (armyName === "yc")
        {
            offset = Qt.point(0, 32);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(0, 42);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(0, 24);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(0, 32);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(0, 33);
        }
        else if (armyName === "ac")
        {
            offset = Qt.point(5, 38);
        }
        else if (armyName === "dm")
        {
            offset = Qt.point(5, 37);
        }
        else if (armyName === "ti")
        {
            offset = Qt.point(-5, 31);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                1, 1, -1);
        sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fired");
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                Qt.point(-128, -64), 400, true,
                                1, 1, 0, 0, true);
        sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount();
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ROCKETTHROWER = new Constructor();
