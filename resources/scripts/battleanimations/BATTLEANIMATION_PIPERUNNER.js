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
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_PIPERUNNER = new Constructor();
