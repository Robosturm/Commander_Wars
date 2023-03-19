var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_FRIGATE.loadSprite(sprite, unit, defender, weapon, Qt.point(-58, 0), 580, Qt.point(20, 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FRIGATE.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-38, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, startPos, loops = 1, frames = 0)
    {
        sprite.loadMovingSprite("frigate",  false,
                                BATTLEANIMATION_FRIGATE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("frigate+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_FRIGATE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("frigate+fire+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_FRIGATE.getMaxUnitCount(), Qt.point(startPos.x + 112, startPos.y + 36), movement, moveTime, false, loops,
                                  1, 0, 0, false, 100, frames);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_FRIGATE.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-38, 20), count, -1);
        var offset = Qt.point(100, 60);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSprite("light_shot", false, 1, offset,
                              1, 1, 0, 400 * i);
            sprite.loadSound("tank_shot.wav", 1, 400 * i);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 100 + 500 * sprite.getUnitCount(5);
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
        sprite.loadSprite("bullet_tank",  false, count, Qt.point(10, 30),
                          1, 1.0, 0, 0, true, true, 50);
        sprite.loadSprite("cannon_hit",  false, count, Qt.point(0, 20),
                          1, 1.0, 0, 100, true);
        sprite.addSpriteScreenshake(8, 0.98, 800, 300);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("tank_hit.wav", 1, 50 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("impact_explosion.wav", 1, 100 + i * BATTLEANIMATION.defaultFrameDelay);
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
        BATTLEANIMATION_FRIGATE.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, Qt.point(-38, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FRIGATE = new Constructor();
