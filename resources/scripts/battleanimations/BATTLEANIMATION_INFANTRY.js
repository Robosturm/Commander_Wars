var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        sprite.loadSprite("infantry+" + armyName,  false,
                          BATTLEANIMATION_INFANTRY.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("infantry+" + armyName + "+mask",  true,
                          BATTLEANIMATION_INFANTRY.getMaxUnitCount(), Qt.point(-5, 5));
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
var BATTLEANIMATION_INFANTRY = new Constructor();
