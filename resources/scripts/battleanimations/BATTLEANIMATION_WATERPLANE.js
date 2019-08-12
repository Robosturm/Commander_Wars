var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("waterplane",  false,
                          BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
        sprite.loadSprite("waterplane+mask",  true,
                          BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
    };
    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_WATERPLANE.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(15, 48);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(100, 0), 400, false,
                                    1, 1, -1);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(100, -50), 400, false,
                                    1, 1, -1);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {

        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 20),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
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
var BATTLEANIMATION_WATERPLANE = new Constructor();
