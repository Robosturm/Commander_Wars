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
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        sprite.loadSprite("missile+" + armyName,  false,
                          BATTLEANIMATION_MISSILE.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("missile+" + armyName + "+mask",  true,
                          BATTLEANIMATION_MISSILE.getMaxUnitCount(), Qt.point(-5, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MISSILE = new Constructor();
