var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var offset = Qt.point(-30, 5);
        sprite.loadSpriteV2("siege_cannon+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var count = sprite.getUnitCount(5);
        BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(114, 135);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("artillery_shot", false,
                                          Qt.point(offset.x, offset.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 500 * i, false);
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * 500);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + 500 * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        sprite.loadSprite("artillery_heavy_hit",  false, sprite.getMaxUnitCount(), Qt.point(-16, 20),
                          1, 1.0, 0, 0, true);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 200 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ARTILLERY.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON = new Constructor();
