var UNIT =
{
    init : function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setFuel(99);
        unit.setMaxFuel(99);
    },
    loadSprites : function(unit)
    {
    },
    getMovementType : function()
    {
        return "";
    },
    getActions : function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_WAIT";
    },
    doWalkingAnimation : function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        return animation;
    },
};
