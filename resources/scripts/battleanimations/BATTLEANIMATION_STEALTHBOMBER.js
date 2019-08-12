var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("stealthbomber",  false,
                          BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 60));
        sprite.loadSprite("stealthbomber+mask",  true,
                          BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 60));
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                sprite.loadSingleMovingSprite("rocket", false, Qt.point(45, 62),
                                              Qt.point(128, 0), 400, false,
                                              1, 1, -1, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("rocket_down", false, Qt.point(45, 82),
                                              Qt.point(128, -64), 400, false,
                                              1, 1, -1, i * 150);
            }
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
            sprite.loadMovingSprite("rocket", false, 5, Qt.point(127, 60),
                                    Qt.point(-128, 0), 400, true,
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
var BATTLEANIMATION_STEALTHBOMBER = new Constructor();
