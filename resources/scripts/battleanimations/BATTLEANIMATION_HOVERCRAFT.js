var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("hovercraft",  false,
                          BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
        sprite.loadSprite("hovercraft+mask",  true,
                          BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERCRAFT = new Constructor();
