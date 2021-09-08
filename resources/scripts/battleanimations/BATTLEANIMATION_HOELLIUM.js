var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadMovingSpriteV2("hoellium+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_HOELLIUM.getMaxUnitCount(), Qt.point(-30, 10),
                                  Qt.point(0, 0), 0, false, -1);
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOELLIUM = new Constructor();
