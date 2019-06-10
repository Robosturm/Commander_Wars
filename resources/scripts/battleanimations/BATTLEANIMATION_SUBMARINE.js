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
        sprite.loadSprite("submarine+" + armyName,  false,
                          BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("submarine+" + armyName + "+mask",  true,
                          BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_SUBMARINE = new Constructor();
