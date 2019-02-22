var UNIT =
{
    init : function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(99);
        unit.setMaxFuel(99);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(1);
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
        return "ACTION_JOIN,ACTION_LOAD,ACTION_UNLOAD,ACTION_WAIT";
    },

    getName : function()
    {
        return "";
    },

    getBaseCost : function()
    {
        return -1;
    },

    createExplosionAnimation : function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", 0, -map.getImageSize(), 0, 1.5);
        return animation;
    },

    doWalkingAnimation : function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.25);
        animation.loadSprite(unitID + "+walk", false, 1.25);
        return animation;
    },

    startOfTurn : function(unit)
    {
        // gets called at the start of a turn
    },

    canMoveAndFire : function()
    {
        return false;
    },
    // number of units that can be loaded by this unit
    getLoadingPlace : function()
    {
        return 0;
    },
    getTransportUnits : function()
    {
        return [];
    },
};
