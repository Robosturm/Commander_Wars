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

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, startPos)
    {
        sprite.loadMovingSprite("frigate",  false,
                                BATTLEANIMATION_FRIGATE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("frigate+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_FRIGATE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_FRIGATE.loadStandingAnimation(sprite, unit, defender, weapon, Qt.point(-38, 20));
        var offset = Qt.point(100, 60);
        sprite.loadSprite("flak_shot",  false, sprite.getMaxUnitCount(), offset,
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
        var xOffset = 0;
        var yOffset = 22;
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            yOffset = 45;
        }
        sprite.loadSprite("flak_hit",  false, 5, Qt.point(xOffset, yOffset),
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
        BATTLEANIMATION_FRIGATE.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, Qt.point(-38, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FRIGATE = new Constructor();
