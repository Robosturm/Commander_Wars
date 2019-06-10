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
            (armyName !== "bm"))
        {
            armyName = "os";
        }
        sprite.loadSprite("t_heli+" + armyName,  false,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40));
        sprite.loadSprite("t_heli+" + armyName + "+mask",  true,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_T_HELI = new Constructor();
