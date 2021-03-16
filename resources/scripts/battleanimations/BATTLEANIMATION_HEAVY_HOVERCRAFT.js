var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("heavy_hovercraft",  false,
                          BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
        sprite.loadSpriteV2("heavy_hovercraft+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_HOVERCRAFT.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount());
        if (weapon === 0)
        {
            // gun
            offset = Qt.point(11, 30);
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            // mg
            offset = Qt.point(5, 48);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_HOVERCRAFT = new Constructor();
