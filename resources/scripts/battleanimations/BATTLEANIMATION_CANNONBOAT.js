var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CANNONBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        sprite.loadMovingSprite("cannonboat",  false,
                                BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cannonboat+mask", GameEnums.Recoloring_Table,
                                  BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CANNONBOAT.loadStandingAnimation(sprite, unit, defender, weapon);
        // gun
        var offset = Qt.point(61, 70);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            var offset2 = Qt.point(0, 0);
            switch (i)
            {
            case 1:
                offset2 = Qt.point(-4, -5);
                break;
            case 2:
                offset2 = Qt.point(24, -10);
                break;
            case 3:
                offset2 = Qt.point(0, 0);
                break;
            case 4:
                offset2 = Qt.point(-4, -5);
                break;
            }
            sprite.loadSingleMovingSprite("medium_shot", false,
                                          Qt.point(offset.x + offset2.x,
                                                   offset.y + offset2.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 0.5, 5, 200 * i, false);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", i * 200);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CANNONBOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 1000);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CANNONBOAT = new Constructor();
