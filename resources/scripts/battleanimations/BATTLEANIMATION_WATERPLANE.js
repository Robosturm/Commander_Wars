var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("waterplane",  false,
                          BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
        sprite.addMoveTweenToLastLoadedSprites(0, -5, 1200);
        sprite.loadSpriteV2("waterplane+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
        sprite.addMoveTweenToLastLoadedSprites(0, -5, 1200);
    };
    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_WATERPLANE.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(15, 48);
        var count = sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(100, 0), 400, false,
                                    1, 1, -1);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(100, -50), 400, false,
                                    1, 1, -1);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 20),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    1, 1, 0, 0, true);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_WATERPLANE.getMaxUnitCount();
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_WATERPLANE.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_WATERPLANE = new Constructor();
