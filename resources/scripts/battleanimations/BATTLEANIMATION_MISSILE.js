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
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, startFrame = 0, endFrame = 0)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var offset = Qt.point(-10, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-35, 5);
        }
        sprite.loadSpriteV2("missile+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset,
                            1, 1, 0, 0, false, false, 200, endFrame, startFrame);
        BATTLEANIMATION.loadSpotter(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fire", 0, 1);
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        var offset = Qt.point(11, 35);
        if (armyName === "yc")
        {
            offset = Qt.point(11, 35);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(11, 35);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(5, 40);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(11, 35);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(11, 35);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                -1, 1, -1);
        offset.x -= 35;
        offset.y -= 22;
        sprite.loadMovingSprite("rocket_launch", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(0, 0), 0, false, 1, 1, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fire", 1, 1);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        sprite.loadSprite("rocket_hit_air",  false, sprite.getMaxUnitCount(), Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                -1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MISSILE.getMaxUnitCount();
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MISSILE.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MISSILE = new Constructor();
