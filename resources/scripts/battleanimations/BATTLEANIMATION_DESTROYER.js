var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        sprite.loadMovingSprite("destroyer",  false,
                                BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("destroyer+mask", GameEnums.Recoloring_Table,
                                  BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DESTROYER.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            // gun
            offset = Qt.point(40, 86);
            var count = sprite.getUnitCount(5);
            for (var i = 0; i < count; i++)
            {
                var offset2 = Qt.point(0, 0);
                switch (i)
                {
                case 1:
                    offset2 = Qt.point(-15, -10);
                    break;
                case 2:
                    offset2 = Qt.point(0, 0);
                    break;
                case 3:
                    offset2 = Qt.point(-15, -10);
                    break;
                case 4:
                    offset2 = Qt.point(0, 0);
                    break;
                }
                sprite.loadSingleMovingSprite("medium_shot", false,
                                              Qt.point(offset.x + offset2.x,
                                                       offset.y + offset2.y),
                                              Qt.point(0, 0), 0, true,
                                              1, 1.0, 5, 200 * i, false);
                sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 200 * i);
            }
        }
        else
        {
            // mg
            offset = Qt.point(80, 104);
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            // the time will be scaled with animation speed inside the engine
            return 1200;
        }
        else
        {
            return 1000;
        }
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 1000);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DESTROYER = new Constructor();
