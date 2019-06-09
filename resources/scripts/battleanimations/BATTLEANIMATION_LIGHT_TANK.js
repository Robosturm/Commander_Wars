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
        var zoom = 1;
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
        sprite.loadSprite("light_tank+" + armyName,  false,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("light_tank+" + armyName + "+mask",  true,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-5, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LIGHT_TANK = new Constructor();
