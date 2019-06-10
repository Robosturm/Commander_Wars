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
        var armyName = "+" + player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "+os"))
        {
            armyName = "";
        }
        sprite.loadSprite("megatank" + armyName,  false,
                          BATTLEANIMATION_MEGATANK.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSprite("megatank" + armyName + "+mask",  true,
                          BATTLEANIMATION_MEGATANK.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MEGATANK = new Constructor();
