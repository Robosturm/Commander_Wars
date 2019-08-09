var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "ac") &&
			(armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
			(armyName !== "dm") &&
			(armyName !== "ti") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        sprite.loadSprite("rocketthrower+" + armyName,  false,
                          BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), Qt.point(-10, 5));
        sprite.loadSprite("rocketthrower+" + armyName + "+mask",  true,
                          BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), Qt.point(-10, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ROCKETTHROWER = new Constructor();
