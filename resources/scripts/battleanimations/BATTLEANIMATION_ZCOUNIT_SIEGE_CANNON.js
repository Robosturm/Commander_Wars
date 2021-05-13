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
        sprite.loadSprite("siege_cannon",  false,
                          BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("siege_cannon+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var count = sprite.getUnitCount(5);
        BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(114, 135);
        // 148 137
        // -30   5
        // - 4  -7
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("artillery_shot", false,
                                          Qt.point(offset.x, offset.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 500 * i, false);
            sprite.loadSound("cannon_weapon_fire.wav", 1, "resources/sounds/", i * 500);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000 + 500 * sprite.getUnitCount(5);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    }
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_SIEGE_CANNON = new Constructor();
