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
        sprite.loadSprite("heavy_hovercraft+mask",  true,
                          BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_HOVERCRAFT = new Constructor();
