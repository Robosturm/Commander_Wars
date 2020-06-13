var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("zeppelin",  false,
                          BATTLEANIMATION_ZCOUNIT_KIROV.getMaxUnitCount(), Qt.point(0, 80));
        sprite.loadSpriteV2("zeppelin+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_ZCOUNIT_KIROV.getMaxUnitCount(), Qt.point(0, 80));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_KIROV.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("bombs_projectile", false, Qt.point(70, 90),
                                    Qt.point(0, -110), 400, false,
                                    1, 1, -1, i * 150);
        }
        sprite.loadSound("falling_bomb.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("bomb_falling", false, 5, Qt.point(0, 150),
                                Qt.point(0, -130), 400, true,
                                1, 1, 0, 0, true);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 0);
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_KIROV = new Constructor();
