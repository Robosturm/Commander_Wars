var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_CANNONBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-58, 0), 580, Qt.point(20, 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CANNONBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-38, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, startPos)
    {
        sprite.loadMovingSprite("cannonboat",  false,
                                BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cannonboat+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_CANNONBOAT.loadStandingAnimation(sprite, unit, defender, weapon, Qt.point(-38, 20));
        var offset = Qt.point(60, 65);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            var offset2 = Qt.point(0, 0);
            if (i % 2 === 1)
            {
                offset2 = Qt.point(10, 10);
            }
            sprite.loadSingleMovingSprite("rocket_up", false,
                                          Qt.point(offset.x + offset2.x,
                                                   offset.y + offset2.y),
                                          Qt.point(80, 40), 400, false,
                                          -1, 1.0, 5, 100 * i, false);
            sprite.loadSound("rocket_launch.wav", 1, 100 * i);
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
        var i = 0;
        sprite.loadSprite("rocket_hit",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 300, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 90),
                                Qt.point(-127, -60), 400, true,
                                -1, 1, 0, 0, true);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("impact_explosion.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
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
        BATTLEANIMATION_CANNONBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, Qt.point(-38, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CANNONBOAT = new Constructor();
