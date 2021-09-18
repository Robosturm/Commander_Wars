var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSpriteV2("waterplane+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20), -1, 1.0, 0, 0,
                            false, false, 30);
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
            sprite.loadSound("missile_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());

        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadSprite("rocket_hit_air",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 20),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadSprite("rocket_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    1, 1, 0, 0, true);
        }
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        }
        else
        {
            return 1100 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_WATERPLANE = new Constructor();
