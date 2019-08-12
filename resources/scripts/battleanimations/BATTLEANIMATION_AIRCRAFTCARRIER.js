var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("aircraftcarrier",  false,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("aircraftcarrier+mask",  true,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 20));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_AIRCRAFTCARRIER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("rocket_up", false, Qt.point(50, 75),
                                    Qt.point(128, 64), 400, false,
                                    1, 1, -1, i * 150);
        }
        sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
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
        sprite.loadMovingSprite("rocket_up", false, 5, Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                1, 1, 0, 0, true);
        sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_AIRCRAFTCARRIER = new Constructor();
