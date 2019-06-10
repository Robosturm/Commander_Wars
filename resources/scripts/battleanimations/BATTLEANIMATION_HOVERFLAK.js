var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        sprite.loadSprite("hoverflak",  false,
                          BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-45, 5));
        sprite.loadSprite("hoverflak+mask",  true,
                          BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-45, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERFLAK = new Constructor();
