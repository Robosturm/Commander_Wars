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
        sprite.loadSprite("k_heli+" + armyName,  false,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), Qt.point(-40, 30));
        sprite.loadSprite("k_heli+" + armyName + "+mask",  true,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), Qt.point(-40, 30));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_K_HELI = new Constructor();
