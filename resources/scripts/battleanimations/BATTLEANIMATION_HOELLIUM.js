var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("hoellium",  false,
                          BATTLEANIMATION_HOELLIUM.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSpriteV2("hoellium+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_HOELLIUM.getMaxUnitCount(), Qt.point(10, 10));
    };

    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true);
        animation.loadSprite(unitID + "+walk", false);
        animation.setSound("oozium_move.wav", -2);
        return animation;
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOELLIUM = new Constructor();
