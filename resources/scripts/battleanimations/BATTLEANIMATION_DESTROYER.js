var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("destroyer",  false,
                          BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSpriteV2("destroyer+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20));
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

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DESTROYER = new Constructor();
