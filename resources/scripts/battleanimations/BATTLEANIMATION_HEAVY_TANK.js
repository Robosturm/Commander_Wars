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
        sprite.loadSprite("heavy_tank+" + armyName,  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("heavy_tank+" + armyName + "+mask",  true,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), Qt.point(-5, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_TANK = new Constructor();
