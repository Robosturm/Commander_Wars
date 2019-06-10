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
        sprite.loadSprite("battleship+" + armyName,  false,
                          BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("battleship+" + armyName + "+mask",  true,
                          BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BATTLESHIP = new Constructor();
