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

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-35, 5);
        }
        sprite.loadSprite("missile+" + armyName + ending,  false,
                          BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("missile+" + armyName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fired");
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var offset = Qt.point(5, 30);
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        // rocket
        if (armyName === "yc")
        {
            offset = Qt.point(7, 30);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(8, 30);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(5, 40);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(8, 30);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(8, 28);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                1, 1, -1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fired");
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
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
