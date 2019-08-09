var BATTLEANIMATION =
{
    getMaxUnitCount : function()
    {
        return 5;
    },

    loadStandingAnimation : function(sprite, unit)
    {

    },

    hasMoveInAnimation : function(sprite, unit)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return false;
    },

    loadMoveInAnimation : function(sprite, unit)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
    },

    loadFireAnimation : function(sprite, unit)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
    },

    getPositionOffset : function(sprite, unit, terrain, unitIdx)
    {
        return Qt.point(0, 0);
    },

    loadImpactAnimation : function(sprite, unit)
    {
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0);
    },

    getImpactDurationMS : function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1000;
    },

    getFireDurationMS : function()
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    },

    getMoveInDurationMS : function()
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    },
};
