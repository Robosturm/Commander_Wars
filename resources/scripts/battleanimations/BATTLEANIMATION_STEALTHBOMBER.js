var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, alive = true)
    {
        sprite.loadMovingSpriteV2("stealthbomber+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 60), movement, moveTime);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 800);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                sprite.loadSingleMovingSprite("rocket", false, Qt.point(45, 62),
                                              Qt.point(128, 0), 400, false,
                                              1, 1, -1, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("rocket_down", false, Qt.point(45, 82),
                                              Qt.point(128, -64), 400, false,
                                              1, 1, -1, i * 150);
            }
            sprite.loadSound("stealthfighter_fire.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + 150 * sprite.getUnitCount(5);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
		var count = sprite.getUnitCount(5);

        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, 5, Qt.point(127, 60),
                                    Qt.point(-128, 0), 400, true,
                                    1, 1, 0, 0, true);
            sprite.loadSprite("rocket_hit_air",  false, 5, Qt.point(0, 60),
                              1, 1.0, 0, 300);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    1, 1, 0, 0, true);
            sprite.loadSprite("rocket_hit",  false, 5, Qt.point(0, 60),
                              1, 1.0, 0, 300);
        }
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
            return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
        }
        else
        {
            return 1100 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
        }
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2000;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_STEALTHBOMBER = new Constructor();
