var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
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
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "ac") &&
            (armyName !== "dm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        sprite.loadSprite("apc+" + armyName,  false,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSprite("apc+" + armyName + "+mask",  true,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_APC = new Constructor();
