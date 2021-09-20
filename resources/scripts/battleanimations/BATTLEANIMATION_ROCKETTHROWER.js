var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["ti", "ti"],
                     ["pf", "pf"],
                     ["yc", "yc"],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, startFrame = 0)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        var offset = Qt.point(-10, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-25, 5);
        }
        sprite.loadSpriteV2("rocketthrower+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), offset, 1, 1, 0, 0, false, false, 100, -1, startFrame);
        BATTLEANIMATION.loadSpotter(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fire");
        var offset = Qt.point(0, 31);
        var count = sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
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
        else if (armyName === "pf")
        {
            offset = Qt.point(7, 45);
        }
        else if (armyName === "bd")
        {
            offset = Qt.point(-2, 30);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                1, 1, -1);
        offset.x -= 35;
        offset.y -= 22;
        sprite.loadMovingSprite("rocket_launch", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(0, 0), 0, false, 1, 1, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
        sprite.loadSprite("rocket_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                Qt.point(-128, -64), 400, true,
                                1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1100 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ROCKETTHROWER = new Constructor();
