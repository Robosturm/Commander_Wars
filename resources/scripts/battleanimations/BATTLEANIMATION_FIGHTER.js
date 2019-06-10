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
        sprite.loadSprite("fighter+" + armyName,  false,
                          BATTLEANIMATION_FIGHTER.getMaxUnitCount(), Qt.point(-40, 20));
        sprite.loadSprite("fighter+" + armyName + "+mask",  true,
                          BATTLEANIMATION_FIGHTER.getMaxUnitCount(), Qt.point(-40, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FIGHTER = new Constructor();
