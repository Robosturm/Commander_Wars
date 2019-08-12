var BATTLEANIMATION =
{
    getMaxUnitCount : function()
    {
        return 5;
    },

    loadStandingAnimation : function(sprite, unit, defender, weapon)
    {

    },

    hasMoveInAnimation : function()
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return false;
    },

    loadMoveInAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    loadStandingFiredAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    loadFireAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    getPositionOffset : function(sprite, unit, terrain, unitIdx)
    {
        return Qt.point(0, 0);
    },

    loadImpactAnimation : function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 0);
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0);
        }
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
