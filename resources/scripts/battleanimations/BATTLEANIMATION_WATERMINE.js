var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadMovingSpriteV2("watermine+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_WATERMINE.getMaxUnitCount(), Qt.point(5, 10),
                                  Qt.point(0, 0), 0, false, -1);
        sprite.loadMovingSpriteV2("watermine", GameEnums.Recoloring_None,
                                  BATTLEANIMATION_WATERMINE.getMaxUnitCount(), Qt.point(5, 10),
                                  Qt.point(0, 0), 0, false, -1);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_WATERMINE = new Constructor();
