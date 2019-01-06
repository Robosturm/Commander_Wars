var UNIT =
{
    init : function(unit)
    {
        unit.ammo1 = 0;
        unit.maxAmmo1 = 0;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 99;
        unit.maxFuel = 99;
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
