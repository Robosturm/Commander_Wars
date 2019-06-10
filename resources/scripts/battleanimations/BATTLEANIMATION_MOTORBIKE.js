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
        var armyName = "+" + player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "+bm") &&
            (armyName !== "+os"))
        {
            armyName = "";
        }
        sprite.loadSprite("motorbike" + armyName,  false,
                          BATTLEANIMATION_MOTORBIKE.getMaxUnitCount(), Qt.point(-15, 5));
        sprite.loadSprite("motorbike" + armyName + "+mask",  true,
                          BATTLEANIMATION_MOTORBIKE.getMaxUnitCount(), Qt.point(-15, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MOTORBIKE = new Constructor();
