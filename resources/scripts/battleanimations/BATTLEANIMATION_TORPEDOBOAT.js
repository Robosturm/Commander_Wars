var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_TORPEDOBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-30, 0), 580, Qt.point(20, 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_TORPEDOBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-10, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, startPos)
    {
        sprite.loadMovingSprite("torpedoboat",  false,
                                BATTLEANIMATION_TORPEDOBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("torpedoboat+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_TORPEDOBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_TORPEDOBOAT.loadStandingAnimation(sprite, unit, defender, weapon, Qt.point(-38, 20));
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (globals.isEven(i))
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(70, 10),
                                              Qt.point(60, 0), 400, false,
                                              1, 1, 2, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(90, 50),
                                              Qt.point(60, 0), 400, false,
                                              1, 0.5, -1, i * 150);
            }
            sprite.loadSound("torpedo_fire.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 860;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        for (var i = 0; i < count; i++)
        {
            if (globals.isEven(i))
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 20),
                                              Qt.point(-70, 0), 400, true,
                                              1, 1, 2, i * 150, true);

                sprite.loadSingleMovingSprite("water_hit",  false, Qt.point(45, 20),
                                              Qt.point(0, 0), 0, false,
                                              1, 1.0, 2, 400 + i * 150, true,
                                              100, -1, 0, 0, 180);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 50),
                                              Qt.point(-60, 0), 400, true,
                                              1, 0.5, -1, i * 150);
                sprite.loadSingleMovingSprite("water_hit",  false, Qt.point(57, 50),
                                              Qt.point(0, 0), 0, false,
                                              1, 0.5, -1, 400 + i * 150, true,
                                              100, -1, 0, 0, 180);
            }
            sprite.loadSound("torpedo_move.wav", 1);
            sprite.loadSound("impact_explosion.wav", 1, 300 + i * 150);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_TORPEDOBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, Qt.point(-38, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_TORPEDOBOAT = new Constructor();
