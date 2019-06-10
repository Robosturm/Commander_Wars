var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        armyName = "os";
        sprite.loadSprite("sniper+" + armyName,  false,
                          BATTLEANIMATION_SNIPER.getMaxUnitCount(), Qt.point(-35, 5));
        sprite.loadSprite("sniper+" + armyName + "+mask",  true,
                          BATTLEANIMATION_SNIPER.getMaxUnitCount(), Qt.point(-35, 5));
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain.getID() === "MOUNTAIN")
        {
            if (unitIdx >= 4)
            {
                return Qt.point(-20 * (6 - unitIdx), 0);
            }
        }
        return Qt.point(0, 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_SNIPER = new Constructor();
