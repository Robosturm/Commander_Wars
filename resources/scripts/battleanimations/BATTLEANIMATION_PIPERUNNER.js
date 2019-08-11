var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("piperunner",  false,
                          BATTLEANIMATION_PIPERUNNER.getMaxUnitCount(), Qt.point(30, 30));
        sprite.loadSprite("piperunner+mask",  true,
                          BATTLEANIMATION_PIPERUNNER.getMaxUnitCount(), Qt.point(30, 30));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_PIPERUNNER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("rocket_up", false, Qt.point(82, 82),
                                    Qt.point(128, 64), 400, false,
                                    1, 1, -1, i * 150);
        }
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
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket_up", false, 5, Qt.point(127, 0),
                                    Qt.point(-128, 64), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    1, 1, 0, 0, true);
        }
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_PIPERUNNER = new Constructor();
