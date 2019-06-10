var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
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
        sprite.loadSprite("cruiser+" + armyName,  false,
                          BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("cruiser+" + armyName + "+mask",  true,
                          BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CRUISER = new Constructor();
