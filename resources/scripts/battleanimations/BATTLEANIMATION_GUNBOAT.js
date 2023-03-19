var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_GUNBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-30, 0), 580, Qt.point(20, 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_GUNBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-10, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, startPos, gunSprite = "+gun", gunStartFrame = 0, gunEndFrame = 0, loops = 1)
    {
        sprite.loadMovingSprite("gunboat",  false,
                                BATTLEANIMATION_GUNBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("gunboat+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_GUNBOAT.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("gunboat" + gunSprite + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_GUNBOAT.getMaxUnitCount(), Qt.point(startPos.x + 92, startPos.y + 40),
                                  movement, moveTime, false, loops, 1, 0, 0,
                                  false, 200, gunEndFrame, gunStartFrame);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_GUNBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, Qt.point(-10, 20), "+gun+fire", 0, 3, 1);

        var offset = Qt.point(106 - 10, 47 + 12);
        sprite.loadSprite("flak_shot",  false, 1, offset,
                          1, 1, 0, 100, false, true);
        sprite.loadSound("anti_air_gun_fire.wav", 1, 0);
        sprite.loadSound("anti_air_gun_fire.wav", 1, 200, 1, true);
        sprite.loadSound("anti_air_gun_fire.wav", 1, 400, 1, true);
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
        var xOffset = 0;
        var yOffset = 22;
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            yOffset = 45;
        }
        sprite.loadSprite("flak_hit",  false, count, Qt.point(xOffset, yOffset),
                          1, 1.0, 0, 0);
        BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
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
        BATTLEANIMATION_GUNBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, Qt.point(-38, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_GUNBOAT = new Constructor();
