var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("cannonboat",  false,
                          BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("cannonboat+mask",  true,
                          BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20));
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

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CANNONBOAT = new Constructor();
